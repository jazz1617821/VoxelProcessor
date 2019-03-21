#define _USE_MATH_DEFINES
extern "C" {
#include "mymath.h"
#include "voxelvbo.h"
#include "dfvbo.h"
#include "image.h"
#include "ray.h"
}
#include "GcodeGen.h"
#include "LoadShaders.h"
#include "voxelvieweroglwidget.hpp"
#include "voxelscenemanagerwidget.hpp"
#include <QMenu>
#include <QPoint>
#include <QTableWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <math.h>

#define X 0
#define Y 1
#define Z 2

enum RENDER_MODE { RENDER_POLYGON, RENDER_WIREFRAME, RENDER_PICKING, RENDER_LINES };
enum OP_MODE { VIEW_MODE, TRANSLATE_MODE, ROTATE_MODE, SCALE_MODE };

extern float g_originWidth;
extern float g_originHeight;

VoxelViewerOGLWidget::VoxelViewerOGLWidget(QWidget * parent) : QOpenGLWidget(parent)
{
	int i;

	opMode = VIEW_MODE;

	eye[0] = 1; eye[1] = 0; eye[2] = 0;
	lok[0] = 0; lok[1] = 0; lok[2] = 0;
	vup[0] = 0, vup[1] = 1, vup[2] = 0;
	viewTrans[0] = 0; viewTrans[1] = 0; viewTrans[2] = 0;
	viewRot[0] = 0; viewRot[1] = 0; viewRot[2] = 0;
	viewPhi = radians(90);
	viewTheta = radians(60);
	viewRadius = 400.0;
	fovy = 45.0;
	nearClip = 0.1;
	farClip = 10000.0;
	orthoWidth = 50;
	fixView();

	// selected
	selectedItem = NULL;
	selectedObject = NULL;
	isTransform = false;

	// light
	for (i = 0; i < 10; ++i)
	{
		lights[i] = NULL;
	}
	numLights = 0;
	lights[numLights++] = newLight("light0");
	globalLight = newGlobalLight();
	setLightAmbient(globalLight, 0.3, 0.3, 0.3, 1.0);
	setLightPosition(lights[0], 1, 1, 1, 0);
	setLightDirection(lights[0], 1, 1, 1);
	setLightDiffuse(lights[0], 0.45, 0.45, 0.45, 1.0);
	setLightSpecular(lights[0], 0.6, 0.6, 0.6, 1.0);

//
//	Widgets
//
	// light widget
	lightWidget = new LightWidget(numLights, lights, globalLight, this);
	lightWidget->close();

	// voxel distance field widget
	vdfWidget = new VoxelDistanceFieldWidget(this);
	vdfWidget->close();

	// voxel thickness widget
	vthnWidget = new VoxelThicknessWidget(this);
	vthnWidget->close();

	// slice widget
	sliceWidget = new SliceWidget();
	sliceWidget->close();

	// voxel clip plane widget
	clipWidget = new VoxelClipPlaneWidget(this);
	clipWidget->close();

	// tamplate widget 
	templatewidget = new TemplateWidget();
	templatewidget->close();

	// initialze vbo objects
	projCylinder = NULL;

	axisX = NULL;
	axisY = NULL;
	axisZ = NULL;
	axesTsfm = newTransformation();
	selectedAxis = EMPTY_ID;

	rotCircleX = NULL;
	rotCircleY = NULL;
	rotCircleZ = NULL;
	rotCircleTsfm = newTransformation();
	selectedRotCircle = EMPTY_ID;

	scaleAxisX = NULL;
	scaleAxisY = NULL;
	scaleAxisZ = NULL;
	scaleAxesTsfm = newTransformation();
	selectedScaleAxis = EMPTY_ID;

	modifyingTransformation = newTransformation();


	// flags settings
	activeTextureMapping = false;

	// ssao
	kernelSize = 64;
	noiseSize = 4.0;
	kernelRadius = 1.0;
	quadVAO = 0;
	ssaoKernel = (GLfloat*)calloc(kernelSize * 3, sizeof(GLfloat));
	ssaoNoise = (GLfloat*)calloc(noiseSize * noiseSize * 3, sizeof(GLfloat));


	// shadow mapping
	shadowWidth = 4096;
	shadowHeight = 4096;

	// data
	vmodel = newVoxelModel();


	// effect
	projCylinder = NULL;
	enableShadowMapping = false;
	enableSSAO = false;

	//  set opengl version and profile
	setupOpenGL();

	// set context menu
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	// connect
	connect(lightWidget, SIGNAL(dataUpdate(void)), this, SLOT(update(void)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(toggleContextMenu(const QPoint&)));
	connect(vthnWidget, SIGNAL(thickDone(VoxelItem*, float, float)), this, SLOT(genDFVBO(VoxelItem*, float, float)));
	connect(sliceWidget, SIGNAL(sliceDone(void)), this, SLOT(slicing(void)));
	connect(clipWidget, SIGNAL(updateVoxelClip(int, VoxelItem*)), this, SLOT(updateVoxelClip(int, VoxelItem*)));
}

VoxelViewerOGLWidget::~VoxelViewerOGLWidget()
{
	deleteVoxelModel(&vmodel);
}

void VoxelViewerOGLWidget::genSSAO(void)
{
	// Set samplers
	glUseProgram(program[7]);
	glUniform1i(glGetUniformLocation(program[7], "gPositionDepth"), 0);
	glUniform1i(glGetUniformLocation(program[7], "gNormal"), 1);
	glUniform1i(glGetUniformLocation(program[7], "gAlbedo"), 2);
	glUniform1i(glGetUniformLocation(program[7], "ssao"), 3);
	glUseProgram(program[5]);
	glUniform1i(glGetUniformLocation(program[5], "gPositionDepth"), 0);
	glUniform1i(glGetUniformLocation(program[5], "gNormal"), 1);
	glUniform1i(glGetUniformLocation(program[5], "texNoise"), 2);

	// Set up G-Buffer
	// 3 textures:
	// 1. Positions + depth (RGBA)
	// 2. Color (RGB) 
	// 3. Normals (RGB) 
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// - Position + linear depth color buffer
	glGenTextures(1, &gPositionDepth);
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionDepth, 0);
	// - Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// - Albedo color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// - Create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "GBuffer Framebuffer not complete!\\n");
	}

	// Also create framebuffer to hold SSAO processing stage 
	glGenFramebuffers(1, &ssaoFBO);
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// - SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "SSAO Framebuffer not complete!");
	}
	// - and blur stage
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "SSAO Blur Framebuffer not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());

	int i;
	float factor, scale, sample[3];
	// Sample kernel

	for (i = 0; i < kernelSize; ++i)
	{
		sample[0] = randomF(2.0) - 1;
		sample[1] = randomF(2.0) - 1;
		sample[2] = randomF(1.0);
		normalize3fv(sample);
		factor = randomF(1.0);
		mult3x1fv(sample, factor, sample);
		scale = i / (float)kernelSize;

		// Scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		mult3x1fv(sample, scale, sample);
		ssaoKernel[i * 3] = sample[0];
		ssaoKernel[i * 3 + 1] = sample[1];
		ssaoKernel[i * 3 + 2] = sample[2];
	}

	// Noise texture
	for (i = 0; i < 16; i++)
	{
		ssaoNoise[i * 3] = randomF(2.0) - 1.0;
		ssaoNoise[i * 3 + 1] = randomF(2.0) - 1.0;
		ssaoNoise[i * 3 + 2] = 0.0;
	}

	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, noiseSize, noiseSize, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void VoxelViewerOGLWidget::genShadowMapping(void)
{
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("FBO error\\n");
	}
}

void VoxelViewerOGLWidget::genMultiSampling(void)
{
	glGenTextures(1, &multiSampleTex);
	glGenFramebuffers(1, &multiSampleFBO);
	glGenRenderbuffers(1, &multiSampleColorBuffer);
	glGenRenderbuffers(1, &multiSampleDepthBuffer);

	glEnable(GL_MULTISAMPLE);
}

void VoxelViewerOGLWidget::setupOpenGL(void)
{
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();

	format.setDepthBufferSize(64);
	format.setStencilBufferSize(8);
	format.setVersion(4, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	setFocusPolicy(Qt::StrongFocus);
}

void VoxelViewerOGLWidget::initializeGL(void)
{
	float mat[16];

	//  initialize glew
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	GGInit();

	printf("%s\n", glGetString(GL_VERSION));
	printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

	//  load shaders

	// phong shading vdata (object) render
	ShaderInfo modelShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\model.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\model.frag" },
		{ GL_NONE, NULL }
	};
	program[0] = LoadShaders(modelShaders);

	// shadow mapping for vdata render
	ShaderInfo shadowShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\shadow.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\shadow.frag" },
		{ GL_NONE, NULL }
	};
	program[1] = LoadShaders(shadowShaders);

	ShaderInfo pickingShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\picking.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\picking.frag" },
		{ GL_NONE, NULL }
	};
	program[2] = LoadShaders(pickingShaders);

	ShaderInfo simpleShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\simple.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\simple.frag" },
		{ GL_NONE, NULL }
	};
	program[3] = LoadShaders(simpleShaders);

	ShaderInfo deferredShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\ssao_geometry.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\ssao_geometry.frag" },
		{ GL_NONE, NULL }
	};
	program[4] = LoadShaders(deferredShaders);

	ShaderInfo ssaoShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\ssao.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\ssao.frag" },
		{ GL_NONE, NULL }
	};
	program[5] = LoadShaders(ssaoShaders);

	ShaderInfo ssaoBlurShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\ssao_blur.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\ssao_blur.frag" },
		{ GL_NONE, NULL }
	};
	program[6] = LoadShaders(ssaoBlurShaders);

	ShaderInfo ssaoLightPassShaders[] = {
		{ GL_VERTEX_SHADER, "shaders\\\\lightpass.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\\\lightpass.frag" },
		{ GL_NONE, NULL }
	};
	program[7] = LoadShaders(ssaoLightPassShaders);

	// use shader program
	glUseProgram(program[0]);

	// viewing varibles
	setViewingMatrix();

	// light varibles
	setLighting();

	// color varibles
	colorLoc = glGetUniformLocation(program[0], "color");

	// flag varible
	wireframe = glGetUniformLocation(program[0], "wireframe");
	glUniform1i(wireframe, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	// check assignment error
	//CheckGLErrors();

	// create gound plane
	ground = newPlane(1000, 1000, 1);	//x-y plane 0-200
	setMaterialAmbient(ground->material, .19225, .19225, .19225, 1.0);
	setMaterialDiffuse(ground->material, .50754, .50754, .50754, 1.0);
	setMaterialSpecular(ground->material, .508273, .508273, .508273, 1.0);
	setMaterialShininess(ground->material, .4 * 128.0);
	//translate(0,0,-500,mat);
	rotateX(90.0, mat);
	multMat4fv(mat, ground->modelMat, ground->modelMat);
	bindData(ground);


	// create project cylinder
	//projCylinder = newCylinder(10.0, 0.0, 30.0, 360, 100);

	// create axes
	// x-axis
	axisX = newAxis(10, 2, 4, 4);
	setColorVBO(1.0, 0.0, 0.0, 1.0, axisX);
	rotateZ(-90, mat);
	multMat4fv(mat, axisX->modelMat, axisX->modelMat);
	translateVBO(axisX, true);
	registerID(axisX, 0);
	bindData(axisX);

	// y-axis
	axisY = newAxis(10, 2, 4, 4);
	setColorVBO(0.0, 1.0, 0.0, 1.0, axisY);
	registerID(axisY, 1);
	bindData(axisY);

	// z-axis
	axisZ = newAxis(10, 2, 4, 4);
	setColorVBO(0.0, 0.0, 1.0, 1.0, axisZ);
	rotateX(-90, mat);
	multMat4fv(mat, axisZ->modelMat, axisZ->modelMat);
	translateVBO(axisZ, true);
	registerID(axisZ, 2);
	bindData(axisZ);

	// create rotate circles
	rotCircleX = newCircle(1, 360, 1);
	setColor(1.0, 0.0, 0.0, 1.0, rotCircleX);
	rotateY(90, mat);
	multMat4fv(mat, rotCircleX->modelMat, rotCircleX->modelMat);
	translateVBO(rotCircleX, true);
	registerID(rotCircleX, 0);
	bindData(rotCircleX);

	// create rotate circles
	rotCircleY = newCircle(1, 360, 1);
	setColor(0.0, 1.0, 0.0, 1.0, rotCircleY);
	rotateX(90, mat);
	multMat4fv(mat, rotCircleY->modelMat, rotCircleY->modelMat);
	translateVBO(rotCircleY, true);
	registerID(rotCircleY, 1);
	bindData(rotCircleY);

	// create rotate circles
	rotCircleZ = newCircle(1, 360, 1);
	setColor(0.0, 0.0, 1.0, 1.0, rotCircleZ);
	registerID(rotCircleZ, 2);
	bindData(rotCircleZ);

	// create scale axes
	scaleAxisX = newHammer(10, 2, 6, 6, 6);
	setColorVBO(1.0, 0.0, 0.0, 1.0, scaleAxisX);
	rotateZ(-90, mat);
	multMat4fv(mat, scaleAxisX->modelMat, scaleAxisX->modelMat);
	translateVBO(scaleAxisX, true);
	registerID(scaleAxisX, 0);
	bindData(scaleAxisX);

	// create scale axes
	scaleAxisY = newHammer(10, 2, 6, 6, 6);
	setColorVBO(0.0, 1.0, 0.0, 1.0, scaleAxisY);
	registerID(scaleAxisY, 1);
	bindData(scaleAxisY);

	// create scale axes
	scaleAxisZ = newHammer(10, 2, 6, 6, 6);
	setColorVBO(0.0, 0.0, 1.0, 1.0, scaleAxisZ);
	rotateX(-90, mat);
	multMat4fv(mat, scaleAxisZ->modelMat, scaleAxisZ->modelMat);
	translateVBO(scaleAxisZ, true);
	registerID(scaleAxisZ, 2);
	bindData(scaleAxisZ);


	// ssao
	genSSAO();
	glUseProgram(program[0]);
	glUniform1i(17, enableSSAO);

	// FBO depth map
	genShadowMapping();
	glUseProgram(program[0]);
	glUniform1i(16, enableShadowMapping);

	// multiple sample FBO
	genMultiSampling();

	// enable clip plane
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CLIP_DISTANCE1);
	glEnable(GL_CLIP_DISTANCE2);
	glEnable(GL_CLIP_DISTANCE3);
	glEnable(GL_CLIP_DISTANCE4);
	glEnable(GL_CLIP_DISTANCE5);

	
	// test texture
	int width, height;
	unsigned char* data = NULL;

	//loadBMP("C:\\Users\\LuKang\\Desktop\\lab503slicer\\Textures\\test_low.bmp", &width, &height, &data);

	glGenTextures(1, &testTexID);

	glBindTexture(GL_TEXTURE_2D, testTexID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	activeTextureMapping = false;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	// init parameters
	//managerWidget->move(this->width() - managerWidget->width(), 0);
	oldWidth = this->width();
	oldHeight = this->height();
}

void VoxelViewerOGLWidget::configureShadowMapping(void)
{
	float test = 200.0;
	float test2 = 130.0;
	float nearPlane = 1.0, farPlane = 2 * test, mat[16], view[16], proj[16];
	float lit[] = { lights[0]->position[0], lights[0]->position[1], lights[0]->position[2] };
	normalize3fv(lit);
	float lightPos[] = { lit[0] * test2, lit[1] * test2, lit[2] * test2 };
	float lightLok[] = { 0, 0, 0 };
	float lightVup[] = { 0, 1, 0 };

	ortho(-test, test, -test, test, nearPlane, farPlane, proj);
	lookAt(lightPos, lightLok, lightVup, view);
	multMat4fv(proj, view, lightSpaceMat);
	transposeMat4fv(lightSpaceMat, mat);
	glUniformMatrix4fv(6, 1, GL_FALSE, mat);
}

void VoxelViewerOGLWidget::configureGBufferShader(void)
{

}

void VoxelViewerOGLWidget::renderScene(void)
{
	int i;
	VoxelItem *item;

	// draw static vmodel
	drawStaticScene();

	// reserve 99% of the back depth range for the 3D axis
	glDepthRange(0.01, 1.0);
	// draw models
	item = firstVoxelItem(vmodel->items);
	while (!isAnchorVoxelItem(item))
	{
		if (item->vdata->isEnableDF && item->vdata->df != NULL && item->vdata->df->vbo != NULL) {
			drawData(item->vdata->df->vbo, RENDER_POLYGON);
		}
		else {
			drawData(item->vdata->vbo, RENDER_POLYGON);
		}
		item = nextVoxelItem(item);
	}

	// restore depth range
	glDepthRange(0, 1.0);
}

void VoxelViewerOGLWidget::renderBbox(void)
{
	/*
	VoxelItem* item;

	item = nextVoxelItem(vmodel->items);
	while (!isAnchorVoxelItem(item))
	{
		if (item->vdata->bbox->vbo->bboxDetect) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			drawBbox(item->vdata->bbox->vbo, RENDER_WIREFRAME);
			glDisable(GL_BLEND);
		}
		item = nextVoxelItem(item);
	}
	*/
}

void VoxelViewerOGLWidget::renderOthers(void)
{
	int i;
	VoxelItem *item;

	glUseProgram(program[0]);
	glUniform1i(16, false);
	glUniform1i(17, false);

	// reserve 1% of the front depth range for the 3D axis
	glDepthRange(0, 0.01);
	if (opMode == TRANSLATE_MODE) {
		drawAxes(RENDER_POLYGON);
	}
	if (opMode == ROTATE_MODE) {
		drawRotCircle(RENDER_WIREFRAME);
	}
	if (opMode == SCALE_MODE) {
		drawScaleAxes(RENDER_POLYGON);
	}

	glDepthRange(0.01, 1.0);
	// draw models
	item = firstVoxelItem(vmodel->items);
	while (!isAnchorVoxelItem(item))
	{
		for (i = 0; i < NUM_CPP_DIR; ++i)
		{
			if (isTransform && selectedItem == item)
				break;
			if (item->vdata->isEnableDF && item->vdata->df != NULL && item->vdata->df->vbo != NULL) {
				if (item->vdata->df->vbo->clip[i]->isEnable)
					drawData(item->vdata->df->vbo->clip[i]->vbo, RENDER_POLYGON);
			}
			else {
				if (item->vdata->vbo->clip[i]->isEnable)
					drawData(item->vdata->vbo->clip[i]->vbo, RENDER_POLYGON);
			}
		}
		item = nextVoxelItem(item);
	}

	if (projCylinder != NULL) {
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniform1i(10, activeTextureMapping);	// active texture
		//glUniform1i(glGetUniformLocation(program[0], "textureMap"), 2);
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, testTexID);
		drawData(projCylinder, RENDER_POLYGON);
		//glUniform1i(10, 0); // disable texute
		//glDisable(GL_BLEND);
	}
	glDepthRange(0, 1.0);

	glUseProgram(program[0]);
	glUniform1i(16, enableShadowMapping);
	glUniform1i(17, enableSSAO);
}

void VoxelViewerOGLWidget::setViewingMatrix(void)
{
	float mat[16], tmp[16], aspect;

	// initialize variables and assign them to shader program
	// viewing matrices
	// z-axis up

	identifyMat4fv(modelMat);						// vdata matrix
	transposeMat4fv(modelMat, mat);
	glUniformMatrix4fv(0, 1, false, mat);
	lookAt(eye, lok, vup, viewMat);					// view matrix
	transposeMat4fv(viewMat, mat);
	glUniformMatrix4fv(1, 1, false, mat);
	multMat4fv(viewMat, modelMat, mvMat);			// modelview matrix
	transposeMat4fv(mvMat, mat);
	glUniformMatrix4fv(3, 1, false, mat);
	normalM(mvMat, normalMat);						// normal matrix
	transposeMat4fv(normalMat, mat);
	glUniformMatrix4fv(5, 1, false, mat);
	aspect = this->width() / (float)this->height();	// projection matrix
	//perspective(fovy, aspect, nearClip, farClip, projectionMat);
	ortho(-orthoWidth, orthoWidth, -orthoWidth / aspect, orthoWidth / aspect, nearClip, farClip, projectionMat);
	transposeMat4fv(projectionMat, mat);
	glUniformMatrix4fv(2, 1, false, mat);
	multMat4fv(projectionMat, viewMat, mvpMat);
	multMat4fv(mvpMat, modelMat, mvpMat);
	transposeMat4fv(mvpMat, mat);
	glUniformMatrix4fv(4, 1, false, mat);
	glViewport(0, 0, this->width(), this->height());
}

void VoxelViewerOGLWidget::setLighting(void)
{
	GLuint loc;

	glUniform4fv(7, 1, lights[0]->position);
	loc = glGetUniformLocation(program[0], "lightAmbient0");
	glUniform4fv(loc, 1, globalLight->ambient);
	loc = glGetUniformLocation(program[0], "lightDiffuse0");
	glUniform4fv(loc, 1, lights[0]->diffuse);
	loc = glGetUniformLocation(program[0], "lightSpecular0");
	glUniform4fv(loc, 1, lights[0]->specular);
}

void VoxelViewerOGLWidget::setClipping(VBO *vbo)
{
	int i;
	float clip[6][4] = { 0 };

	if (isTransform && selectedItem->vdata->vbo == vbo) {
		glUniform4fv(8, 6, &clip[0][0]);
		return;
	}
	for (i = 0; i < NUM_CPP_DIR; ++i)
	{
		if (vbo->clip[i] != NULL && vbo->clip[i]->isEnable)
			copy4fv(vbo->clip[i]->plane, clip[i]);
	}
	glUniform4fv(8, 6, &clip[0][0]);
	
}

void VoxelViewerOGLWidget::setMaterial(VBO obj)
{
	GLuint loc;

	// material varibles
	loc = glGetUniformLocation(program[0], "materialAmbient");
	glUniform4fv(loc, 1, obj.material->ambient);
	loc = glGetUniformLocation(program[0], "materialDiffuse");
	glUniform4fv(loc, 1, obj.material->diffuse);
	loc = glGetUniformLocation(program[0], "materialSpecular");
	glUniform4fv(loc, 1, obj.material->specular);
	loc = glGetUniformLocation(program[0], "shininess");
	glUniform1f(loc, obj.material->shininess);
	loc = glGetUniformLocation(program[0], "enableMaterial");
	glUniform1i(loc, obj.material->isEnable);
}

void VoxelViewerOGLWidget::fixView(void)
{
	float temp;

	if (viewTheta < 0.01) {
		viewTheta = 0.0 + 0.001;
	}
	else if (viewTheta > M_PI) {
		viewTheta = M_PI - 0.001;
	}
	else {
		vup[0] = 0;
		vup[1] = 1;
		vup[2] = 0;
	}

	eye[0] = lok[0] + viewRadius * cos(viewPhi) * sin(viewTheta);
	eye[1] = lok[1] + viewRadius * cos(viewTheta);
	eye[2] = lok[2] + viewRadius * sin(viewPhi) * sin(viewTheta);

	lookAt(eye, lok, vup, viewMat);

	float aspect = this->width() / (float)this->height();
	ortho(-orthoWidth, orthoWidth, -orthoWidth / aspect, orthoWidth / aspect, nearClip, farClip, projectionMat);
}

void VoxelViewerOGLWidget::updateViewing(void)
{
	float mat[16];

	multMat4fv(viewMat, modelMat, mvMat);
	normalM(mvMat, normalMat);
	multMat4fv(projectionMat, mvMat, mvpMat);

	transposeMat4fv(modelMat, mat);
	glUniformMatrix4fv(0, 1, false, mat);
	transposeMat4fv(viewMat, mat);
	glUniformMatrix4fv(1, 1, false, mat);
	transposeMat4fv(normalMat, mat);
	glUniformMatrix4fv(5, 1, false, mat);
	transposeMat4fv(mvMat, mat);
	glUniformMatrix4fv(3, 1, false, mat);
	transposeMat4fv(mvpMat, mat);
	glUniformMatrix4fv(4, 1, false, mat);
}

void VoxelViewerOGLWidget::bindData(VBO *vbo)
{
	int i;

	if (!vbo->isGenBuffer) {
		glGenBuffers(NUM_ATTRIBS, vbo->buffers);
		//glGenBuffers(NUM_ATTRIBS, vbo->bbox->buffers);
		vbo->isGenBuffer = true;
		//printf("gen: %d\\n", CheckGLErrors());
	}
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		if (vbo->enableBuffers[i]) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[i]);
			//printf("bindBuffer: %d\\n", CheckGLErrors());
			glBufferData(GL_ARRAY_BUFFER, vbo->numVertices * vbo->dataSize[i] * sizeof(float), vbo->attribs[i], GL_STATIC_DRAW);
			//printf("bufferData: %d\\n", CheckGLErrors());
		}
		/*
		if (vbo->bbox->enableBuffers[i]) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo->bbox->buffers[i]);
		if (i == WIREFRAME) {
		glBufferData(GL_ARRAY_BUFFER, vbo->bbox->numWFVertices * vbo->bbox->dataSize[i] * sizeof(float), vbo->bbox->attribs[i], GL_STATIC_DRAW);
		}
		else {
		glBufferData(GL_ARRAY_BUFFER, vbo->bbox->numVertices * vbo->bbox->dataSize[i] * sizeof(float), vbo->bbox->attribs[i], GL_STATIC_DRAW);
		}
		}
		*/
	}
}

void VoxelViewerOGLWidget::drawStaticScene()
{
	// draw ground
	//drawData(ground, RENDER_POLYGON);
}

void VoxelViewerOGLWidget::drawAxes(int mode)
{
	float center[3], translate[3], mat[16];
	AABB* bbox;

	if (selectedItem == NULL) return;

	bbox = selectedItem->vdata->bbox;
	aabbCenter(*bbox, center);

	tsfmTranslate(modifyingTransformation, translate);

	tsfmSetTranslate(center, axesTsfm);
	tsfmAddTranslate(translate, axesTsfm);

	transformMatrix(axesTsfm, axisX->modelMat, NULL);
	transformMatrix(axesTsfm, axisY->modelMat, NULL);
	transformMatrix(axesTsfm, axisZ->modelMat, NULL);

	drawData(axisX, mode);
	drawData(axisY, mode);
	drawData(axisZ, mode);
}

void VoxelViewerOGLWidget::drawRotCircle(int mode)
{
	float center[3], translate[3], size[3], mat[16];
	AABB* bbox;

	if (selectedItem == NULL) return;

	bbox = selectedItem->vdata->bbox;
	aabbCenter(*bbox, center);
	size[0] = size[1] = size[2] = aabbMaxSize(*bbox) / 2.0;

	tsfmTranslate(modifyingTransformation, translate);

	tsfmSetTranslate(center, rotCircleTsfm);
	tsfmSetScale(size, rotCircleTsfm);
	tsfmAddTranslate(translate, rotCircleTsfm);

	transformMatrix(rotCircleTsfm, rotCircleX->modelMat, NULL);
	transformMatrix(rotCircleTsfm, rotCircleY->modelMat, NULL);
	transformMatrix(rotCircleTsfm, rotCircleZ->modelMat, NULL);

	drawData(rotCircleX, mode);
	drawData(rotCircleY, mode);
	drawData(rotCircleZ, mode);
}

void VoxelViewerOGLWidget::drawScaleAxes(int mode)
{
	float center[3], translate[3], mat[16];
	AABB* bbox;

	if (selectedItem == NULL) return;

	bbox = selectedItem->vdata->bbox;
	aabbCenter(*bbox, center);

	tsfmTranslate(modifyingTransformation, translate);

	tsfmSetTranslate(center, scaleAxesTsfm);
	tsfmAddTranslate(translate, scaleAxesTsfm);

	transformMatrix(scaleAxesTsfm, scaleAxisX->modelMat, NULL);
	transformMatrix(scaleAxesTsfm, scaleAxisY->modelMat, NULL);
	transformMatrix(scaleAxesTsfm, scaleAxisZ->modelMat, NULL);

	drawData(scaleAxisX, mode);
	drawData(scaleAxisY, mode);
	drawData(scaleAxisZ, mode);
}

void VoxelViewerOGLWidget::drawData(VBO *const vbo, int mode)
{
	int i;
	float center[3], rotMat[16];

	if (vbo == NULL)
		return;

	copyMat4fv(vbo->modelMat, modelMat);
	if (isMousePressed && selectedItem != NULL) {
		VoxelData* vdata = selectedItem->vdata;
		if (vbo == vdata->vbo) {
			aabbCenter(*vdata->bbox, center);
			transformMatrix(modifyingTransformation, modelMat, center);
			multMat4fv(modelMat, vbo->modelMat, modelMat);
		}
	}

	if (mode == RENDER_PICKING) {
		if (vbo->enableBuffers[FACE]) {
			glUseProgram(program[2]);
			updateViewing();
			setLighting();
			setClipping(vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[VERTEX]);
			glVertexAttribPointer(VERTEX, vbo->dataSize[VERTEX], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERTEX);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[FACE]);
			glVertexAttribPointer(FACE, vbo->dataSize[FACE], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(FACE);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
		}
	}
	else if (mode == RENDER_WIREFRAME) {
		glUseProgram(program[3]);
		updateViewing();
		setLighting();
		setClipping(vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[WIREFRAME]);
		glVertexAttribPointer(VERTEX, vbo->dataSize[WIREFRAME], GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(VERTEX);
		glLineWidth(3);
		glUniform4f(11, vbo->color[0], vbo->color[1], vbo->color[2], vbo->color[3]);
		glDrawArrays(GL_LINES, 0, vbo->numWFVertices);
		/*
		glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[VERTEX]);
		glVertexAttribPointer(VERTEX, vbo->dataSize[VERTEX], GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(VERTEX);
		glUniform4f(0, 0.0, 0.0, 0.0, 1.0);
		glLineWidth(2.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		*/
	}
	else {
		updateViewing();
		setLighting();
		setClipping(vbo);
		setMaterial(*vbo);
		float screenSize[2];
		screenSize[0] = this->width();
		screenSize[1] = this->height();
		glUniform2fv(glGetUniformLocation(program[0], "screenSize"), 1, screenSize);
		for (i = 0; i < NUM_ATTRIBS; ++i)
		{
			if (vbo->enableBuffers[i]) {
				glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[i]);
				glVertexAttribPointer(i, vbo->dataSize[i], GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(i);
				//printf("bind: %d\\n", CheckGLErrors());
			}
		}
		if (mode == RENDER_POLYGON) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glPolygonMode(GL_BACK, GL_LINE);
			glLineWidth(2.0);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
			glLineWidth(1.0);
			glPolygonMode(GL_BACK, GL_FILL);
			glDisable(GL_CULL_FACE);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
		}
	}
	//printf("draw: %d\\n", CheckGLErrors());
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		if (vbo->enableBuffers[i]) {
			glDisableVertexAttribArray(i);
		}
	}
}

void VoxelViewerOGLWidget::drawBbox(VBO *const vbo, int mode)
{
	int i;
	float rotMat[16], color[4];

	copyMat4fv(vbo->modelMat, modelMat);

	// render bounding box
	if (mode == RENDER_POLYGON) {
		if (vbo->enableBuffers[VERTEX]) {
			glUseProgram(program[3]);
			updateViewing();
			setLighting();
			setClipping(vbo);
			setMaterial(*vbo);
			for (i = 0; i < 4; ++i)
			{
				if (vbo->enableBuffers[i]) {
					glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[i]);
					glVertexAttribPointer(i, vbo->dataSize[i], GL_FLOAT, GL_FALSE, 0, NULL);
					glEnableVertexAttribArray(i);
				}
			}
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
			glDisable(GL_BLEND);
		}
	}
	// line loop each face
	if (mode == RENDER_WIREFRAME) {
		if (vbo->enableBuffers[WIREFRAME]) {
			glUseProgram(program[3]);
			updateViewing();
			setLighting();
			setClipping(vbo);
			color[0] = 0.0;
			color[1] = 1.0;
			color[2] = 0.0;
			color[3] = 1.0;
			glUniform4fv(11, 1, color);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[WIREFRAME]);
			glVertexAttribPointer(VERTEX, vbo->dataSize[WIREFRAME], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERTEX);
			//printf("bind: %d\\n", CheckGLErrors());
			for (i = 0; i < vbo->numWFVertices; i += vbo->numWFLoop)
			{
				glDrawArrays(GL_LINE_LOOP, i, vbo->numWFLoop);
			}
		}
	}
	if (mode == RENDER_PICKING) {
		if (vbo->enableBuffers[FACE]) {
			glUseProgram(program[2]);
			updateViewing();
			setLighting();
			setClipping(vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[VERTEX]);
			glVertexAttribPointer(VERTEX, vbo->dataSize[VERTEX], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERTEX);
			glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[FACE]);
			glVertexAttribPointer(FACE, vbo->dataSize[FACE], GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(FACE);
			glDrawArrays(GL_TRIANGLES, 0, vbo->numVertices);
		}
	}
	// close Attirbs
	for (i = 0; i < NUM_ATTRIBS; ++i)
	{
		if (vbo->enableBuffers[i]) {
			glDisableVertexAttribArray(i);
		}
	}

}

void VoxelViewerOGLWidget::gBufferRendering(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program[4]);
	renderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void VoxelViewerOGLWidget::ssaoTextureRendering(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program[5]);
	float screenSize[2];
	screenSize[0] = this->width();
	screenSize[1] = this->height();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glUniform1i(glGetUniformLocation(program[5], "noiseSize"), noiseSize);
	glUniform1i(glGetUniformLocation(program[5], "kernelSize"), kernelSize);
	glUniform1f(glGetUniformLocation(program[5], "radius"), kernelRadius);
	glUniform2fv(glGetUniformLocation(program[5], "screenSize"), 1, screenSize);
	// Send kernel + rotation 
	char str[100] = { '\\0' }, idx[10];
	for (GLuint i = 0; i < kernelSize; ++i)
	{
		strcpy(str, "samples[");
		sprintf(idx, "%d", i);
		strcat(str, idx);
		strcat(str, "]");
		glUniform3fv(glGetUniformLocation(program[5], str), 1, ssaoKernel + i * 3);
	}
	setViewingMatrix();
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void VoxelViewerOGLWidget::ssaoBlurRendering(void)
{
	// 3. Blur SSAO texture to remove noise
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program[6]);
	glUniform1i(glGetUniformLocation(program[6], "noiseSize"), noiseSize);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void VoxelViewerOGLWidget::ssaoLightPassRendering(void)
{
	// 4. Lighting Pass: traditional deferred Blinn-Phong lighting now with added screen-space ambient occlusion
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program[7]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glActiveTexture(GL_TEXTURE3); // Add extra SSAO texture to lighting pass
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	// Also send light relevant uniforms
	// Lights
	float lightPos[3] = { 2.0, 4.0, -2.0 };
	float lightColor[3] = { 0.7, 0.7, 0.7 };
	glUniform3fv(glGetUniformLocation(program[7], "light.Position"), 1, lightPos);
	glUniform3fv(glGetUniformLocation(program[7], "light.Color"), 1, lightColor);
	// Update attenuation parameters
	const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
	const GLfloat linear = 0.09;
	const GLfloat quadratic = 0.032;
	glUniform1f(glGetUniformLocation(program[7], "light.Linear"), linear);
	glUniform1f(glGetUniformLocation(program[7], "light.Quadratic"), quadratic);
	renderQuad();
}

void VoxelViewerOGLWidget::shadowMappingRendering(void)
{
	// 1. first render to depth map
	glUseProgram(program[1]);
	glViewport(0, 0, shadowWidth, shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	configureShadowMapping();
	renderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());

	// 2. then render vmodel as normal with shadow mapping (using depth map)
	glUseProgram(program[0]);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multiSampleTex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, this->width(), this->height(), GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, multiSampleFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, multiSampleColorBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, multiSampleColorBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, multiSampleDepthBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, multiSampleDepthBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multiSampleDepthBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multiSampleTex, 0);
}

void VoxelViewerOGLWidget::transformVoxelClip(VoxelItem *item)
{
	int i;
	VoxelData *vdata;

	if (item == NULL)
		return;

	vdata = item->vdata;
	vdata->vbo->clip[CPP_X]->plane[3] = -(vdata->bbox->min[0] + vdata->clip[CPP_X] * vdata->voxelSize[0]);
	vdata->vbo->clip[CPP_INV_X]->plane[3] = vdata->bbox->min[0] + vdata->clip[CPP_X] * vdata->voxelSize[0];
	vdata->vbo->clip[CPP_Y]->plane[3] = -(vdata->bbox->min[1] + vdata->clip[CPP_Y] * vdata->voxelSize[1]);
	vdata->vbo->clip[CPP_INV_Y]->plane[3] = vdata->bbox->min[1] + vdata->clip[CPP_INV_Y] * vdata->voxelSize[1];
	vdata->vbo->clip[CPP_Z]->plane[3] = -(vdata->bbox->min[2] + vdata->clip[CPP_Z] * vdata->voxelSize[2]);
	vdata->vbo->clip[CPP_INV_Z]->plane[3] = vdata->bbox->min[2] + vdata->clip[CPP_INV_Z] * vdata->voxelSize[2];
}

void VoxelViewerOGLWidget::paintGL(void)
{
	float mat[16], color[4];

	// ssao
	if (enableSSAO) {
		gBufferRendering();
		ssaoTextureRendering();
		ssaoBlurRendering();
	}
	//ssaoLightPassRendering();
	
	// render shadow map

	if (enableShadowMapping) {
		shadowMappingRendering();
	}

	glUseProgram(program[0]);
	glViewport(0, 0, this->width(), this->height());
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (enableShadowMapping) {
		glUniform1i(glGetUniformLocation(program[0], "shadowMap"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		transposeMat4fv(lightSpaceMat, mat);
		glUniformMatrix4fv(6, 1, GL_FALSE, mat);
	}
	if (enableSSAO) {
		glUniform1i(glGetUniformLocation(program[0], "ssaoMap"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	}
	renderScene();
	//renderBbox();
	renderOthers();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, multiSampleFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->defaultFramebufferObject());
	glDrawBuffer(GL_BACK);
	glBlitFramebuffer(0, 0, this->width(), this->height(), 0, 0, this->width(), this->height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
}

void VoxelViewerOGLWidget::resizeSSAO(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// - Position + linear depth color buffer
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// - Normal color buffer
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// - Albedo color buffer
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// - Depth buffer (renderbuffer)
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width(), this->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// - SSAO color buffer
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// - and blur stage
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->width(), this->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
}

void VoxelViewerOGLWidget::resizeGL(int width, int height)
{
	float mat[16];

	glViewport(0, 0, width, height);
	float aspect = width / (float)height;
	//perspective(fovy, aspect, nearClip, farClip, projectionMat);
	ortho(-orthoWidth, orthoWidth, -orthoWidth / aspect, orthoWidth / aspect, nearClip, farClip, projectionMat);
	transposeMat4fv(projectionMat, mat);
	glUniformMatrix4fv(2, 1, false, mat);

	/*
	// fix widget position
	QSize size = voxelizeButton->size();
	float ratio = fmin((width + 211) / (float)g_originWidth, (height + 78) / (float)g_originHeight);
	int scaledSize = iconSize * ratio;
	QPixmap pixmap(":/mainwindow/icons/voxelize.png");
	QPixmap scaled = pixmap.scaled(scaledSize, scaledSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
	QIcon ButtonIcon(scaled);
	voxelizeButton->setIcon(ButtonIcon);
	voxelizeButton->setIconSize(scaled.rect().size());
	voxelizeButton->setFixedSize(scaled.rect().size());

	voxelizeButton->move(QPoint(width - scaledSize - 10, height - scaledSize - 10));
	*/

	// reset widget position
	//managerWidget->move(width * managerWidget->pos().x() / (float)oldWidth,
	//					height * managerWidget->pos().y() / (float)oldHeight);


	resizeSSAO();

	oldWidth = width;
	oldHeight = height;

	update();
}

void VoxelViewerOGLWidget::mousePressEvent(QMouseEvent *e)
{
	int i, j, ret;
	unsigned int objID;
	float rayDir[3], boxCenter[3], param[4], dir[3], rotW[3];
	unsigned char pickedColor[4];
	VoxelItem* item;
	VoxelData* vdata;

	isMousePressed = true;
	switch (e->buttons())
	{
	case Qt::LeftButton:
		this->grabFramebuffer();

		// if a vdata be selected, then...
		if (selectedItem != NULL) {

			if (opMode == TRANSLATE_MODE) {
				// select axes by off-screen picking
				glClearColor(1.0, 1.0, 1.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawAxes(RENDER_PICKING);
				glReadPixels(e->x(), this->height() - e->y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pickedColor);
				toObjectID(pickedColor, &objID);
				selectedAxis = objID;
				if (selectedAxis != EMPTY_ID) {	// if selected, then cancel the actions bellow
					break;
				}
			}
			else if (opMode == SCALE_MODE) {
				// select scale axes by off-screen picking
				glClearColor(1.0, 1.0, 1.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawScaleAxes(RENDER_PICKING);
				glReadPixels(e->x(), this->height() - e->y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pickedColor);
				toObjectID(pickedColor, &objID);
				selectedScaleAxis = objID;
				if (selectedScaleAxis != EMPTY_ID) {	// if selected, then cancel the actions bellow
					break;
				}
			}
			else if (opMode == ROTATE_MODE) {
				// select rotate circles by off-screen picking
				glClearColor(1.0, 1.0, 1.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawRotCircle(RENDER_PICKING);
				glReadPixels(e->x(), this->height() - e->y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pickedColor);
				toObjectID(pickedColor, &objID);
				selectedRotCircle = objID;
				if (selectedRotCircle != EMPTY_ID) {	// if selected, then cancel the actions bellow
					vdata = selectedItem->vdata;
					aabbCenter(*vdata->bbox, boxCenter);
					// mouse position
					if (selectedRotCircle == 0) {
						rotW[0] = 1; rotW[1] = 0; rotW[2] = 0;					//rotW = rotation circle normal
					}
					else if (selectedRotCircle == 1) {
						rotW[0] = 0; rotW[1] = 1; rotW[2] = 0;
					}
					else if (selectedRotCircle == 2) {
						rotW[0] = 0; rotW[1] = 0; rotW[2] = -1;
					}
					compPlane(rotW, boxCenter, param);
					getRayDir(e->x(), e->y(), this->width(), this->height(), viewMat, projectionMat, dir);
					compPntOnPlane(param, eye, dir, selectedCirclePos);

					//printf("X:%f, Y:%f, Z:%f \n", selectedCirclePos[0], selectedCirclePos[1], selectedCirclePos[2]);
					break;
				}
			}
		}

		// select models by off-screen picking
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		item = firstVoxelItem(vmodel->items);
		while (!isAnchorVoxelItem(item))
		{
			vdata = item->vdata;
			drawData(vdata->vbo, RENDER_PICKING);
			item = nextVoxelItem(item);
		}
		glReadPixels(e->x(), this->height() - e->y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pickedColor);
		toObjectID(pickedColor, &objID);
		if (objID != EMPTY_ID) {
			//managerWidget->show();
			item = searchVoxelData(objID, vmodel);
			item->vdata->bbox->vbo->bboxDetect = true;
			selectedItem = item;
			aabbCenter(*item->vdata->bbox, selectedModelPos);
			emit selectVoxel(item);
			emit sendVoxelObject(item->parent);
		}
		else {
			selectedItem = NULL;
		}
		break;
	case Qt::MidButton:
		break;
	case Qt::RightButton:
		break;
	default:
		break;
	}
	oldPosX = e->x();
	oldPosY = e->y();
	update();
}

void VoxelViewerOGLWidget::mouseMoveEvent(QMouseEvent *e)
{
	float u[3] = { viewMat[0], viewMat[1], viewMat[2] };
	float v[3] = { viewMat[4], viewMat[5], viewMat[6] };
	float w[3] = { viewMat[8], viewMat[9], viewMat[10] };
	float rotVec1[3], rotVec2[3], rotW[3], rotCross[3], transMat[16], boxCenter[3], boxSize[3], param[4], projPnt[3], dir[3], tmp;
	float nWidth, nHeight, mOffX, mOffY;
	float translation[3] = { 0.0, 0.0, 0.0 };
	float rotation[3] = { 0.0, 0.0, 0.0 };
	float scale[3] = { 1.0, 1.0, 1.0 };
	int offsetX = (e->x() - oldPosX);
	int offsetY = -(e->y() - oldPosY);
	bool isColckwise;
	VoxelObject *obj;
	VoxelData *vdata;

	switch (e->buttons())
	{
	case Qt::LeftButton:
		if (selectedItem != NULL && opMode != VIEW_MODE) {		
			vdata = selectedItem->vdata;
			aabbCenter(*vdata->bbox, selectedModelPos);
			// if axis selected, then move the object along a spcific direction
			if (opMode == ROTATE_MODE && selectedRotCircle != EMPTY_ID) {
				if (selectedRotCircle == 0) {
					rotW[0] = 1; rotW[1] = 0; rotW[2] = 0;					//rotW = rotation circle normal
				}
				else if (selectedRotCircle == 1) {
					rotW[0] = 0; rotW[1] = 1; rotW[2] = 0;
				}
				else if (selectedRotCircle == 2) {
					rotW[0] = 0; rotW[1] = 0; rotW[2] = -1;
				}
				compPlane(rotW, selectedModelPos, param);					//compute rotation circle plane param 
				getRayDir(e->x(), e->y(), this->width(), this->height(), viewMat, projectionMat, dir);	//compute mouse point ray direction
				compPntOnPlane(param, eye, dir, projPnt);					//compute mouse and rotation circle plane intersection point
				subtract3fv(selectedCirclePos, selectedModelPos, rotVec1);	//	   .←(selectCirclePos)
				subtract3fv(projPnt, selectedModelPos, rotVec2);			//	  /	←(V1 : rotVec1)
				cross3fv(rotVec1, rotVec2, rotCross);						//	 /	 ↓(V2 : rotVec2)
				normalize3fv(rotCross);										//	.-------.←(projPnt : mouse project point)
				if (equal3fv(rotCross, rotW, 0.0001)) {						//  ↑(selectModelPos)
					rotation[selectedRotCircle] = -degrees(acos(dot3fv(rotVec1, rotVec2) / (length3fv(rotVec1) * length3fv(rotVec2))));
				}
				else {
					rotation[selectedRotCircle] = degrees(acos(dot3fv(rotVec1, rotVec2) / (length3fv(rotVec1) * length3fv(rotVec2))));
				}
				if (rotation[selectedRotCircle] > 360 || rotation[selectedRotCircle] < -360) {
					rotation[selectedRotCircle] = 0;
				}
				isTransform = true;
			}
			else if (opMode == TRANSLATE_MODE) {
				// calculate scales of translation
				compPlane(w, selectedModelPos, param);
				getRayDir(e->x(), e->y(), this->width(), this->height(), viewMat, projectionMat, dir);
				compPntOnPlane(param, eye, dir, projPnt);
				subtract3fv(projPnt, selectedModelPos, translation);
				if (selectedAxis == 0) {
					translation[1] = translation[2] = 0.0;
				}
				else if (selectedAxis == 1) {
					translation[0] = translation[2] = 0.0;
				}
				else if (selectedAxis == 2) {
					translation[0] = translation[1] = 0.0;
				}
				isTransform = true;
			}
			else if (opMode == SCALE_MODE && selectedScaleAxis != EMPTY_ID) {
				// calculate scales of translation
				compPlane(w, selectedModelPos, param);
				getRayDir(e->x(), e->y(), this->width(), this->height(), viewMat, projectionMat, dir);
				compPntOnPlane(param, eye, dir, projPnt);
				subtract3fv(projPnt, selectedModelPos, scale);
				aabbSize(*vdata->bbox, boxSize);
				div3fv(scale, boxSize, scale);
				if (selectedScaleAxis == 0) {
					scale[1] = scale[2] = 1.0;
				}
				else if (selectedScaleAxis == 1) {
					scale[0] = scale[2] = 1.0;
				}
				else if (selectedScaleAxis == 2) {
					scale[0] = scale[1] = 1.0;
				}
				isTransform = true;
			}
			// update modifying matrix
			tsfmSetTranslate(translation, modifyingTransformation);
			tsfmSetRotate(rotation, modifyingTransformation);
			tsfmSetScale(scale, modifyingTransformation);
			//transformModel(vdata);
			transformMatrix(modifyingTransformation, modifyingMatrix, NULL);

		}
		else {
			viewTheta += offsetY * 0.01;
			viewPhi += offsetX * 0.01;
			if (viewPhi > 2 * M_PI) viewPhi -= 2 * M_PI;
			else if (viewPhi < 2 * M_PI) viewPhi += 2 * M_PI;
		}
		break;
	case Qt::MiddleButton:
		lok[0] -= sqrt(viewRadius) * (u[0] * offsetX * 0.01 + v[0] * offsetY * 0.01);
		lok[1] -= sqrt(viewRadius) * (u[1] * offsetX * 0.01 + v[1] * offsetY * 0.01);
		lok[2] -= sqrt(viewRadius) * (u[2] * offsetX * 0.01 + v[2] * offsetY * 0.01);
		break;
	case Qt::RightButton:
		break;
	}
	oldPosX = e->x();
	oldPosY = e->y();
	fixView();
	update();
}

void VoxelViewerOGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	int i, j;
	unsigned int objID;
	unsigned char pickedColor[4];
	float center[3], translation[3], transMat[16], newModelMat[16];
	VoxelData* vdata;

	isMousePressed = false;
	switch (e->button())
	{
	case Qt::LeftButton:
		if (selectedItem != NULL) {
			vdata = selectedItem->vdata;
			aabbCenter(*vdata->bbox, center);								
			transformMatrix(modifyingTransformation, newModelMat, center);			//creat new Model Matrix ,  rotation need an odd center
			multMat4fv(newModelMat, vdata->vbo->modelMat, vdata->vbo->modelMat);	//update vbi model matrix
			transformAABB(vdata->bbox, modifyingTransformation);					//creat a new aabb
			//printf("Min: %f %f %f\n", vdata->bbox->min[0], vdata->bbox->min[1], vdata->bbox->min[2]);	
			//printf("Max: %f %f %f\n\n", vdata->bbox->max[0], vdata->bbox->max[1], vdata->bbox->max[2]);
			initTransformation(modifyingTransformation);		//reset to 0
			identifyMat4fv(modifyingMatrix);					//reset to I
			selectedAxis = selectedRotCircle = selectedScaleAxis = EMPTY_ID;	
			calVoxelObjectBBox(vmodel->root);					//reset vmodel bbox
			// update clip value
			transformVoxelClip(selectedItem);					
			// upadte clip vbo
			for (i = 0; i < NUM_CPP_DIR; ++i)
			{
				updateVoxelClipPlane(i, selectedItem->vdata);
				bindData(selectedItem->vdata->vbo->clip[i]->vbo);
			}
			isTransform = false;
		}
		break;
	case Qt::MidButton:
		break;
	case Qt::RightButton:
		break;
	default:
		break;
	}
	update();
}

void VoxelViewerOGLWidget::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == '+') {
		orthoWidth += 1;
	}
	if (e->key() == '-') {
		orthoWidth -= 1;
	}
	fixView();
	update();
}

void VoxelViewerOGLWidget::wheelEvent(QWheelEvent *e)
{
	//orthoWidth -= orthoWidth * e->delta() / 250;

	viewRadius -= viewRadius * e->delta() / 250;
	fixView();
	update();
}


// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.

void VoxelViewerOGLWidget::renderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

// Slots

void VoxelViewerOGLWidget::toggleContextMenu(const QPoint &pos)
{
	QMenu menu(tr("Actions"), this);
	QMenu panelMenu(tr("Panels"), this);

	//  Panels
	QAction action1("Lighting panel", this);
	action1.setCheckable(true);
	action1.setChecked(this->lightWidget->isVisible());
	connect(&action1, SIGNAL(triggered()), this, SLOT(openLightWidget()));
	panelMenu.addAction(&action1);

	menu.addMenu(&panelMenu);

	menu.exec(mapToGlobal(pos));
}

void VoxelViewerOGLWidget::openLightWidget(void)
{
	if (lightWidget->isVisible()) {
		lightWidget->close();
	}
	else {
		lightWidget->show();
	}
}

void VoxelViewerOGLWidget::actionOnVoxelObject(VoxelObject *obj, int action)
{
	QEventLoop loop;
	VOXEL_SCENE_OBJ_ACTIONS ac;

	ac = (VOXEL_SCENE_OBJ_ACTIONS)action;

	if (obj == NULL)
		return;

	switch (ac)
	{
	case VOBJ_SELECT:
		emit sendVoxelObject(obj);
		break;
	case VOBJ_DELETE:
		if (obj->dataItem == selectedItem) {
			selectedItem = NULL;
		}
		delVoxelObjectFromScene(vmodel, obj);
		break;
	case VOBJ_CLIP:
		clipWidget->show();
		clipWidget->setItems(vmodel->items);
		clipWidget->setCurrentItem(obj->dataItem);
		break;
	case VOBJ_ADD:
		VoxelObject *vobj;
		VoxelData* vdata;
		VoxelItem* item;
		VBO* vbo;

		templatewidget->show();

		// wait for data
		connect(templatewidget, SIGNAL(loadParaFinished()), &loop, SLOT(quit()));
		loop.exec();

		vdata = templatewidget->makeVoxelObject();
		if (vdata != NULL) {
			vobj = newVoxelObject();

			insertVoxelObject(obj, vobj);

			this->vmodel->numberOfVoxelData++;
			obj->numberOfChild++;

			item = newVoxelItem(vdata, vobj);
			pushBackVoxelItem(vmodel->items, item);
			memcpy(vobj->name, vdata->name, 10);

			//updateVoxelObjectBBox(((VoxelObject*)(vobj->node.parent)));
			calVoxelObjectBBox(vmodel->root);

			// create drawing staff
			item = firstVoxelItem(vmodel->items);
			while (!isAnchorVoxelItem(item))
			{
				vbo = createVoxelVBO(item->vdata);
				//setModelID(model, vbo);
				//addVertexBufferObject(vbo, vboa);
				setColorVBO(1.0, 1.0, 1.0, 1.0, vbo);
				bindData(vbo);
				// clip vbo
				createVoxelClipPlane(item->vdata);
				item = nextVoxelItem(item);
			}

			//sent to model manager
			emit sendVoxelModel(vmodel);

		}
		break;
	case VOBJ_EXPORT_DF:

		exportDFData(obj->dataItem->vdata);

		break;
	default:
		break;
	}
	update();
}

void VoxelViewerOGLWidget::openVDFWidget(void)
{
	if (vdfWidget->isHidden()) {
		vdfWidget->show();
		vdfWidget->setItems(vmodel->items);
	}
	else
		vdfWidget->hide();
}

void VoxelViewerOGLWidget::openVTHNWidget(void)
{
	if (vthnWidget->isHidden()) {
		vthnWidget->show();
		vthnWidget->setItems(vmodel->items);
	}
	else
		vthnWidget->hide();
}

void VoxelViewerOGLWidget::openSliceWidget(void)
{
	if (sliceWidget->isHidden()) {
		sliceWidget->show();
	}
	else {
		sliceWidget->hide();
	}
}

void VoxelViewerOGLWidget::openVoxelModel(const char *filepath)
{
	int i;
	VoxelItem *item;
	VBO *vbo;

	// open file
	openVM(filepath, vmodel, 0);
	
	// create drawing staff
	item = firstVoxelItem(vmodel->items);
	while (!isAnchorVoxelItem(item))
	{
		vbo = createVoxelVBO(item->vdata);
		//setModelID(model, vbo);
		//addVertexBufferObject(vbo, vboa);
		setColorVBO(1.0, 1.0, 1.0, 1.0, vbo);
		bindData(vbo);
		// clip vbo
		createVoxelClipPlane(item->vdata);
		item = nextVoxelItem(item);
	}
	
	//sent to model manager
	emit sendVoxelModel(vmodel);	

	update();
}

void VoxelViewerOGLWidget::saveVoxelModel()
{
	QString filename;
	const char *str;
	const char *fe;


	if (vmodel->numberOfVoxelData != 0) {
		filename = QFileDialog::getSaveFileName(
			this,
			tr("Save File"),
			"untitled",
			tr("Model Files (*.vm);"));
		QByteArray ba = filename.toLatin1();
		str = ba.data();
		fe = strrchr(str, '.');


		if (!filename.isEmpty()) {
			saveVM(str, vmodel, 0);
		}
		else {
			QMessageBox::information(this, tr("Warning"), "Failed to save the file.");
		}
	}
	else {
		QMessageBox::information(this, tr("Warning"), "There is no data to export.");
	}

}

void VoxelViewerOGLWidget::createVoxelModel()
{
	VoxelItem *item;
	VoxelObject* vobj;
	VoxelData* vdata;
	VBO *vbo;

	templatewidget->show();

	// wait for data
	QEventLoop loop;
	connect(templatewidget, SIGNAL(loadParaFinished()), &loop, SLOT(quit()));
	loop.exec();

	vdata = templatewidget->makeVoxelObject();
	templatewidget->reset();
	if (vdata != NULL) {

		vmodel->numberOfVoxelData = 1;

		vmodel->root->numberOfChild = 1;
		vobj = newVoxelObject();

		insertVoxelObject(vmodel->root, vobj);

		item = newVoxelItem(vdata, vobj);
		pushBackVoxelItem(vmodel->items, item);
		memcpy(vobj->name, vdata->name, 10);

		//updateVoxelObjectBBox(((VoxelObject*)(vobj->node.parent)));
		calVoxelObjectBBox(vmodel->root);

		// create drawing staff
		item = firstVoxelItem(vmodel->items);
		while (!isAnchorVoxelItem(item))
		{
			vbo = createVoxelVBO(item->vdata);
			//setModelID(model, vbo);
			//addVertexBufferObject(vbo, vboa);
			setColorVBO(1.0, 1.0, 1.0, 1.0, vbo);
			bindData(vbo);
			// clip vbo
			createVoxelClipPlane(item->vdata);
			item = nextVoxelItem(item);
		}



		//sent to model manager
		emit sendVoxelModel(vmodel);
	}

	update();
}

void VoxelViewerOGLWidget::exportDFData(VoxelData * obj)
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Distance Field File"), QString(), tr("Raw Data (*.raw)"));
	QByteArray ba = filename.toLatin1();
	char *str = ba.data();

	if (!filename.isEmpty()) {
		
		// write distance field to raw data file
		FILE* fp = fopen(str,"wb");

		int index = 0;

		while (index < obj->df->resolution[X] * obj->df->resolution[Y] * obj->df->resolution[Z]) {
			fprintf(fp, "%c", ((char)((int)obj->df->data[index])));
			index++;
		}
		fclose(fp);
		
		// write distance field info file
		char *infostr = (char*)malloc(sizeof(char) * (strlen(str)));
		strcpy(infostr, str);
		infostr[strrchr(infostr, '.') - infostr + 1] = '\0';
		strcat(infostr, "inf");

		char *infofilestr = strrchr(str, '/') + 1;

		FILE *fp1 = fopen(infostr, "w");

		fprintf(fp1, "raw-file=%s\n", infofilestr);
		fprintf(fp1, "resolution=%dx%dx%d\n", obj->df->resolution[X], obj->df->resolution[Y], obj->df->resolution[Z]);
		fprintf(fp1, "sample-type=%s\n", "char");
		fprintf(fp1, "ratio=%f:%f:%f\n", obj->df->voxelSize[X], obj->df->voxelSize[Y], obj->df->voxelSize[Z]);

		fclose(fp1);
		
	}
}

void VoxelViewerOGLWidget::changeToViewMode(void)
{
	opMode = VIEW_MODE;
}

void VoxelViewerOGLWidget::changeToTranslateMode(void)
{
	opMode = TRANSLATE_MODE;
}

void VoxelViewerOGLWidget::changeToRotateMode(void)
{
	opMode = ROTATE_MODE;
}

void VoxelViewerOGLWidget::changeToScaleMode(void)
{
	opMode = SCALE_MODE;
}

void VoxelViewerOGLWidget::changeToEditWidget(int widgetID)
{
	if (widgetID == 1) {
		emit(sendVoxelItem(vmodel->items));
	}
}

void VoxelViewerOGLWidget::genDFVBO(VoxelItem *item, float minValue, float maxValue)
{
	DistanceField *df;
	VBO *vbo;

	df = item->vdata->df;
	df->innerThick = minValue;
	df->outerThick = maxValue;

	vbo = createDistanceFieldVBO(df);
	bindData(vbo);

	createDFClipPlane(df);

	update();
}

void VoxelViewerOGLWidget::changeDFEnable(int state)
{
	if (selectedItem == NULL)
		return;

	selectedItem->vdata->isEnableDF = state;
}

void VoxelViewerOGLWidget::setClipItem(VoxelItem *clipItem)
{
	this->selectedClipItem = clipItem;


}

void VoxelViewerOGLWidget::updateVoxelClip(int clipIdx, VoxelItem *item)
{
	if(item->vdata->isEnableDF) {
		if (item->vdata->df->vbo->clip[clipIdx]->isEnable) {
			updateDFClipPlane(clipIdx, item->vdata->df);
			bindData(item->vdata->df->vbo->clip[clipIdx]->vbo);
		}
	}
	else {
		if (item->vdata->vbo->clip[clipIdx]->isEnable) {
			updateVoxelClipPlane(clipIdx, item->vdata);
			bindData(item->vdata->vbo->clip[clipIdx]->vbo);
		}
	}

	update();
}

void VoxelViewerOGLWidget::slicing(void)
{
	int i, j, k, l, x, y, z, index, imgRes[3], imgPos[3], imgSize, imgIndex, start, end, minZSize, maxZSize, count, level;
	float fill, sceneSize[3], voxelSize[3], origin[3];
	unsigned char *data, *img;
	char path[1024] = { '\0' }, gcodeFileName[1024] = { '\0' }, imgFileName[1024] = { '\0' }, number[10];
	bool isEmpty;
	FILE *fp;
	VoxelObject *obj;
	VoxelItem *item;
	VoxelData *vdata;
	Dequeue **sliceQueue;

	if (vmodel->numberOfVoxelData <= 0) {
		QMessageBox::warning(this, tr("Warning"), tr("Please load in a data!"));
		return;
	}

	voxelSize[0] = 0.4;
	voxelSize[1] = 0.2;
	voxelSize[2] = 0.4;

	aabbSize(*vmodel->root->bbox, sceneSize);
	imgRes[0] = ceil(sceneSize[0] / voxelSize[0]);
	imgRes[1] = ceil(sceneSize[1] / voxelSize[1]);
	imgRes[2] = ceil(sceneSize[2] / voxelSize[2]);

	imgSize = imgRes[0] * imgRes[2];
	img = (unsigned char*)calloc(imgSize, sizeof(unsigned char));

	origin[0] = vmodel->root->bbox->min[0];
	origin[1] = vmodel->root->bbox->min[1];
	origin[2] = vmodel->root->bbox->min[2];


	GetCurrentDirectoryA(1024, path);
	strcpy(gcodeFileName, path);
	strcat(gcodeFileName, "\\Gcodes\\");
	if (GetFileAttributesA(gcodeFileName) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(gcodeFileName, 0);
	}
	strcat(gcodeFileName, vmodel->name);
	strcat(gcodeFileName, ".gcode");

	// image path
	strcat(path, "\\Images\\");
	if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(path, 0);
	}
	strcat(path, vmodel->name);

	// configuration
	//machineSetting();

	start = 0;
	fill = 0;
	level = 0;
	fopen_s(&fp, gcodeFileName, "w");
	// y-direction
	for (i = 0; i < imgRes[1]; ++i)
	{
		memset(img, 0, imgRes[0] * imgRes[2] * sizeof(unsigned char));
		// slice voxel data
		item = firstVoxelItem(vmodel->items);
		while (!isAnchorVoxelItem(item))
		{
			vdata = item->vdata;
			if (i >= vdata->resolution[1]) {
				item = nextVoxelItem(item);
				continue;
			}
			imgPos[0] = floor(fabs(vdata->bbox->min[0] - origin[0]) / voxelSize[0]);
			imgPos[2] = floor(fabs(vdata->bbox->min[2] - origin[2]) / voxelSize[2]);
			y = i;
			for (z = 0; z < vdata->resolution[2]; ++z)
			{
				for (x = 0; x < vdata->resolution[0]; ++x)
				{
					index = x + y * vdata->resolution[0] + z * vdata->resolution[0] * vdata->resolution[1];
					if (vdata->rawData[index].data != VOX_EMPTY) {
						imgIndex = (x + imgPos[0]) + (z + imgPos[2]) * imgRes[0];
						img[imgIndex] = 255;
					}
				}
			}
			item = nextVoxelItem(item);
		}
		sprintf_s(number, "%d", i);
		memset(imgFileName, 0, 1024);
		strcpy(imgFileName, path);
		strcat(imgFileName, number);
		strcat(imgFileName, ".bmp");
		saveBMP(imgFileName, imgRes[0], imgRes[2], img);

		GGLoadBIN(img, imgRes[2], imgRes[0]);
		GGMain(level, start, fill, fp);
		//GGTell(img);
	}
	fclose(fp);
}