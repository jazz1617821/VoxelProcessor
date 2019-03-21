#define _USE_MATH_DEFINES
extern "C" {
#include "mymath.h"
#include "voxelvbo.h"
#include "dfvbo.h"
#include "image.h"
#include "ray.h"
}
#include "LoadShaders.h"
#include "voxeleditoroglwidget.hpp"
#include "voxelscenemanagerwidget.hpp"
#include "equivalentlabel.h"
#include <QMenu>
#include <QPoint>
#include <QTableWidget>
#include <QMessageBox>
#include <math.h>

#define X 0
#define Y 1
#define Z 2

#define R 0
#define G 1
#define B 2
#define A 3


// window mode
enum WINDOW_MODE { XAXISVIEW, YAXISVIEW, ZAXISVIEW, THREEDVIEW, FOURWINDOW };

// operation mode
enum OP_MODE { SELECT, CONTROL, FREEFORM, SMOOTHING };

// 
enum RENDER_MODE { RENDER_POLYGON, RENDER_WIREFRAME, RENDER_PICKING, RENDER_LINES };

VoxelEditorOGLWidget::VoxelEditorOGLWidget(QWidget * parent) : QOpenGLWidget(parent)
{

	// Slice setting
	numOfAxisLayer[X] = numOfAxisLayer[Y] = numOfAxisLayer[Z] = EDITSPACERESOLUTION / 2;

	// Slider
	xAxisLayer_slider = new QSlider(Qt::Vertical, this);
	xAxisLayer_slider->setRange(0, EDITSPACERESOLUTION - 1);						// range
	xAxisLayer_slider->setValue(numOfAxisLayer[X]);
	xAxisLayer_slider->setHidden(true);

	yAxisLayer_slider = new QSlider(Qt::Vertical, this);
	yAxisLayer_slider->setRange(0, EDITSPACERESOLUTION - 1);						// range
	yAxisLayer_slider->setValue(numOfAxisLayer[Y]);
	yAxisLayer_slider->setHidden(true);

	zAxisLayer_slider = new QSlider(Qt::Vertical, this);
	zAxisLayer_slider->setRange(0, EDITSPACERESOLUTION - 1);						// range
	zAxisLayer_slider->setValue(numOfAxisLayer[Z]);
	zAxisLayer_slider->setHidden(true);

	// Spinbox

	xAxisLayer_spinbox = new QSpinBox(this);
	xAxisLayer_spinbox->setRange(0, EDITSPACERESOLUTION - 1);
	xAxisLayer_spinbox->setValue(numOfAxisLayer[X]);
	xAxisLayer_spinbox->setHidden(true);

	yAxisLayer_spinbox = new QSpinBox(this);
	yAxisLayer_spinbox->setRange(0, EDITSPACERESOLUTION - 1);
	yAxisLayer_spinbox->setValue(numOfAxisLayer[Y]);
	yAxisLayer_spinbox->setHidden(true);

	zAxisLayer_spinbox = new QSpinBox(this);
	zAxisLayer_spinbox->setRange(0, EDITSPACERESOLUTION - 1);
	zAxisLayer_spinbox->setValue(numOfAxisLayer[Z]);
	zAxisLayer_spinbox->setHidden(true);

	slider_padding = 20;

	connect(this->xAxisLayer_slider, SIGNAL(sliderMoved(int)), this, SLOT(xAxisSliderChangerd(int)));
	connect(this->yAxisLayer_slider, SIGNAL(sliderMoved(int)), this, SLOT(yAxisSliderChangerd(int)));
	connect(this->zAxisLayer_slider, SIGNAL(sliderMoved(int)), this, SLOT(zAxisSliderChangerd(int)));
	connect(this->xAxisLayer_spinbox, SIGNAL(valueChanged(int)), this, SLOT(xAxisSpinboxChangerd(int)));
	connect(this->yAxisLayer_spinbox, SIGNAL(valueChanged(int)), this, SLOT(yAxisSpinboxChangerd(int)));
	connect(this->zAxisLayer_spinbox, SIGNAL(valueChanged(int)), this, SLOT(zAxisSpinboxChangerd(int)));

	windowMode = THREEDVIEW;

	//perspective view matrix init
	eye[0] = 1; eye[1] = 0; eye[2] = 0;
	lok[0] = 0; lok[1] = 0; lok[2] = 0;
	vup[0] = 0, vup[1] = 1, vup[2] = 0;
	//x paralle view matrix init
	xAxis_eye[0] = 300; xAxis_eye[1] = 0; xAxis_eye[2] = 0;
	xAxis_lok[0] = 0; xAxis_lok[1] = 0; xAxis_lok[2] = 0;
	xAxis_vup[0] = 0, xAxis_vup[1] = 1, xAxis_vup[2] = 0;
	//y paralle view matrix init
	yAxis_eye[0] = 0; yAxis_eye[1] = 300; yAxis_eye[2] = 0;
	yAxis_lok[0] = 0; yAxis_lok[1] = 0; yAxis_lok[2] = 0;
	yAxis_vup[0] = 0, yAxis_vup[1] = 0, yAxis_vup[2] = 1;
	//z paralle view matrix init
	zAxis_eye[0] = 0; zAxis_eye[1] = 0; zAxis_eye[2] = 300;
	zAxis_lok[0] = 0; zAxis_lok[1] = 0; zAxis_lok[2] = 0;
	zAxis_vup[0] = 0, zAxis_vup[1] = 1, zAxis_vup[2] = 0;

	viewTrans[0] = 0; viewTrans[1] = 0; viewTrans[2] = 0;
	viewRot[0] = 0; viewRot[1] = 0; viewRot[2] = 0;
	viewPhi = radians(90);
	viewTheta = radians(90);
	viewRadius = 800.0;
	fovy = 45.0;
	xAxis_width = EDITSPACERESOLUTION + 20;
	yAxis_width = EDITSPACERESOLUTION + 20;
	zAxis_width = EDITSPACERESOLUTION + 20;
	nearClip = 0.1;
	farClip = EDITSPACERESOLUTION * 10;
	fixView();



	// light
	for (int i = 0; i < 10; ++i)
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

	// free form button
	penButton = new QPushButton(this);
	penButton->setIcon(QIcon(("ApplicationImages/pen.ico")));
	penButton->setIconSize(QSize(60, 60));
	penButton->setStyleSheet("background-color:  rgba(0, 0, 0, 0)");
	penButton->setVisible(false);

	eraserButton = new QPushButton(this);
	eraserButton->setIcon(QIcon(("ApplicationImages/eraser.ico")));
	eraserButton->setIconSize(QSize(60, 60));
	eraserButton->setStyleSheet("background-color:  rgba(0, 0, 0, 0)");
	eraserButton->setVisible(false);

	resetButton = new QPushButton(this);
	resetButton->setIcon(QIcon(("ApplicationImages/reset.ico")));
	resetButton->setIconSize(QSize(60, 60));
	resetButton->setStyleSheet("background-color:  rgba(0, 0, 0, 0)");
	resetButton->setVisible(false);

	// free form button connect 
	connect(this->penButton, SIGNAL(pressed()), this, SLOT(penButtonPress()));
	connect(this->penButton, SIGNAL(released()), this, SLOT(penButtonRelease()));
	connect(this->eraserButton, SIGNAL(pressed()), this, SLOT(eraserButtonPress()));
	connect(this->eraserButton, SIGNAL(released()), this, SLOT(eraserButtonRelease()));
	connect(this->resetButton, SIGNAL(pressed()), this, SLOT(resetButtonPress()));
	connect(this->resetButton, SIGNAL(released()), this, SLOT(resetButtonRelease()));

	// save button
	saveButton = new QPushButton(this);
	saveButton->setIcon(QIcon(("ApplicationImages/Save.ico")));
	saveButton->setIconSize(QSize(60, 60));
	saveButton->setStyleSheet("background-color:  rgba(0, 0, 0, 0)");
	saveButton->setVisible(false);



	connect(this->saveButton, SIGNAL(pressed()), this, SLOT(saveButtonPress()));
	connect(this->saveButton, SIGNAL(released()), this, SLOT(saveButtonRelease()));

	//
	//	Widgets
	//
	// light widget
	lightWidget = new LightWidget(numLights, lights, globalLight, this);
	lightWidget->close();

	// freeform widget
	freeformwidget = new FreeFormWidget(this);
	freeformwidget->close();

	// smoothing widget
	smoothingwidget = new SmoothingWidget(this);
	smoothingwidget->close();


	
	// data


	//  set opengl version and profile
	setupOpenGL();

	// set context menu
	this->setContextMenuPolicy(Qt::CustomContextMenu);


	// widget connect
	connect(this->freeformwidget, SIGNAL(freeFormParameterChecked(int, int)), this, SLOT(setFreeFormParameter(int, int)));

	connect(this->smoothingwidget, SIGNAL(erosionClicked(int)), this, SLOT(erosion(int)));
	connect(this->smoothingwidget, SIGNAL(dilationClicked(int)), this, SLOT(dilation(int)));
	connect(this->smoothingwidget, SIGNAL(openingClicked(int, int, int)), this, SLOT(opening(int, int, int)));
	connect(this->smoothingwidget, SIGNAL(closingClicked(int, int, int)), this, SLOT(closing(int, int, int)));

	// init mode
	changeOPMode(CONTROL);
	editDataChanged = false;
}

VoxelEditorOGLWidget::~VoxelEditorOGLWidget()
{

}

// opengl function
void VoxelEditorOGLWidget::initializeGL(void)
{
	float mat[16];

	//  initialize glew
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	printf("%s\n", glGetString(GL_VERSION));
	printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));


	// enable blending
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


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

	createRangeBox(EDITSPACERESOLUTION);
	createDirectionArrow(EDITSPACERESOLUTION);

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

	// init parameters
	//managerWidget->move(this->width() - managerWidget->width(), 0);
	oldWidth = this->width();
	oldHeight = this->height();

}

void VoxelEditorOGLWidget::paintGL(void)
{
	// init button position
	penButton->setGeometry(this->width() - 80, this->height() - 80, 60, 60);
	eraserButton->setGeometry(this->width() - 160, this->height() - 80, 60, 60);
	resetButton->setGeometry(this->width() - 240, this->height() - 80, 60, 60);
	saveButton->setGeometry(this->width() - 80, 20, 60, 60);

	switch (windowMode) {
	case FOURWINDOW:

		showThreeDWindow(FOURWINDOW);
		showXAxisWindow(FOURWINDOW);
		xAxisLayer_slider->setGeometry(slider_padding, slider_padding, 80, this->height() / 2 - 2 * slider_padding - 40);
		xAxisLayer_slider->setHidden(false);
		xAxisLayer_spinbox->setGeometry(slider_padding, this->height() / 2 - 2 * slider_padding, 80, 40);
		xAxisLayer_spinbox->setHidden(false);
		showYAxisWindow(FOURWINDOW);
		yAxisLayer_slider->setGeometry(slider_padding, this->height() / 2 + slider_padding, 80, this->height() / 2 - 2 * slider_padding - 40);
		yAxisLayer_slider->setHidden(false);
		yAxisLayer_spinbox->setGeometry(slider_padding, this->height() - 2 * slider_padding, 80, 40);
		yAxisLayer_spinbox->setHidden(false);
		showZAxisWindow(FOURWINDOW);
		zAxisLayer_slider->setGeometry(this->width() / 2 + slider_padding, this->height() / 2 + slider_padding, 80, this->height() / 2 - 2 * slider_padding - 40);
		zAxisLayer_slider->setHidden(false);
		zAxisLayer_spinbox->setGeometry(this->width() / 2 + slider_padding, this->height() - 2 * slider_padding, 80, 40);
		zAxisLayer_spinbox->setHidden(false);

		break;
	case XAXISVIEW:

		xAxisLayer_slider->setGeometry(slider_padding, slider_padding, 100, this->height() - 2 * slider_padding - 40);
		xAxisLayer_slider->setHidden(false);
		yAxisLayer_slider->setHidden(true);
		zAxisLayer_slider->setHidden(true);
		xAxisLayer_spinbox->setGeometry(slider_padding, this->height() - 2 * slider_padding, 100, 40);
		xAxisLayer_spinbox->setHidden(false);
		yAxisLayer_spinbox->setHidden(true);
		zAxisLayer_spinbox->setHidden(true);
		showXAxisWindow(XAXISVIEW);

		break;
	case YAXISVIEW:

		yAxisLayer_slider->setGeometry(slider_padding, slider_padding, 100, this->height() - 2 * slider_padding - 40);
		yAxisLayer_slider->setHidden(false);
		xAxisLayer_slider->setHidden(true);
		zAxisLayer_slider->setHidden(true);
		yAxisLayer_spinbox->setGeometry(slider_padding, this->height() - 2 * slider_padding, 100, 40);
		yAxisLayer_spinbox->setHidden(false);
		xAxisLayer_spinbox->setHidden(true);
		zAxisLayer_spinbox->setHidden(true);
		showYAxisWindow(YAXISVIEW);

		break;
	case ZAXISVIEW:

		zAxisLayer_slider->setGeometry(slider_padding, slider_padding, 100, this->height() - 2 * slider_padding - 40);
		zAxisLayer_slider->setHidden(false);
		xAxisLayer_slider->setHidden(true);
		yAxisLayer_slider->setHidden(true);
		zAxisLayer_spinbox->setGeometry(slider_padding, this->height() - 2 * slider_padding, 100, 40);
		zAxisLayer_spinbox->setHidden(false);
		xAxisLayer_spinbox->setHidden(true);
		yAxisLayer_spinbox->setHidden(true);
		showZAxisWindow(ZAXISVIEW);

		break;
	case THREEDVIEW:

		xAxisLayer_slider->setHidden(true);
		yAxisLayer_slider->setHidden(true);
		zAxisLayer_slider->setHidden(true);
		xAxisLayer_spinbox->setHidden(true);
		yAxisLayer_spinbox->setHidden(true);
		zAxisLayer_spinbox->setHidden(true);
		showThreeDWindow(THREEDVIEW);

		break;
	default:
		break;
	}

}

void VoxelEditorOGLWidget::resizeGL(int width, int height)
{
	float mat[16];

	float aspect = width / (float)height;
	perspective(fovy, aspect, nearClip, farClip, projectionMat);

	ortho(-xAxis_width / 2, xAxis_width / 2, -xAxis_width / (2 * aspect), xAxis_width / (2 * aspect), 0, 600, xAxis_projectionMat);

	ortho(-yAxis_width / 2, yAxis_width / 2, -yAxis_width / (2 * aspect), yAxis_width / (2 * aspect), 0, 600, yAxis_projectionMat);

	ortho(-zAxis_width / 2, zAxis_width / 2, -zAxis_width / (2 * aspect), zAxis_width / (2 * aspect), 0, 600, zAxis_projectionMat);


	oldWidth = width;
	oldHeight = height;

	update();
}

void VoxelEditorOGLWidget::mousePressEvent(QMouseEvent *e)
{
	int i, j, ret;
	float rayDir[3], plaCenter[3], plaParam[4], dir[3], rotW[3], pnt[3];
	float selectedPos[2];
	VoxelItem* item;
	VoxelData* vdata;

	isMousePressed = true;

	switch (windowMode) {
	case THREEDVIEW:
		switch (e->buttons())
		{
		case Qt::LeftButton:
			this->grabFramebuffer();
			break;
		case Qt::MidButton:
			break;
		case Qt::RightButton:
			break;
		default:
			break;
		}
		break;
	case XAXISVIEW:
		switch (opMode) {
		case SELECT:

			break;
		case CONTROL:

			break;
		case FREEFORM:
			getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width(), this->height(), xAxis_viewMat, xAxis_projectionMat, pnt);
			pnt[X] = 0;
			setFreeFormBuffer(pnt);
			ff_oldPosX = pnt[Z];
			ff_oldPosY = pnt[Y];
			break;
		case SMOOTHING:

			break;
		default:
			break;
		}
		break;
	case YAXISVIEW:
		switch (opMode) {
		case SELECT:

			break;
		case CONTROL:

			break;
		case FREEFORM:
			getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width(), this->height(), yAxis_viewMat, yAxis_projectionMat, pnt);
			pnt[Y] = 0;
			setFreeFormBuffer(pnt);
			ff_oldPosX = pnt[X];
			ff_oldPosY = pnt[Z];
			break;
		case SMOOTHING:

			break;
		default:
			break;
		}
		break;
	case ZAXISVIEW:
		switch (opMode) {
		case SELECT:

			break;
		case CONTROL:

			break;
		case FREEFORM:
			getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width(), this->height(), zAxis_viewMat, zAxis_projectionMat, pnt);
			pnt[Z] = 0;
			setFreeFormBuffer(pnt);
			ff_oldPosX = pnt[X];
			ff_oldPosY = pnt[Y];
			break;
		case SMOOTHING:

			break;
		default:
			break;
		}
		break;
	case FOURWINDOW:
		if (e->x() > this->width() / 2 && e->y() < this->height() / 2) {		// three d window
			switch (e->buttons())
			{
			case Qt::LeftButton:
				this->grabFramebuffer();
				break;
			case Qt::MidButton:
				break;
			case Qt::RightButton:
				break;
			default:
				break;
			}
		}
		else if (e->x() < this->width() / 2 && e->y() < this->height() / 2) {	//x axis window
			switch (opMode) {
			case SELECT:
				
				break;
			case CONTROL:

				break;
			case FREEFORM:
				
				break;
			case SMOOTHING:
				

				break;
			default:
				break;
			}
		}
		else if (e->x() < this->width() / 2 && e->y() > this->height() / 2) {	//y axis window
			switch (opMode) {
			case SELECT:
				
				break;
			case CONTROL:

				break;
			case FREEFORM:
				
				break;
			case SMOOTHING:
				

				break;
			default:
				break;
			}
		}
		else if (e->x() > this->width() / 2 && e->y() > this->height() / 2) {	//z axis window
			switch (opMode) {
			case SELECT:
				
				break;
			case CONTROL:

				break;
			case FREEFORM:
				
				break;
			case SMOOTHING:
				
				break;
			default:
				break;
			}
		}
		break;
	}
	oldPosX = e->x();
	oldPosY = e->y();
	update();
}

void VoxelEditorOGLWidget::mouseMoveEvent(QMouseEvent *e)
{
	float u[3] = { viewMat[0], viewMat[1], viewMat[2] };
	float v[3] = { viewMat[4], viewMat[5], viewMat[6] };
	float w[3] = { viewMat[8], viewMat[9], viewMat[10] };

	Transformation *tsfm = newTransformation();
	float rayDir[3], plaCenter[3], plaParam[4], dir[3], rotW[3], oldpnt[3], newpnt[3];
	float selectedPos[2];
	float rota[16], trans[16], ident[16];
	float center[3] = { 0,0,0 };

	float nWidth, nHeight, mOffX, mOffY;

	int offsetX = (e->x() - oldPosX);
	int offsetY = -(e->y() - oldPosY);


	switch (windowMode) {
	case THREEDVIEW:
		switch (e->buttons())
		{
		case Qt::LeftButton:
			viewTheta += offsetY * 0.01;
			viewPhi += offsetX * 0.01;
			if (viewPhi > 2 * M_PI) viewPhi -= 2 * M_PI;
			else if (viewPhi < 2 * M_PI) viewPhi += 2 * M_PI;
			break;
		case Qt::MiddleButton:
			lok[0] -= sqrt(viewRadius) * (u[0] * offsetX * 0.01 + v[0] * offsetY * 0.01);
			lok[1] -= sqrt(viewRadius) * (u[1] * offsetX * 0.01 + v[1] * offsetY * 0.01);
			lok[2] -= sqrt(viewRadius) * (u[2] * offsetX * 0.01 + v[2] * offsetY * 0.01);
			break;
		case Qt::RightButton:
			break;
		}
		fixView();
		break;
	case XAXISVIEW:
		switch (opMode) {
		case SELECT:

			break;
		case CONTROL:
			if (isMousePressed) {
				getRayOrthoPnt(oldPosX, oldPosY, 0, 0, this->width(), this->height(), xAxis_viewMat, xAxis_projectionMat, oldpnt);
				getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width(), this->height(), xAxis_viewMat, xAxis_projectionMat, newpnt);

				for (int i = 0; i < 3; i++) {
					xAxis_eye[i] -= newpnt[i] - oldpnt[i];
					xAxis_lok[i] -= newpnt[i] - oldpnt[i];
				}
				fixView();
			}
			break;
		case FREEFORM:
			if (axisFreeFromTool[X] != NULL) {
				getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width(), this->height(), xAxis_viewMat, xAxis_projectionMat, tsfm->translate);
				tsfm->translate[X] = 0;
				tsfm->rotate[X] = tsfm->rotate[Z] = 0;
				tsfm->rotate[Y] = 90;
				transformMatrix(tsfm, axisFreeFromTool[X]->modelMat, center);
			}
			if (isMousePressed && axisFreeformBuffer[X] != NULL) {
				if (fabs(tsfm->translate[Z] - ff_oldPosX) > 1.0 || fabs(tsfm->translate[Y] - ff_oldPosY) > 1.0) {
					setFreeFormBuffer(tsfm->translate);
					ff_oldPosX = tsfm->translate[Z];
					ff_oldPosY = tsfm->translate[Y];
				}
			}


			break;
		case SMOOTHING:

			break;
		default:
			break;
		}
		break;
	case YAXISVIEW:
		switch (opMode) {
		case SELECT:

			break;
		case CONTROL:
			if (isMousePressed) {
				getRayOrthoPnt(oldPosX, oldPosY, 0, 0, this->width(), this->height(), yAxis_viewMat, yAxis_projectionMat, oldpnt);
				getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width(), this->height(), yAxis_viewMat, yAxis_projectionMat, newpnt);

				for (int i = 0; i < 3; i++) {
					yAxis_eye[i] -= newpnt[i] - oldpnt[i];
					yAxis_lok[i] -= newpnt[i] - oldpnt[i];
				}
				fixView();
			}

			break;
		case FREEFORM:
			if (axisFreeFromTool[Y] != NULL) {
				getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width(), this->height(), yAxis_viewMat, yAxis_projectionMat, tsfm->translate);
				tsfm->translate[Y] = 0;
				tsfm->rotate[Z] = tsfm->rotate[Y] = 0;
				tsfm->rotate[X] = -90;
				transformMatrix(tsfm, axisFreeFromTool[Y]->modelMat, center);
			}
			if (isMousePressed && axisFreeformBuffer[Y] != NULL) {
				if (fabs(tsfm->translate[Z] - ff_oldPosX) > 1.0 || fabs(tsfm->translate[Y] - ff_oldPosY) > 1.0) {
					setFreeFormBuffer(tsfm->translate);
					ff_oldPosX = tsfm->translate[X];
					ff_oldPosY = tsfm->translate[Z];
				}
			}

			break;
		case SMOOTHING:

			break;
		default:
			break;
		}
		break;
		break;
	case ZAXISVIEW:
		switch (opMode) {
		case SELECT:

			break;
		case CONTROL:
			if (isMousePressed) {
				getRayOrthoPnt(oldPosX, oldPosY, 0, 0, this->width(), this->height(), zAxis_viewMat, zAxis_projectionMat, oldpnt);
				getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width(), this->height(), zAxis_viewMat, zAxis_projectionMat, newpnt);
				for (int i = 0; i < 3; i++) {
					zAxis_eye[i] -= newpnt[i] - oldpnt[i];
					zAxis_lok[i] -= newpnt[i] - oldpnt[i];
				}
				fixView();
			}

			break;
		case FREEFORM:
			if (axisFreeFromTool[Z] != NULL) {
				getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width(), this->height(), zAxis_viewMat, zAxis_projectionMat, tsfm->translate);
				tsfm->translate[Z] = 0;
				transformMatrix(tsfm, axisFreeFromTool[Z]->modelMat, center);
			}
			if (isMousePressed && axisFreeformBuffer[Z] != NULL) {
				if (fabs(tsfm->translate[X] - ff_oldPosX) > 1.0 || fabs(tsfm->translate[Y] - ff_oldPosY) > 1.0) {
					setFreeFormBuffer(tsfm->translate);
					ff_oldPosX = tsfm->translate[X];
					ff_oldPosY = tsfm->translate[Y];
				}
			}
			

			break;
		case SMOOTHING:

			break;
		default:
			break;
		}
		break;
		break;
	case FOURWINDOW:
		if (e->x() > this->width() / 2 && e->y() < this->height() / 2) {		// three d window
			switch (e->buttons())
			{
			case Qt::LeftButton:
				viewTheta += offsetY * 0.01;
				viewPhi += offsetX * 0.01;
				if (viewPhi > 2 * M_PI) viewPhi -= 2 * M_PI;
				else if (viewPhi < 2 * M_PI) viewPhi += 2 * M_PI;
				break;
			case Qt::MiddleButton:
				lok[0] -= sqrt(viewRadius) * (u[0] * offsetX * 0.01 + v[0] * offsetY * 0.01);
				lok[1] -= sqrt(viewRadius) * (u[1] * offsetX * 0.01 + v[1] * offsetY * 0.01);
				lok[2] -= sqrt(viewRadius) * (u[2] * offsetX * 0.01 + v[2] * offsetY * 0.01);
				break;
			case Qt::RightButton:
				break;
			}
			fixView();
		}
		else if (e->x() < this->width() / 2 && e->y() < this->height() / 2) {	//x axis window
			switch (opMode) {
			case SELECT:

				break;
			case CONTROL:
				if (isMousePressed) {
					getRayOrthoPnt(oldPosX, oldPosY, 0, 0, this->width() / 2, this->height() / 2, xAxis_viewMat, xAxis_projectionMat, oldpnt);
					getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width() / 2, this->height() / 2, xAxis_viewMat, xAxis_projectionMat, newpnt);

					for (int i = 0; i < 3; i++) {
						xAxis_eye[i] -= newpnt[i] - oldpnt[i];
						xAxis_lok[i] -= newpnt[i] - oldpnt[i];
					}
					fixView();
				}
				break;
			case FREEFORM:
				/*if (axisFreeFromTool[X] != NULL) {
					getRayOrthoPnt(e->x(), e->y(), 0, 0, this->width() / 2, this->height() / 2, xAxis_viewMat, xAxis_projectionMat, tsfm->translate);
					tsfm->translate[X] = 0;
					tsfm->rotate[X] = tsfm->rotate[Z] = 0;
					tsfm->rotate[Y] = 90;
					transformMatrix(tsfm, axisFreeFromTool[X]->modelMat, center);
				}*/
				

				break;
			case SMOOTHING:

				break;
			default:
				break;
			}
			break;
		}
		else if (e->x() < this->width() / 2 && e->y() > this->height() / 2) {	//y axis window
			switch (opMode) {
			case SELECT:

				break;
			case CONTROL:
				if (isMousePressed) {
					getRayOrthoPnt(oldPosX, oldPosY, 0, this->height() / 2, this->width() / 2, this->height() / 2, yAxis_viewMat, yAxis_projectionMat, oldpnt);
					getRayOrthoPnt(e->x(), e->y(), 0, this->height() / 2, this->width() / 2, this->height() / 2, yAxis_viewMat, yAxis_projectionMat, newpnt);

					for (int i = 0; i < 3; i++) {
						yAxis_eye[i] -= newpnt[i] - oldpnt[i];
						yAxis_lok[i] -= newpnt[i] - oldpnt[i];
					}
					fixView();
				}
				break;
			case FREEFORM:
				/*if (axisFreeFromTool[Y] != NULL) {
					getRayOrthoPnt(e->x(), e->y(), 0, this->height() / 2, this->width() / 2, this->height() / 2, yAxis_viewMat, yAxis_projectionMat, tsfm->translate);
					tsfm->translate[Y] = 0;
					tsfm->rotate[Z] = tsfm->rotate[Y] = 0;
					tsfm->rotate[X] = -90;
					transformMatrix(tsfm, axisFreeFromTool[Y]->modelMat, center);
				}*/
				

				break;
			case SMOOTHING:

				break;
			default:
				break;
			}
			break;
		}
		else if (e->x() > this->width() / 2 && e->y() > this->height() / 2) {	//z axis window
			switch (opMode) {
			case SELECT:

				break;
			case CONTROL:
				if (isMousePressed) {
					getRayOrthoPnt(oldPosX, oldPosY, this->width() / 2, this->height() / 2, this->width() / 2, this->height() / 2, zAxis_viewMat, zAxis_projectionMat, oldpnt);
					getRayOrthoPnt(e->x(), e->y(), this->width() / 2, this->height() / 2, this->width() / 2, this->height() / 2, zAxis_viewMat, zAxis_projectionMat, newpnt);

					for (int i = 0; i < 3; i++) {
						zAxis_eye[i] -= newpnt[i] - oldpnt[i];
						zAxis_lok[i] -= newpnt[i] - oldpnt[i];
					}
					fixView();
				}
				break;
			case FREEFORM:
				/*if (axisFreeFromTool[Z] != NULL) {
					getRayOrthoPnt(e->x(), e->y(), this->width() / 2, this->height() / 2, this->width() / 2, this->height() / 2, zAxis_viewMat, zAxis_projectionMat, tsfm->translate);
					tsfm->translate[Z] = 0;
					transformMatrix(tsfm, axisFreeFromTool[Z]->modelMat, center);
				}*/
				

				break;
			case SMOOTHING:

				break;
			default:
				break;
			}
			break;
		}
		break;
	}

	free(tsfm);

	oldPosX = e->x();
	oldPosY = e->y();
	update();
}

void VoxelEditorOGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	int i, j;
	unsigned int objID;
	unsigned char pickedColor[4];
	float center[3], translation[3], transMat[16], newModelMat[16];
	VoxelData* vdata;
	VBO *vbo;

	switch (windowMode) {
	case THREEDVIEW:
		switch (e->button())
		{
		case Qt::LeftButton:
			break;
		case Qt::MidButton:
			break;
		case Qt::RightButton:
			break;
		default:
			break;
		}
		break;
	case XAXISVIEW:
		switch (opMode) {
		case FREEFORM:
			
			break;
		default:
			break;
		}
		break;
	case YAXISVIEW:
		switch (opMode) {
		case FREEFORM:
			
			break;
		default:
			break;
		}
		break;
	case ZAXISVIEW:
		switch (opMode) {
		case FREEFORM:

			break;
		default:
			break;
		}
		break;
	case FOURWINDOW:
		if (e->x() > this->width() / 2 && e->y() < this->height() / 2) {		// three d window
			
		}
		else if (e->x() < this->width() / 2 && e->y() < this->height() / 2) {	//x axis window
			
		}
		else if (e->x() < this->width() / 2 && e->y() > this->height() / 2) {	//y axis window
			
		}
		else if (e->x() > this->width() / 2 && e->y() > this->height() / 2) {	//z axis window
			
		}
		break;
	}

	isMousePressed = false;
	update();
}

void VoxelEditorOGLWidget::wheelEvent(QWheelEvent *e)
{
	switch (windowMode) {
	case THREEDVIEW:
		viewRadius -= viewRadius * e->delta() / 250;
		break;
	case XAXISVIEW:
		xAxis_width -= e->delta() / 10;
		break;
	case YAXISVIEW:
		yAxis_width -= e->delta() / 10;
		break;
	case ZAXISVIEW:
		zAxis_width -= e->delta() / 10;
		break;
	case FOURWINDOW:
		if (e->x() > this->width() / 2 && e->y() < this->height() / 2) {		// three d window
			viewRadius -= viewRadius * e->delta() / 250;
		}
		else if (e->x() < this->width() / 2 && e->y() < this->height() / 2) {	//x axis window
			xAxis_width -= e->delta() / 10;
		}
		else if (e->x() < this->width() / 2 && e->y() > this->height() / 2) {	//y axis window
			yAxis_width -= e->delta() / 10;
		}
		else if (e->x() > this->width() / 2 && e->y() > this->height() / 2) {	//z axis window
			zAxis_width -= e->delta() / 10;
		}
		break;
	}
	fixView();
	update();
}

void VoxelEditorOGLWidget::keyPressEvent(QKeyEvent *e)
{
	VBO *vbo;

	if (e->key() == '1') {
		if (editedobj != NULL) {
			restoreToEditData(windowMode);
			setSliceData(YAXISVIEW);
		}
		windowMode = YAXISVIEW;
	}
	else if (e->key() == '3') {
		if (editedobj != NULL) {
			restoreToEditData(windowMode);
			setSliceData(ZAXISVIEW);
		}
		windowMode = ZAXISVIEW;
	}
	else if (e->key() == '7') {
		if (editedobj != NULL) {
			restoreToEditData(windowMode);
			setSliceData(XAXISVIEW);
		}
		windowMode = XAXISVIEW;
	}
	else if (e->key() == '9') {
		if (editedobj != NULL) {
			restoreToEditData(windowMode);
			if (editDataChanged && windowMode != FOURWINDOW && windowMode != THREEDVIEW) {
				deleteVBO(&editedobj->vbo);
				vbo = createEditVoxelVBO(editedobj);
				setColorVBO(1.0, 1.0, 1.0, 1.0, vbo);
				bindData(vbo);
				editDataChanged = false;
			}
		}

		windowMode = THREEDVIEW;
	}
	else if (e->key() == '0') {
		if (editedobj != NULL) {
			restoreToEditData(windowMode);
			if (editDataChanged && windowMode != FOURWINDOW && windowMode != THREEDVIEW) {
				deleteVBO(&editedobj->vbo);
				vbo = createEditVoxelVBO(editedobj);
				setColorVBO(1.0, 1.0, 1.0, 1.0, vbo);
				bindData(vbo);
				editDataChanged = false;
			}
			setSliceData(XAXISVIEW);
			setSliceData(YAXISVIEW);
			setSliceData(ZAXISVIEW);
		}
		windowMode = FOURWINDOW;
	}
	else if (e->key() == '5') {
		switch (windowMode) {
		case XAXISVIEW:
			xAxis_eye[X] *= -1;
			xAxis_eye[Y] *= -1;
			xAxis_eye[Z] *= -1;
			break;
		case YAXISVIEW:
			yAxis_eye[X] *= -1;
			yAxis_eye[Y] *= -1;
			yAxis_eye[Z] *= -1;
			break;
		case ZAXISVIEW:
			zAxis_eye[X] *= -1;
			zAxis_eye[Y] *= -1;
			zAxis_eye[Z] *= -1;
			break;
		case FOURWINDOW:
			xAxis_eye[X] *= -1;
			xAxis_eye[Y] *= -1;
			xAxis_eye[Z] *= -1;
			yAxis_eye[X] *= -1;
			yAxis_eye[Y] *= -1;
			yAxis_eye[Z] *= -1;
			zAxis_eye[X] *= -1;
			zAxis_eye[Y] *= -1;
			zAxis_eye[Z] *= -1;
			break;
		}
		fixView();
	}

	if (e->key() == Qt::Key_Up) {

	}
	else if (e->key() == Qt::Key_Down) {

	}
	else if (e->key() == Qt::Key_Left) {

	}
	else if (e->key() == Qt::Key_Right) {

	}

	if (e->key() == Qt::Key_Delete) {

	}
	else if (e->key() == Qt::Key_Insert) {

	}

	if (e->key() == Qt::Key_Control) {
		
	}

	if (e->key() == Qt::Key_Shift) {

	}


	update();
}

void VoxelEditorOGLWidget::keyReleaseEvent(QKeyEvent *e) {

	if (e->key() == Qt::Key_Control) {
		
	}
	if (e->key() == Qt::Key_Shift) {
		
	}

	update();
}

// extended opengl function
void VoxelEditorOGLWidget::setupOpenGL(void)
{
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();

	format.setDepthBufferSize(64);
	format.setStencilBufferSize(8);
	format.setVersion(4, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	setFocusPolicy(Qt::StrongFocus);
}

void VoxelEditorOGLWidget::setViewingMatrix(void)
{
	float mat[16], tmp[16], aspect;

	// initialize variables and assign them to shader program
	// viewing matrices
	// z-axis up

	// vdata matrix
	identifyMat4fv(modelMat);
	// view matrix
	lookAt(eye, lok, vup, viewMat);
	// modelview matrix
	multMat4fv(viewMat, modelMat, mvMat);
	// normal matrix
	normalM(mvMat, normalMat);
	// projection matrix
	aspect = this->width() / (float)this->height();
	perspective(fovy, aspect, nearClip, farClip, projectionMat);
	multMat4fv(projectionMat, viewMat, mvpMat);
	multMat4fv(mvpMat, modelMat, mvpMat);

	// x axis view matrix
	lookAt(xAxis_eye, xAxis_lok, xAxis_vup, xAxis_viewMat);
	// x axis modelview matrix
	multMat4fv(xAxis_viewMat, modelMat, xAxis_mvMat);
	// x axis normal matrix
	normalM(xAxis_mvMat, xAxis_normalMat);
	// projection matrix
	aspect = this->width() / (float)this->height();
	ortho(-xAxis_width / 2, xAxis_width / 2, -xAxis_width / (2 * aspect), xAxis_width / (2 * aspect), 0, 600, xAxis_projectionMat);
	multMat4fv(xAxis_projectionMat, xAxis_viewMat, xAxis_mvpMat);
	multMat4fv(xAxis_mvpMat, modelMat, xAxis_mvpMat);


	// y axis view matrix
	lookAt(yAxis_eye, yAxis_lok, yAxis_vup, yAxis_viewMat);
	// y axis modelview matrix
	multMat4fv(yAxis_viewMat, modelMat, yAxis_mvMat);
	// y axis normal matrix
	normalM(yAxis_mvMat, yAxis_normalMat);
	// y Axis projection matrix
	aspect = this->width() / (float)this->height();
	ortho(-yAxis_width / 2, yAxis_width / 2, -yAxis_width / (2 * aspect), yAxis_width / (2 * aspect), 0, 600, yAxis_projectionMat);
	multMat4fv(yAxis_projectionMat, yAxis_viewMat, yAxis_mvpMat);
	multMat4fv(yAxis_mvpMat, modelMat, yAxis_mvpMat);

	// z axis view matrix
	lookAt(zAxis_eye, zAxis_lok, zAxis_vup, zAxis_viewMat);
	// z axis modelview matrix
	multMat4fv(zAxis_viewMat, modelMat, zAxis_mvMat);
	// z axis normal matrix
	normalM(zAxis_mvMat, zAxis_normalMat);
	// z Axis projection matrix
	aspect = this->width() / (float)this->height();
	ortho(-zAxis_width / 2, zAxis_width / 2, -zAxis_width / (2 * aspect), zAxis_width / (2 * aspect), 0, 600, zAxis_projectionMat);
	multMat4fv(zAxis_projectionMat, zAxis_viewMat, zAxis_mvpMat);
	multMat4fv(zAxis_mvpMat, modelMat, zAxis_mvpMat);


	glViewport(0, 0, this->width(), this->height());
}

void VoxelEditorOGLWidget::setLighting(void)
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

void VoxelEditorOGLWidget::setMaterial(VBO obj)
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

void VoxelEditorOGLWidget::fixView(void)
{
	float temp;

	// three d view fix
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

	lookAt(xAxis_eye, xAxis_lok, xAxis_vup, xAxis_viewMat);
	lookAt(yAxis_eye, yAxis_lok, yAxis_vup, yAxis_viewMat);
	lookAt(zAxis_eye, zAxis_lok, zAxis_vup, zAxis_viewMat);

	//axis view fix
	float aspect = this->width() / (float)this->height();
	ortho(-xAxis_width / 2, xAxis_width / 2, -xAxis_width / (2 * aspect), xAxis_width / (2 * aspect), 0, 600, xAxis_projectionMat);
	ortho(-yAxis_width / 2, yAxis_width / 2, -yAxis_width / (2 * aspect), yAxis_width / (2 * aspect), 0, 600, yAxis_projectionMat);
	ortho(-zAxis_width / 2, zAxis_width / 2, -zAxis_width / (2 * aspect), zAxis_width / (2 * aspect), 0, 600, zAxis_projectionMat);

}

void VoxelEditorOGLWidget::updateViewing(int Wmode)
{
	float mat[16];

	switch (Wmode) {
	case THREEDVIEW:
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
		break;
	case XAXISVIEW:
		multMat4fv(xAxis_viewMat, modelMat, xAxis_mvMat);
		normalM(xAxis_mvMat, xAxis_normalMat);
		multMat4fv(xAxis_projectionMat, xAxis_mvMat, xAxis_mvpMat);

		transposeMat4fv(modelMat, mat);
		glUniformMatrix4fv(0, 1, false, mat);
		transposeMat4fv(xAxis_viewMat, mat);
		glUniformMatrix4fv(1, 1, false, mat);
		transposeMat4fv(xAxis_normalMat, mat);
		glUniformMatrix4fv(5, 1, false, mat);
		transposeMat4fv(xAxis_mvMat, mat);
		glUniformMatrix4fv(3, 1, false, mat);
		transposeMat4fv(xAxis_mvpMat, mat);
		glUniformMatrix4fv(4, 1, false, mat);
		break;
	case YAXISVIEW:
		multMat4fv(yAxis_viewMat, modelMat, yAxis_mvMat);
		normalM(yAxis_mvMat, yAxis_normalMat);
		multMat4fv(yAxis_projectionMat, yAxis_mvMat, yAxis_mvpMat);

		transposeMat4fv(modelMat, mat);
		glUniformMatrix4fv(0, 1, false, mat);
		transposeMat4fv(yAxis_viewMat, mat);
		glUniformMatrix4fv(1, 1, false, mat);
		transposeMat4fv(yAxis_normalMat, mat);
		glUniformMatrix4fv(5, 1, false, mat);
		transposeMat4fv(yAxis_mvMat, mat);
		glUniformMatrix4fv(3, 1, false, mat);
		transposeMat4fv(yAxis_mvpMat, mat);
		glUniformMatrix4fv(4, 1, false, mat);
		break;
	case ZAXISVIEW:
		multMat4fv(zAxis_viewMat, modelMat, zAxis_mvMat);
		normalM(zAxis_mvMat, zAxis_normalMat);
		multMat4fv(zAxis_projectionMat, zAxis_mvMat, zAxis_mvpMat);

		transposeMat4fv(modelMat, mat);
		glUniformMatrix4fv(0, 1, false, mat);
		transposeMat4fv(zAxis_viewMat, mat);
		glUniformMatrix4fv(1, 1, false, mat);
		transposeMat4fv(zAxis_normalMat, mat);
		glUniformMatrix4fv(5, 1, false, mat);
		transposeMat4fv(zAxis_mvMat, mat);
		glUniformMatrix4fv(3, 1, false, mat);
		transposeMat4fv(zAxis_mvpMat, mat);
		glUniformMatrix4fv(4, 1, false, mat);
		break;
	default:
		break;
	}
}

void VoxelEditorOGLWidget::createRangeBox(int range)
{
	int cylinderRadius = 1;

	range = range + cylinderRadius;

	float mat[16];

	float trans[12][3] = {
		// bottom
		{ -range / 2.0, -range / 2.0, -range / 2.0 },
		{ -range / 2.0, -range / 2.0, -range / 2.0 },
		{ range / 2.0, -range / 2.0, -range / 2.0 },
		{ -range / 2.0, -range / 2.0, range / 2.0 },
		// middle
		{ -range / 2.0, -range / 2.0, -range / 2.0 },
		{ range / 2.0, -range / 2.0, -range / 2.0 },
		{ range / 2.0, -range / 2.0, range / 2.0 },
		{ -range / 2.0, -range / 2.0, range / 2.0 },
		// top
		{ -range / 2.0, -range / 2.0, range / 2.0 },
		{ range / 2.0, -range / 2.0, -range / 2.0 },
		{ range / 2.0, -range / 2.0, range / 2.0 },
		{ range / 2.0, -range / 2.0, range / 2.0 }
	};

	for (int i = 0; i < 12; i++) {
		rangeBox[i] = newCylinder(cylinderRadius, cylinderRadius, range, 50, 20);
		setMaterialAmbient(rangeBox[i]->material, .19225, .19225, .19225, 1.0);
		setMaterialDiffuse(rangeBox[i]->material, .50754, .50754, .50754, 1.0);
		setMaterialSpecular(rangeBox[i]->material, .508273, .508273, .508273, 1.0);
		setMaterialShininess(rangeBox[i]->material, .4 * 128.0);
		identifyMat4fv(rangeBox[i]->modelMat);
		if (i < 4 || i >= 8) {
			if (!(i % 2)) {
				rotateX(90, mat);
			}
			else {
				rotateZ(90, mat);
			}
		}
		else {
			rotateX(0, mat);
		}
		translate(trans[i][0], trans[i][1], trans[i][2], rangeBox[i]->modelMat);
		multMat4fv(mat, rangeBox[i]->modelMat, rangeBox[i]->modelMat);
		bindData(rangeBox[i]);
	}
}

void VoxelEditorOGLWidget::createDirectionArrow(int range)
{
	int cylinderRadius = 3;

	range = range + 1;

	float mat[16];

	float color[3][3] = {
		{ .50754, 0.0, 0.0 },
		{ 0.0, .50754, 0.0 },
		{ 0.0, 0.0, .50754 }
	};

	float trans[6][3] = {
		// x axis direction arrow
		{ range / 2.0, -range / 2.0, -range / 2.0 },
		{ range / 2.0, -range / 2.0 + range / 4, -range / 2.0 },
		
		// y axis direction arrow
		{ -range / 2.0, -range / 2.0, -range / 2.0 },
		{ -range / 2.0, -range / 2.0 + range / 4, -range / 2.0 },

		// z axis direction arrow
		{ -range / 2.0, -range / 2.0, range / 2.0 },
		{ -range / 2.0, -range / 2.0 + range / 4, range / 2.0 }
	};


	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			directionArrow[i * 2 + j] = j == 0 ? newCylinder(cylinderRadius, cylinderRadius, range / 4, 50, 20) : newCylinder(cylinderRadius * 3, 0, range / 24, 50, 20);
			setMaterialAmbient(directionArrow[i * 2 + j]->material, .19225, .19225, .19225, 1.0);
			setMaterialDiffuse(directionArrow[i * 2 + j]->material, color[i][0], color[i][1], color[i][2], 1.0);
			setMaterialSpecular(directionArrow[i * 2 + j]->material, .508273, .508273, .508273, 1.0);
			setMaterialShininess(directionArrow[i * 2 + j]->material, .4 * 128.0);
			identifyMat4fv(directionArrow[i * 2 + j]->modelMat);

			translate(trans[i * 2 + j][0], trans[i * 2 + j][1], trans[i * 2 + j][2], directionArrow[i * 2 + j]->modelMat);
			if (i == 0) {
				rotateZ(-90, mat);
			}
			else if ( i == 1) {
				rotateX(0, mat);
			}
			else {
				rotateX(-90, mat);
			}
			multMat4fv(mat, directionArrow[i * 2 + j]->modelMat, directionArrow[i * 2 + j]->modelMat);
			bindData(directionArrow[i * 2 + j]);
		}
	}

}


// render function
void VoxelEditorOGLWidget::bindData(VBO *vbo)
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
	}
}

void VoxelEditorOGLWidget::drawStaticScene(int mode)
{
	// draw range and direction arrow

	// draw range
	if (mode == XAXISVIEW) {
		drawData(rangeBox[0], RENDER_POLYGON, mode);
		drawData(rangeBox[4], RENDER_POLYGON, mode);
		drawData(rangeBox[6], RENDER_POLYGON, mode);
		drawData(rangeBox[8], RENDER_POLYGON, mode);
	}
	else if (mode == YAXISVIEW) {
		drawData(rangeBox[0], RENDER_POLYGON, mode);
		drawData(rangeBox[1], RENDER_POLYGON, mode);
		drawData(rangeBox[2], RENDER_POLYGON, mode);
		drawData(rangeBox[3], RENDER_POLYGON, mode);
	}
	else if(mode == ZAXISVIEW){
		drawData(rangeBox[1], RENDER_POLYGON, mode);
		drawData(rangeBox[4], RENDER_POLYGON, mode);
		drawData(rangeBox[5], RENDER_POLYGON, mode);
		drawData(rangeBox[9], RENDER_POLYGON, mode);
	}
	else {
		for (int i = 0; i < 12; i++) {
			drawData(rangeBox[i], RENDER_POLYGON, mode);
		}
	}

	// draw all of direction arrow   
	for (int i = 0; i < 6; i++) {
		drawData(directionArrow[i], RENDER_POLYGON, mode);
	}
}

void VoxelEditorOGLWidget::drawData(VBO *const vbo, int mode, int windowmode)
{
	int i;
	float center[3], rotMat[16];

	if (vbo == NULL)
		return;

	copyMat4fv(vbo->modelMat, modelMat);

	if (mode == RENDER_PICKING) {
		if (vbo->enableBuffers[FACE]) {
			glUseProgram(program[2]);
			updateViewing(windowmode);
			setLighting();
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
		updateViewing(windowmode);
		setLighting();
		glBindBuffer(GL_ARRAY_BUFFER, vbo->buffers[WIREFRAME]);
		glVertexAttribPointer(VERTEX, vbo->dataSize[WIREFRAME], GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(VERTEX);
		glLineWidth(3);
		glUniform4f(11, vbo->color[0], vbo->color[1], vbo->color[2], vbo->color[3]);
		glDrawArrays(GL_LINES, 0, vbo->numWFVertices);
		
	}
	else {
		glUseProgram(program[0]);
		updateViewing(windowmode);
		setLighting();
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

void VoxelEditorOGLWidget::renderScene(int mode)
{

	// reserve 99% of the back depth range for the 3D axis
	glDepthRange(0.01, 1.0);

	// draw static vmodel
	//drawStaticScene(mode);
	
	if (editedobj != NULL) {
		if (mode == THREEDVIEW) {
			drawData(editedobj->vbo, RENDER_POLYGON, mode);
		}else if (mode == XAXISVIEW) {
			drawData(axisLayerBuffer[X]->vbo, RENDER_POLYGON, mode);
		}else if (mode == YAXISVIEW) {
			drawData(axisLayerBuffer[Y]->vbo, RENDER_POLYGON, mode);
		}else if (mode == ZAXISVIEW) {
			drawData(axisLayerBuffer[Z]->vbo, RENDER_POLYGON, mode);
		}
	}

	// restore depth range
	glDepthRange(0, 0.01);

	if (opMode == FREEFORM) {
		if (mode == THREEDVIEW) {

		}
		else if (mode == XAXISVIEW) {
			// draw free form tool 
			if (axisFreeFromTool[X] != NULL) {
				drawData(axisFreeFromTool[X], RENDER_POLYGON, mode);
			}
			if (axisFreeformBuffer[X] != NULL) {
				drawData(axisFreeformBuffer[X]->vbo, RENDER_POLYGON, mode);
			}
		}
		else if (mode == YAXISVIEW) {
			// draw free form tool 
			if (axisFreeFromTool[Y] != NULL) {
				drawData(axisFreeFromTool[Y], RENDER_POLYGON, mode);
			}
			if (axisFreeformBuffer[Y] != NULL) {
				drawData(axisFreeformBuffer[Y]->vbo, RENDER_POLYGON, mode);
			}
		}
		else if (mode == ZAXISVIEW) {
			// draw free form tool 
			if (axisFreeFromTool[Z] != NULL) {
				drawData(axisFreeFromTool[Z], RENDER_POLYGON, mode);
			}
			if (axisFreeformBuffer[Z] != NULL) {
				drawData(axisFreeformBuffer[Z]->vbo, RENDER_POLYGON, mode);
			}
		}
	}

}

void VoxelEditorOGLWidget::showThreeDWindow(int mode)
{
	float mat[16], color[4];

	glUseProgram(program[0]);
	if (mode == THREEDVIEW) {
		glViewport(0, 0, this->width(), this->height());
	}
	else if(mode == FOURWINDOW){
		glViewport(this->width() / 2, this->height() / 2, this->width() / 2, this->height() / 2);
	}
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	renderScene(THREEDVIEW);
}

void VoxelEditorOGLWidget::showXAxisWindow(int mode)
{
	float mat[16], color[4];

	glUseProgram(program[0]);
	if (mode == XAXISVIEW) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, this->width(), this->height());
	}
	else if(mode == FOURWINDOW) {
		glViewport(0, this->height() / 2, this->width() / 2, this->height() / 2);
	}

	renderScene(XAXISVIEW);

}

void VoxelEditorOGLWidget::showYAxisWindow(int mode)
{
	float mat[16], color[4];

	glUseProgram(program[0]);
	if (mode == XAXISVIEW) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, this->width(), this->height());
	}
	else if (mode == FOURWINDOW) {
		glViewport(0, 0, this->width() / 2, this->height() / 2);
	}

	renderScene(YAXISVIEW);

}

void VoxelEditorOGLWidget::showZAxisWindow(int mode)
{
	float mat[16], color[4];

	glUseProgram(program[0]);
	if (mode == XAXISVIEW) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, this->width(), this->height());
	}
	else if (mode == FOURWINDOW) {
		glViewport(this->width() / 2, 0, this->width() / 2, this->height() / 2);
	}

	renderScene(ZAXISVIEW);

}


// data process
void VoxelEditorOGLWidget::initEditData(void)
{
	editedobj = newEditVoxelData();
	editedobj->bbox->max[X] = editedobj->bbox->max[Y] = editedobj->bbox->max[Z] = EDITSPACERESOLUTION / 2;
	editedobj->bbox->min[X] = editedobj->bbox->min[Y] = editedobj->bbox->min[Z] = -EDITSPACERESOLUTION / 2;
	editedobj->voxelSize[X] = editedobj->voxelSize[Y] = editedobj->voxelSize[Z] = 1.0;
	editedobj->resolution[X] = editedobj->resolution[Y] = editedobj->resolution[Z] = EDITSPACERESOLUTION;
	editedobj->rawData = (EditVoxel*)calloc(EDITSPACERESOLUTION * EDITSPACERESOLUTION * EDITSPACERESOLUTION, sizeof(EditVoxel));
}

void VoxelEditorOGLWidget::cleanEditBuffer(void)
{
	if (editedobj != NULL) {
		deleteEditVoxelData(&editedobj);
	}
	if (axisLayerBuffer[X] != NULL) {
		deleteEditVoxelData(&axisLayerBuffer[X]);
		deleteEditVoxelData(&axisLayerBuffer[Y]);
		deleteEditVoxelData(&axisLayerBuffer[Z]);
	}
}

void VoxelEditorOGLWidget::copyData(void)
{
	VBO* vbo;

	int i, j, k, voxIdx, cpyIdx, bound[3];
	int count = 0;

	bound[X] = (EDITSPACERESOLUTION - vItem->vdata->resolution[X]) / 2 - 1;
	bound[Y] = (EDITSPACERESOLUTION - vItem->vdata->resolution[Y]) / 2 - 1;
	bound[Z] = (EDITSPACERESOLUTION - vItem->vdata->resolution[Z]) / 2 - 1;

	for (i = 0; i < vItem->vdata->resolution[Z]; ++i)
	{
		for (j = 0; j <  vItem->vdata->resolution[Y]; ++j)
		{
			for (k = 0; k <  vItem->vdata->resolution[X]; ++k)
			{
				voxIdx = k + j * vItem->vdata->resolution[X] + i * vItem->vdata->resolution[X] * vItem->vdata->resolution[Y];
				cpyIdx = (k + bound[X]) +
					(j + bound[Y]) * EDITSPACERESOLUTION +
					(i + bound[Z]) * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				editedobj->rawData[cpyIdx].data = vItem->vdata->rawData[voxIdx].data;
				if (editedobj->rawData[cpyIdx].data != VOX_EMPTY) {
					editedobj->rawData[cpyIdx].ID = 1;
				}
				else {
					editedobj->rawData[cpyIdx].ID = 0;
				}
				if (editedobj->rawData[cpyIdx].ID)count++;
			} // end x
		} // end y
	} // end z

	printf("Edit data init non-zero ID count:%d\n",count);

	vbo = createEditVoxelVBO(editedobj);
	setColorVBO(1.0, 1.0, 1.0, 1.0, vbo);
	bindData(vbo);

}

void VoxelEditorOGLWidget::calculateConnetComponent(void)
{
	int x_direct_neighboor_ID, y_direct_neighboor_ID, z_direct_neighboor_ID;

	num_connecting_component = 1;

	int max_amount_compoent = 10;

	RootEquivalentLabel* relArray = newRootEquivalentLabelArray(max_amount_compoent);


	// first pass
	for (int z = 0; z < EDITSPACERESOLUTION; z++) {
		for (int y = 0; y < EDITSPACERESOLUTION; y++) {
			for (int x = 0; x < EDITSPACERESOLUTION; x++) {

				int index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				if (editedobj->rawData[index].ID != 0) {
					x_direct_neighboor_ID = y_direct_neighboor_ID = z_direct_neighboor_ID = 0;
					if (x - 1 >= 0) {
						x_direct_neighboor_ID = editedobj->rawData[index - 1].ID;
					}
					if (y - 1 >= 0) {
						y_direct_neighboor_ID = editedobj->rawData[index - (1 * EDITSPACERESOLUTION)].ID;
					}
					if (z - 1 >= 0) {
						z_direct_neighboor_ID = editedobj->rawData[index - (1 * EDITSPACERESOLUTION * EDITSPACERESOLUTION)].ID;
					}
					if (x_direct_neighboor_ID == 0 && y_direct_neighboor_ID == 0 && z_direct_neighboor_ID == 0) {		// three neighbors are zero
						editedobj->rawData[index].ID = num_connecting_component;
						num_connecting_component++;
						if (num_connecting_component == max_amount_compoent){
							relArray = resizeRootEquivalentLabelArray(relArray, max_amount_compoent, max_amount_compoent * 2);
							max_amount_compoent *= 2;
						}
					}	//two neighbors are zero
					else if (y_direct_neighboor_ID == 0 && z_direct_neighboor_ID == 0){
						editedobj->rawData[index].ID = x_direct_neighboor_ID;
					}
					else if (x_direct_neighboor_ID == 0 && z_direct_neighboor_ID == 0) {
						editedobj->rawData[index].ID = y_direct_neighboor_ID;
					}
					else if (x_direct_neighboor_ID == 0 && y_direct_neighboor_ID == 0) {
						editedobj->rawData[index].ID = z_direct_neighboor_ID;
					}	//only one neighbo is zero
					else if (z_direct_neighboor_ID == 0) {
						if (x_direct_neighboor_ID == y_direct_neighboor_ID) {
							editedobj->rawData[index].ID = x_direct_neighboor_ID;
						}
						else{
							if (x_direct_neighboor_ID < y_direct_neighboor_ID) {
								editedobj->rawData[index].ID = x_direct_neighboor_ID;
								addEquivalentLabel(relArray + x_direct_neighboor_ID, y_direct_neighboor_ID);
							}
							else {
								editedobj->rawData[index].ID = y_direct_neighboor_ID;
								addEquivalentLabel(relArray + y_direct_neighboor_ID, x_direct_neighboor_ID);
							}
						}
					}
					else if (y_direct_neighboor_ID == 0) {
						if (x_direct_neighboor_ID == z_direct_neighboor_ID) {
							editedobj->rawData[index].ID = x_direct_neighboor_ID;
						}
						else {
							if (x_direct_neighboor_ID < z_direct_neighboor_ID) {
								editedobj->rawData[index].ID = x_direct_neighboor_ID;
								addEquivalentLabel(relArray + x_direct_neighboor_ID, z_direct_neighboor_ID);
							}
							else {
								editedobj->rawData[index].ID = z_direct_neighboor_ID;
								addEquivalentLabel(relArray + z_direct_neighboor_ID, x_direct_neighboor_ID);
							}
						}
					}
					else if (x_direct_neighboor_ID == 0) {
						if (y_direct_neighboor_ID == z_direct_neighboor_ID) {
							editedobj->rawData[index].ID = y_direct_neighboor_ID;
						}
						else {
							if (y_direct_neighboor_ID < z_direct_neighboor_ID) {
								editedobj->rawData[index].ID = y_direct_neighboor_ID;
								addEquivalentLabel(relArray + y_direct_neighboor_ID, z_direct_neighboor_ID);
							}
							else {
								editedobj->rawData[index].ID = z_direct_neighboor_ID;
								addEquivalentLabel(relArray + z_direct_neighboor_ID, y_direct_neighboor_ID);
							}
						}
					}
					else { // there is no neighbor is zero
						if (x_direct_neighboor_ID == y_direct_neighboor_ID && y_direct_neighboor_ID == z_direct_neighboor_ID) {	// three neighbors are the same value
							editedobj->rawData[index].ID = x_direct_neighboor_ID;
						}
						else if (x_direct_neighboor_ID == y_direct_neighboor_ID){		// 
							if (x_direct_neighboor_ID < z_direct_neighboor_ID) {
								editedobj->rawData[index].ID = x_direct_neighboor_ID;
								addEquivalentLabel(relArray + x_direct_neighboor_ID, z_direct_neighboor_ID);
							}
							else {
								editedobj->rawData[index].ID = z_direct_neighboor_ID;
								addEquivalentLabel(relArray + z_direct_neighboor_ID, x_direct_neighboor_ID);
							}
						}
						else if (x_direct_neighboor_ID == z_direct_neighboor_ID) {
							if (x_direct_neighboor_ID < y_direct_neighboor_ID) {
								editedobj->rawData[index].ID = x_direct_neighboor_ID;
								addEquivalentLabel(relArray + x_direct_neighboor_ID, y_direct_neighboor_ID);
							}
							else {
								editedobj->rawData[index].ID = y_direct_neighboor_ID;
								addEquivalentLabel(relArray + y_direct_neighboor_ID, x_direct_neighboor_ID);
							}
						}
						else if (y_direct_neighboor_ID == z_direct_neighboor_ID) {
							if (x_direct_neighboor_ID < y_direct_neighboor_ID) {
								editedobj->rawData[index].ID = x_direct_neighboor_ID;
								addEquivalentLabel(relArray + x_direct_neighboor_ID, y_direct_neighboor_ID);
							}
							else {
								editedobj->rawData[index].ID = y_direct_neighboor_ID;
								addEquivalentLabel(relArray + y_direct_neighboor_ID, x_direct_neighboor_ID);
							}
						}
						else { //three neighbors are different
							if (x_direct_neighboor_ID < y_direct_neighboor_ID && x_direct_neighboor_ID < z_direct_neighboor_ID) {
								editedobj->rawData[index].ID = x_direct_neighboor_ID;
								addEquivalentLabel(relArray + x_direct_neighboor_ID, y_direct_neighboor_ID);
								addEquivalentLabel(relArray + x_direct_neighboor_ID, z_direct_neighboor_ID);
							}
							else if (y_direct_neighboor_ID < x_direct_neighboor_ID && y_direct_neighboor_ID < z_direct_neighboor_ID) {
								editedobj->rawData[index].ID = y_direct_neighboor_ID;
								addEquivalentLabel(relArray + y_direct_neighboor_ID, x_direct_neighboor_ID);
								addEquivalentLabel(relArray + y_direct_neighboor_ID, z_direct_neighboor_ID);
							}
							else if (z_direct_neighboor_ID < x_direct_neighboor_ID && z_direct_neighboor_ID < y_direct_neighboor_ID) {
								editedobj->rawData[index].ID = z_direct_neighboor_ID;
								addEquivalentLabel(relArray + z_direct_neighboor_ID, x_direct_neighboor_ID);
								addEquivalentLabel(relArray + z_direct_neighboor_ID, y_direct_neighboor_ID);
							}
						}
					}
				}
			}
		}
	}

	// merge Root Equivalent Label
	for (int i = 0; i < max_amount_compoent; i++) {
		if (relArray[i].count > 0) {
			EquivalentLabel* ELptr = (relArray + i)->headEL;
			for (int j = 0; j < relArray[i].count; j++, ELptr = ELptr->nextEL) {
				if ((relArray + ELptr->id)->count > 0) {
					mergeRootEquivalentLabel(relArray + i, relArray + ELptr->id);
				}
			}
		}
	}

	// 
	for (int z = 0; z < EDITSPACERESOLUTION; z++) {
		for (int y = 0; y < EDITSPACERESOLUTION; y++) {
			for (int x = 0; x < EDITSPACERESOLUTION; x++) {
				int index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				int currentID = editedobj->rawData[index].ID;
				if (currentID != 0) {
					for (int i = 1; i < max_amount_compoent; i++) {
						if (relArray[i].count > 0) {
							if (currentID == i || checkEquivalent(relArray + i, currentID)) {
								editedobj->rawData[index].ID = i * 20;
								break;
							}
						}
					}
				}
			}
		}
	}
	/*int non_zero_EL = 0;
	for (int i = 1; i < max_amount_compoent; i++) {
		if ((relArray + i)->count > 0) {
			printf("EL: %d\n", i);
			for (EquivalentLabel* ELptr = (relArray + i)->headEL; ELptr != nullptr; ELptr = ELptr->nextEL) {
				printf("%d, ", ELptr->id);
			}
			printf("\n");
			non_zero_EL++;
		}
	}
	printf("No zero EL:%d\n", non_zero_EL);*/

	FILE* fp = fopen("test.raw","wb");

	for (int z = 0; z < EDITSPACERESOLUTION; z++) {
		for (int y = 0; y < EDITSPACERESOLUTION; y++) {
			for (int x = 0; x < EDITSPACERESOLUTION; x++) {
				int index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				fwrite(&(editedobj->rawData[index].ID), sizeof(unsigned short), 1, fp);
				//if(editedobj->rawData[index].ID != 0)printf("%d, ", editedobj->rawData[index].ID);
			}
		}
	}

	fclose(fp);
	
}

void VoxelEditorOGLWidget::mallocSlice(void)
{
	int resolution[3] = { editedobj->resolution[X],editedobj->resolution[Y],editedobj->resolution[Z] };
	
	// alloc mamory
	if (axisLayerBuffer[X] != NULL) {
		axisLayerBuffer[X]->rawData = (EditVoxel*)realloc(axisLayerBuffer[X]->rawData, resolution[Y] * resolution[Z] * sizeof(EditVoxel));
		axisLayerBuffer[Y]->rawData = (EditVoxel*)realloc(axisLayerBuffer[Y]->rawData, resolution[X] * resolution[Z] * sizeof(EditVoxel));
		axisLayerBuffer[Z]->rawData = (EditVoxel*)realloc(axisLayerBuffer[Z]->rawData, resolution[X] * resolution[Y] * sizeof(EditVoxel));
	}
	else {
		axisLayerBuffer[X] = newEditVoxelData();
		axisLayerBuffer[Y] = newEditVoxelData();
		axisLayerBuffer[Z] = newEditVoxelData();
		axisLayerBuffer[X]->rawData = (EditVoxel*)calloc(resolution[Y] * resolution[Z], sizeof(EditVoxel));
		axisLayerBuffer[Y]->rawData = (EditVoxel*)calloc(resolution[X] * resolution[Z], sizeof(EditVoxel));
		axisLayerBuffer[Z]->rawData = (EditVoxel*)calloc(resolution[X] * resolution[Y], sizeof(EditVoxel));
	}

	// set resolution
	axisLayerBuffer[X]->resolution[X] = 1, axisLayerBuffer[X]->resolution[Y] = resolution[Y], axisLayerBuffer[X]->resolution[Z] = resolution[Z];
	axisLayerBuffer[Y]->resolution[X] = resolution[X], axisLayerBuffer[Y]->resolution[Y] = 1, axisLayerBuffer[Y]->resolution[Z] = resolution[Z];
	axisLayerBuffer[Z]->resolution[X] = resolution[X], axisLayerBuffer[Z]->resolution[Y] = resolution[Y], axisLayerBuffer[Z]->resolution[Z] = 1;

	// set voxel size
	axisLayerBuffer[X]->voxelSize[X] = 1, axisLayerBuffer[X]->voxelSize[Y] = 1, axisLayerBuffer[X]->voxelSize[Z] = 1;
	axisLayerBuffer[Y]->voxelSize[X] = 1, axisLayerBuffer[Y]->voxelSize[Y] = 1, axisLayerBuffer[Y]->voxelSize[Z] = 1;
	axisLayerBuffer[Z]->voxelSize[X] = 1, axisLayerBuffer[Z]->voxelSize[Y] = 1, axisLayerBuffer[Z]->voxelSize[Z] = 1;
	
	// set bbox
	axisLayerBuffer[X]->bbox->min[X] = 0, axisLayerBuffer[X]->bbox->min[Y] = editedobj->bbox->min[Y], axisLayerBuffer[X]->bbox->min[Z] = editedobj->bbox->min[Z];
	axisLayerBuffer[Y]->bbox->min[X] = editedobj->bbox->min[X], axisLayerBuffer[Y]->bbox->min[Y] = 0, axisLayerBuffer[Y]->bbox->min[Z] = editedobj->bbox->min[Z];
	axisLayerBuffer[Z]->bbox->min[X] = editedobj->bbox->min[X], axisLayerBuffer[Z]->bbox->min[Y] = editedobj->bbox->min[Y], axisLayerBuffer[Z]->bbox->min[Z] = 0;

}

void VoxelEditorOGLWidget::setSliceData(int viewmode)
{
	VBO* vbo;

	// reset data
	int count = 0;

	for (int j = 0; j < EDITSPACERESOLUTION; j++) {
		for (int i = 0; i < EDITSPACERESOLUTION; i++) {
			int axisBufferIndex = i + j * EDITSPACERESOLUTION;
			int editdataIndex;
			if (viewmode == XAXISVIEW) {
				editdataIndex = numOfAxisLayer[X] + i * EDITSPACERESOLUTION + j * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
			}
			else if (viewmode == YAXISVIEW) {
				editdataIndex = i + numOfAxisLayer[Y] * EDITSPACERESOLUTION + j * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
			}
			else if (viewmode == ZAXISVIEW) {
				editdataIndex = i + j * EDITSPACERESOLUTION + numOfAxisLayer[Z] * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
			}
			axisLayerBuffer[viewmode]->rawData[axisBufferIndex].data = editedobj->rawData[editdataIndex].data;
			axisLayerBuffer[viewmode]->rawData[axisBufferIndex].ID = editedobj->rawData[editdataIndex].ID;
		}
	}

	if ((axisLayerBuffer[viewmode]->vbo)) {
		deleteVBO(&(axisLayerBuffer[viewmode]->vbo));
	}
	vbo = createEditVoxelVBO(axisLayerBuffer[viewmode]);
	setColorVBO(axisLayerBufferColor[viewmode][R], axisLayerBufferColor[viewmode][G], axisLayerBufferColor[viewmode][B], axisLayerBufferColor[viewmode][A], vbo);
	bindData(vbo);


	/*if (viewmode == XAXISVIEW) {
		for (int z = 0; z < resolution[Z]; z++) {
			for (int y = 0; y < resolution[Y]; y++) {
				axisLayerBuffer[X]->rawData[y + z* resolution[Y]].data = editedobj->rawData[numOfAxisLayer[X] + y*resolution[X] + z * resolution[Y] * resolution[X]].data;
			}
		}
		if ((axisLayerBuffer[X]->vbo)) {
			deleteVBO(&(axisLayerBuffer[X]->vbo));
		}
		vbo = createEditVoxelVBO(axisLayerBuffer[X]);
		setColorVBO(axisLayerBufferColor[X][R],axisLayerBufferColor[X][G],axisLayerBufferColor[X][B],axisLayerBufferColor[X][A], vbo);
		bindData(vbo);
	}
	else if (viewmode == YAXISVIEW) {
		for (int z = 0; z < resolution[Z]; z++) {
			for (int x = 0; x < resolution[X]; x++) {
				axisLayerBuffer[Y]->rawData[x + z* resolution[X]].data = editedobj->rawData[x + numOfAxisLayer[Y]*resolution[X] + z * resolution[Y] * resolution[X]].data;
			}
		}
		if ((axisLayerBuffer[Y]->vbo)) {
			deleteVBO(&(axisLayerBuffer[Y]->vbo));
		}
		vbo = createEditVoxelVBO(axisLayerBuffer[Y]);
		setColorVBO(axisLayerBufferColor[Y][R], axisLayerBufferColor[Y][G], axisLayerBufferColor[Y][B], axisLayerBufferColor[Y][A], vbo);
		bindData(vbo);
	}
	else if (viewmode == ZAXISVIEW) {
		for (int y = 0; y < resolution[Y]; y++) {
			for (int x = 0; x < resolution[X]; x++) {
				axisLayerBuffer[Z]->rawData[x + y* resolution[X]].data = editedobj->rawData[x + y*resolution[X] + numOfAxisLayer[Z] * resolution[Y] * resolution[X]].data;
			}
		}
		if ((axisLayerBuffer[Z]->vbo)) {
			deleteVBO(&(axisLayerBuffer[Z]->vbo));
		}
		vbo = createEditVoxelVBO(axisLayerBuffer[Z]);
		setColorVBO(axisLayerBufferColor[Z][R], axisLayerBufferColor[Z][G], axisLayerBufferColor[Z][B], axisLayerBufferColor[Z][A], vbo);
		bindData(vbo);
	}*/
}

void VoxelEditorOGLWidget::calculateEditDataBBOX(int* minRange, int* maxRange)
{
	//
	bool minFlag[3] = { false,false,false }, maxFlag[3] = { false,false,false };
	//record BBox Range
	int max[3] , min[3];
	int index;

	//x range
	//min path 
	for (int x = 0; x < EDITSPACERESOLUTION; x++) {
		for (int y = 0; y < EDITSPACERESOLUTION; y++) {
			for (int z = 0; z < EDITSPACERESOLUTION; z++) {
				index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				if (editedobj->rawData[index].data != VOX_EMPTY) {
					min[0] = x;
					minFlag[0] = true;
				}
			}
			if (minFlag[0]) break;
		}
		if (minFlag[0]) break;
	}
	//max path 
	for (int x = EDITSPACERESOLUTION - 1; x >= 0; x--) {
		for (int y = 0; y < EDITSPACERESOLUTION; y++) {
			for (int z = 0; z < EDITSPACERESOLUTION; z++) {
				index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				if (editedobj->rawData[index].data != VOX_EMPTY) {
					max[0] = x;
					maxFlag[0] = true;
				}
			}
			if (maxFlag[0]) break;
		}
		if (maxFlag[0]) break;
	}

	//y range
	//min path 
	for (int y = 0; y < EDITSPACERESOLUTION; y++) {
		for (int x = min[0]; x <= max[0]; x++) {
			for (int z = 0; z < EDITSPACERESOLUTION; z++) {
				index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				if (editedobj->rawData[index].data != VOX_EMPTY) {
					min[1] = y;
					minFlag[1] = true;
				}
			}
			if (minFlag[1]) break;
		}
		if (minFlag[1]) break;
	}
	//max path 
	for (int y = EDITSPACERESOLUTION - 1; y >= 0; y--) {
		for (int x = min[0]; x <= max[0]; x++) {
			for (int z = 0; z < EDITSPACERESOLUTION; z++) {
				index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				if (editedobj->rawData[index].data != VOX_EMPTY) {
					max[1] = y;
					maxFlag[1] = true;
				}
			}
			if (maxFlag[1]) break;
		}
		if (maxFlag[1]) break;
	}

	//z range
	//min path 
	for (int z = 0; z < EDITSPACERESOLUTION; z++) {
		for (int x = min[0]; x <= max[0]; x++) {
			for (int y = min[1]; y <= max[1]; y++) {
				index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				if (editedobj->rawData[index].data != VOX_EMPTY) {
					min[2] = z;
					minFlag[2] = true;
				}
			}
			if (minFlag[2]) break;
		}
		if (minFlag[2]) break;
	}
	//max path 
	for (int z = EDITSPACERESOLUTION - 1; z >= 0; z--) {
		for (int x = min[0]; x <= max[0]; x++) {
			for (int y = min[1]; y <= max[1]; y++) {
				index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				if (editedobj->rawData[index].data != VOX_EMPTY) {
					max[2] = z;
					maxFlag[2] = true;
				}
			}
			if (maxFlag[2]) break;
		}
		if (maxFlag[2]) break;
	}

	for (int i = 0; i < 3; i++) {
		minRange[i] = min[i];
		maxRange[i] = max[i];
	}
/*
	if (minFlag[0] && minFlag[1] && minFlag[2]) {
		printf("Find min value.\n");
		printf("Min X:%d, Y:%d, Z:%d. \n", minRange[0], minRange[1], minRange[2]);
	}
	if (maxFlag[0] && maxFlag[1] && maxFlag[2]) {
		printf("Find max value.\n");
		printf("Max X:%d, Y:%d, Z:%d. \n", maxRange[0], maxRange[1], maxRange[2]);
	}*/
	return;
}

void VoxelEditorOGLWidget::setFreeFormBuffer(float *mousePnt)
{
	VBO* vbo;

	if (mousePnt[X] == 0) {			//   xAxis 
		int x = mousePnt[Z] + EDITSPACERESOLUTION / 2;
		int y = mousePnt[Y] + EDITSPACERESOLUTION / 2;

		//printf("x:%d, y:%d\n", x, y);
		if (ff_Method == FF_CIRCLE) {
			for (int i = x - ff_Radius; i < x + ff_Radius; i++) {
				for (int j = y - ff_Radius; j < y + ff_Radius; j++) {
					if (i>=0 && i<EDITSPACERESOLUTION && j>=0 && j<EDITSPACERESOLUTION) {
						if ((i - x) *(i - x) + (j - y)*(j - y) < ff_Radius*ff_Radius) {
							int index = j + i * EDITSPACERESOLUTION;
							axisFreeformBuffer[X]->rawData[index].data = VOX_SOLID;
						}
					}
				}
			}
		}
		else if (ff_Method == FF_SQUARE) {
			for (int i = x - ff_Radius / 2; i < x + ff_Radius / 2; i++) {
				for (int j = y - ff_Radius / 2; j < y + ff_Radius / 2; j++) {
					if (i >= 0 && i<EDITSPACERESOLUTION && j >= 0 && j<EDITSPACERESOLUTION) {
						int index = j + i * EDITSPACERESOLUTION;
						axisFreeformBuffer[X]->rawData[index].data = VOX_SOLID;
					}
				}
			}
		}
		if (axisFreeformBuffer[X] != NULL) {
			deleteVBO(&axisFreeformBuffer[X]->vbo);
		}
		vbo = createEditVoxelVBO(axisFreeformBuffer[X]);
		setColorVBO(1.0, 0.8, 0.8, 0.2, vbo);
		bindData(vbo);
	}
	else if (mousePnt[Y] == 0) {	//	 yAxis
		int x = mousePnt[X] + EDITSPACERESOLUTION / 2;
		int y = mousePnt[Z] + EDITSPACERESOLUTION / 2;

		//printf("x:%d, y:%d\n", x, y);
		if (ff_Method == FF_CIRCLE) {
			for (int i = x - ff_Radius; i < x + ff_Radius; i++) {
				for (int j = y - ff_Radius; j < y + ff_Radius; j++) {
					if (i >= 0 && i<EDITSPACERESOLUTION && j >= 0 && j<EDITSPACERESOLUTION) {
						if ((i - x) *(i - x) + (j - y)*(j - y) < ff_Radius*ff_Radius) {
							int index = i + j * EDITSPACERESOLUTION;
							axisFreeformBuffer[Y]->rawData[index].data = VOX_SOLID;
						}
					}
				}
			}
		}
		else if (ff_Method == FF_SQUARE) {
			for (int i = x - ff_Radius / 2; i < x + ff_Radius / 2; i++) {
				for (int j = y - ff_Radius / 2; j < y + ff_Radius / 2; j++) {
					if (i >= 0 && i<EDITSPACERESOLUTION && j >= 0 && j<EDITSPACERESOLUTION) {
						int index = i + j * EDITSPACERESOLUTION;
						axisFreeformBuffer[Y]->rawData[index].data = VOX_SOLID;
					}
				}
			}
		}
		if (axisFreeformBuffer[Y] != NULL) {
			deleteVBO(&axisFreeformBuffer[Y]->vbo);
		}
		vbo = createEditVoxelVBO(axisFreeformBuffer[Y]);
		setColorVBO(1.0, 0.8, 0.8, 0.2, vbo);
		bindData(vbo);
	}
	else if (mousePnt[Z] == 0) {	//	 zAxis
		int x = mousePnt[X] + EDITSPACERESOLUTION / 2;
		int y = mousePnt[Y] + EDITSPACERESOLUTION / 2;

		//printf("x:%d, y:%d\n", x, y);
		if (ff_Method == FF_CIRCLE) {
			for (int i = x - ff_Radius; i < x + ff_Radius; i++) {
				for (int j = y - ff_Radius; j < y + ff_Radius; j++) {
					if (i >= 0 && i<EDITSPACERESOLUTION && j >= 0 && j<EDITSPACERESOLUTION) {
						if ((i - x) *(i - x) + (j - y)*(j - y) < ff_Radius*ff_Radius) {
							int index = i + j * EDITSPACERESOLUTION;
							axisFreeformBuffer[Z]->rawData[index].data = VOX_SOLID;
						}
					}
				}
			}
		}
		else if (ff_Method == FF_SQUARE) {
			for (int i = x - ff_Radius / 2; i < x + ff_Radius / 2; i++) {
				for (int j = y - ff_Radius / 2; j < y + ff_Radius / 2; j++) {
					if (i >= 0 && i<EDITSPACERESOLUTION && j >= 0 && j<EDITSPACERESOLUTION) {
						int index = i + j * EDITSPACERESOLUTION;
						axisFreeformBuffer[Z]->rawData[index].data = VOX_SOLID;
					}
				}
			}
		}
		if (axisFreeformBuffer[Z] != NULL) {
			deleteVBO(&axisFreeformBuffer[Z]->vbo);
		}
		vbo = createEditVoxelVBO(axisFreeformBuffer[Z]);
		setColorVBO(1.0, 0.8, 0.8, 0.2, vbo);
		bindData(vbo);
	}

}

void VoxelEditorOGLWidget::changeEditData(int view,int set)
{

	VBO *vbo;
	if (view == XAXISVIEW) {
		for (int i = 0; i < EDITSPACERESOLUTION; i++) {
			for (int j = 0; j < EDITSPACERESOLUTION; j++) {
				int dataIndex = numOfAxisLayer[X] + i *  EDITSPACERESOLUTION + j * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				int bufferIndex = i + j * EDITSPACERESOLUTION;
				if (axisFreeformBuffer[X]->rawData[bufferIndex].data) {
					editedobj->rawData[dataIndex].data = set == 1 ? VOX_SOLID : VOX_EMPTY;
				}
			}
		}
	}
	else if (YAXISVIEW) {
		for (int i = 0; i < EDITSPACERESOLUTION; i++) {
			for (int j = 0; j < EDITSPACERESOLUTION; j++) {
				int dataIndex = i + numOfAxisLayer[Y] *  EDITSPACERESOLUTION + j * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				int bufferIndex = i + j * EDITSPACERESOLUTION;
				if (axisFreeformBuffer[Y]->rawData[bufferIndex].data) {
					editedobj->rawData[dataIndex].data = set == 1 ? VOX_SOLID : VOX_EMPTY;
				}
			}
		}
	}
	else if (ZAXISVIEW) {
		for (int i = 0; i < EDITSPACERESOLUTION; i++) {
			for (int j = 0; j < EDITSPACERESOLUTION; j++) {
				int dataIndex = i + j *  EDITSPACERESOLUTION + numOfAxisLayer[Z] * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				int bufferIndex = i + j * EDITSPACERESOLUTION;
				if (axisFreeformBuffer[Z]->rawData[bufferIndex].data) {
					editedobj->rawData[dataIndex].data = set == 1 ? VOX_SOLID : VOX_EMPTY;
				}
			}
		}
	}

	if (editedobj->vbo) {
		deleteVBO(&editedobj->vbo);
	}
	vbo = createEditVoxelVBO(editedobj);
	setColorVBO(1.0, 1.0, 1.0, 1.0, vbo);
	bindData(vbo);
	
	setSliceData(X);
	setSliceData(Y);
	setSliceData(Z);
}

void VoxelEditorOGLWidget::changeAxisFreeFormBufferData(int viewmode, bool setmode)
{
	VBO *vbo;

	if (viewmode == FOURWINDOW || viewmode == THREEDVIEW) { return; }

	for (int i = 0; i < EDITSPACERESOLUTION; i++) {
		for (int j = 0; j < EDITSPACERESOLUTION; j++) {
			int index = i + j * EDITSPACERESOLUTION;
			if (axisFreeformBuffer[viewmode]->rawData[index].data != NULL) {
				axisLayerBuffer[viewmode]->rawData[index].data = setmode ? VOX_SOLID : VOX_EMPTY;
				axisLayerBuffer[viewmode]->rawData[index].ID = setmode ? 1 : 0;
			}
		}
	}

	axisFreeformBufferChanged[viewmode] = true;


	deleteVBO(&axisLayerBuffer[viewmode]->vbo);
	vbo = createEditVoxelVBO(axisLayerBuffer[viewmode]);
	setColorVBO(axisLayerBufferColor[viewmode][R], axisLayerBufferColor[viewmode][G], axisLayerBufferColor[viewmode][B], axisLayerBufferColor[viewmode][A], vbo);
	bindData(vbo);

}

bool VoxelEditorOGLWidget::restoreToEditData(int viewmode)
{
	int count = 0;

	if (viewmode == THREEDVIEW || viewmode == FOURWINDOW) { return false; }
	if (axisFreeformBufferChanged[viewmode] == false) { return false; }

	VBO* vbo;

	// restore to edit data
	for (int j = 0; j < EDITSPACERESOLUTION; j++) {
		for (int i = 0; i < EDITSPACERESOLUTION; i++) {
			int editindex;
			int bufferindex = i + j *EDITSPACERESOLUTION;
			if (viewmode == XAXISVIEW) {
				editindex = numOfAxisLayer[X] + i * EDITSPACERESOLUTION + j * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
			}
			else  if (viewmode == YAXISVIEW) {
				editindex = i + numOfAxisLayer[Y] * EDITSPACERESOLUTION + j * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
			}
			else  if (viewmode == ZAXISVIEW) {
				editindex = i + j * EDITSPACERESOLUTION + numOfAxisLayer[Z] * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
			}
			editedobj->rawData[editindex].data = axisLayerBuffer[viewmode]->rawData[bufferindex].data;
			editedobj->rawData[editindex].ID = axisLayerBuffer[viewmode]->rawData[bufferindex].ID;
		}
	}

	editDataChanged = true;
	axisFreeformBufferChanged[viewmode] = false;

	return true;
}

void VoxelEditorOGLWidget::clearFreeFormBuffer(int viewmode)
{
	// clear free form buffer
	switch (viewmode) {
	case XAXISVIEW:
		if (axisFreeformBuffer[X] != NULL) {
			for (int i = 0; i < EDITSPACERESOLUTION * EDITSPACERESOLUTION; i++) {
				axisFreeformBuffer[X]->rawData[i].data = VOX_EMPTY;
			}
			deleteVBO(&axisFreeformBuffer[X]->vbo);
		}
		break;
	case YAXISVIEW:
		if (axisFreeformBuffer[Y] != NULL) {
			for (int i = 0; i < EDITSPACERESOLUTION * EDITSPACERESOLUTION; i++) {
				axisFreeformBuffer[Y]->rawData[i].data = VOX_EMPTY;
			}
			deleteVBO(&axisFreeformBuffer[Y]->vbo);
		}
		break;
	case ZAXISVIEW:
		if (axisFreeformBuffer[Z] != NULL) {
			for (int i = 0; i < EDITSPACERESOLUTION * EDITSPACERESOLUTION; i++) {
				axisFreeformBuffer[Z]->rawData[i].data = VOX_EMPTY;
			}
			deleteVBO(&axisFreeformBuffer[Z]->vbo);
		}
		break;
	default:
		break;
	}
}

void VoxelEditorOGLWidget::changeOPMode(int mode)
{
	char modeStr[4][10] = { { "Selected" },{ "Control" },{ "Free Form" },{ "Smoothing" } };

	// save the current mode data and close some relative button and object
	if (opMode != mode) {
		switch (opMode) {
		case CONTROL:
			// not need to save anythings
			break;
		case SELECT:
			// cancel all selected voxel


			break;
		case FREEFORM:
			// 1. close save button 
			penButton->setVisible(false);
			eraserButton->setVisible(false);
			resetButton->setVisible(false);

			// 2.


			break;
		case SMOOTHING:
			//

			break;
		}

		opMode = mode;
	}

	// change mode 
	printf("Mode:%s\n", modeStr[mode]);
}

void VoxelEditorOGLWidget::savebackData(int * minRange, int * maxRange)
{
	int new_X_Size, new_Y_Size, new_Z_Size;

	this->vItem->vdata->resolution[X] = new_X_Size = maxRange[X] - minRange[X] + 1;
	this->vItem->vdata->resolution[Y] = new_Y_Size = maxRange[Y] - minRange[Y] + 1;
	this->vItem->vdata->resolution[Z] = new_Z_Size = maxRange[Z] - minRange[Z] + 1;

	this->vItem->vdata->rawData = (Voxel *)realloc(this->vItem->vdata->rawData, new_X_Size * new_Y_Size * new_Z_Size * sizeof(Voxel));

	for (int z = 0; z < new_Z_Size;z++) {
		for (int y = 0; y < new_Y_Size; y++) {
			for (int x = 0; x < new_X_Size; x++) {
				int newvdataIndex = x + y * new_X_Size + z * new_X_Size * new_Y_Size;
				int editdataIndex = (x + minRange[X]) + (y + minRange[Y]) * EDITSPACERESOLUTION + (z + minRange[Z]) * EDITSPACERESOLUTION * EDITSPACERESOLUTION;

				this->vItem->vdata->rawData[newvdataIndex].data = editedobj->rawData[editdataIndex].data;
			}
		}
	}

}

// Slots

void VoxelEditorOGLWidget::openLightWidget(void)
{
	if (lightWidget->isVisible()) {
		lightWidget->close();
	}
	else {
		lightWidget->show();
	}
}

void VoxelEditorOGLWidget::openControlWidget(void)
{
	changeOPMode(CONTROL);

}

void VoxelEditorOGLWidget::openSelectionWidget(void)
{
	changeOPMode(SELECT);

}

void VoxelEditorOGLWidget::openFreeFormWidget(void)
{
	freeformwidget->show();
}

void VoxelEditorOGLWidget::openSmoothingWidget(void)
{
	smoothingwidget->show();
}

void VoxelEditorOGLWidget::recalculateConnectingComponent(void)
{
	if (editedobj != NULL) {
/*
		for (int z = 0; z < EDITSPACERESOLUTION; z++) {
			for (int y = 0; y < EDITSPACERESOLUTION; y++) {
				for (int x = 0; x < EDITSPACERESOLUTION; x++) {
					int index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
					if (editedobj->rawData[index].data == VOX_EMPTY) {
						editedobj->rawData[index].ID = 0;
					}
					else {
						editedobj->rawData[index].ID = 1;
					}
				}
			}
		}*/

		calculateConnetComponent();
	}

}

void VoxelEditorOGLWidget::acceptVoxelItem(QTreeWidgetItem* item, int i)
{
	
	if (((VoxelItem*)((VoxelDataListItem*)item)->it->link.flink) != ((VoxelDataListItem*)item)->it) {
		this->vItem = ((VoxelDataListItem*)item)->it;
		if (vItem != NULL) {
			// init check flag
			dataChangeCheck = false;

			//clean buffer
			cleanEditBuffer();

			// init edit buffer or re init
			initEditData();
			copyData();

			// calculate connecting component
			//calculateConnetComponent();

			if (editedobj != NULL) {
				mallocSlice();					// alloc space for slice
				if (windowMode == FOURWINDOW) {	// give data according tje window mode
					setSliceData(XAXISVIEW);	
					setSliceData(XAXISVIEW);
					setSliceData(XAXISVIEW);
				}
				else if (windowMode != THREEDVIEW) {
					setSliceData(windowMode);
				}
			}
		}
	}
	else {
		this->vItem = NULL;
		editedobj = NULL;
	}
	update();
}

void VoxelEditorOGLWidget::xAxisSliderChangerd(int value)
{
	xAxisLayer_spinbox->setValue(value);
	//printf("slider move\n");
}

void VoxelEditorOGLWidget::yAxisSliderChangerd(int value)
{
	yAxisLayer_spinbox->setValue(value);
	//printf("slider move\n");
}

void VoxelEditorOGLWidget::zAxisSliderChangerd(int value)
{
	zAxisLayer_spinbox->setValue(value);
	//printf("slider move\n");
}

void VoxelEditorOGLWidget::xAxisSpinboxChangerd(int value)
{
	if (editedobj != NULL) {
		if (axisFreeformBufferChanged[X]) {
			//printf("Store X layer : %d\n", numOfAxisLayer[X]);
			restoreToEditData(X);
		}
		numOfAxisLayer[X] = value;
		xAxisLayer_slider->setValue(value);
		//printf("New X layer : %d\n", value);
		setSliceData(X);
	}
	update();
}

void VoxelEditorOGLWidget::yAxisSpinboxChangerd(int value)
{
	if (editedobj != NULL) {
		if (axisFreeformBufferChanged[Y]) {
			//printf("Store Y layer : %d\n", numOfAxisLayer[Y]);
			restoreToEditData(Y);
		}
		numOfAxisLayer[Y] = value;
		yAxisLayer_slider->setValue(value);
		//printf("New Y layer : %d\n", value);
		setSliceData(Y);
	}
	update();
}

void VoxelEditorOGLWidget::zAxisSpinboxChangerd(int value)
{
	if (editedobj != NULL) {
		if (axisFreeformBufferChanged[Z]) {
			//printf("Store Z layer : %d\n", numOfAxisLayer[Z]);
			restoreToEditData(Z);
		}
		numOfAxisLayer[Z] = value;
		zAxisLayer_slider->setValue(value);
		//printf("New Z layer : %d\n", value);
		setSliceData(Z);
	}
	update();
}

void VoxelEditorOGLWidget::changeNumOfAxisLayer(int value)
{




}

void VoxelEditorOGLWidget::setFreeFormParameter(int method, int radius)
{
	VBO* vbo;
	float mat[16];

	ff_Method = method;


	// create free form vbo

	axisFreeformBuffer[X] = newEditVoxelData();
	axisFreeformBuffer[X]->voxelSize[X] = axisFreeformBuffer[X]->voxelSize[Y] = axisFreeformBuffer[X]->voxelSize[Z] = 1;
	axisFreeformBuffer[X]->resolution[X] = 1;
	axisFreeformBuffer[X]->resolution[Y] = axisFreeformBuffer[X]->resolution[Z] = EDITSPACERESOLUTION;
	axisFreeformBuffer[X]->bbox->min[X] = 0;
	axisFreeformBuffer[X]->bbox->min[Y] = axisFreeformBuffer[X]->bbox->min[Z] = -EDITSPACERESOLUTION / 2;
	axisFreeformBuffer[X]->rawData = (EditVoxel*)calloc(EDITSPACERESOLUTION * EDITSPACERESOLUTION * 1, sizeof(EditVoxel));
		 
	axisFreeformBuffer[Y] = newEditVoxelData();
	axisFreeformBuffer[Y]->voxelSize[X] = axisFreeformBuffer[Y]->voxelSize[Y] = axisFreeformBuffer[Y]->voxelSize[Z] = 1;
	axisFreeformBuffer[Y]->resolution[Y] = 1;
	axisFreeformBuffer[Y]->resolution[X] = axisFreeformBuffer[Y]->resolution[Z] = EDITSPACERESOLUTION;
	axisFreeformBuffer[Y]->bbox->min[Y] = 0;
	axisFreeformBuffer[Y]->bbox->min[X] = axisFreeformBuffer[Y]->bbox->min[Z] = -EDITSPACERESOLUTION / 2;
	axisFreeformBuffer[Y]->rawData = (EditVoxel*)calloc(EDITSPACERESOLUTION * EDITSPACERESOLUTION * 1, sizeof(EditVoxel));
	
	axisFreeformBuffer[Z] = newEditVoxelData();
	axisFreeformBuffer[Z]->voxelSize[X] = axisFreeformBuffer[Z]->voxelSize[Y] = axisFreeformBuffer[Z]->voxelSize[Z] = 1;
	axisFreeformBuffer[Z]->resolution[Z] = 1;
	axisFreeformBuffer[Z]->resolution[Y] = axisFreeformBuffer[Z]->resolution[X] = EDITSPACERESOLUTION;
	axisFreeformBuffer[Z]->bbox->min[Z] = 0;
	axisFreeformBuffer[Z]->bbox->min[Y] = axisFreeformBuffer[Z]->bbox->min[X] = -EDITSPACERESOLUTION / 2;
	axisFreeformBuffer[Z]->rawData = (EditVoxel*)calloc(EDITSPACERESOLUTION * EDITSPACERESOLUTION * 1, sizeof(EditVoxel));

	if (ff_Method == FF_CIRCLE) {
		ff_Radius = radius;

		axisFreeFromTool[X] = newCircle(radius, 50, 10);
		setMaterialAmbient(axisFreeFromTool[X]->material, .19225, .19225, .19225, 1.0);
		setMaterialDiffuse(axisFreeFromTool[X]->material, .50754, .50754, .50754, 1.0);
		setMaterialSpecular(axisFreeFromTool[X]->material, .508273, .508273, .508273, 1.0);
		setMaterialShininess(axisFreeFromTool[X]->material, .4 * 128.0);
		rotateY(90.0, mat);
		translate(0, -10000, -10000, axisFreeFromTool[X]->modelMat);
		multMat4fv(mat, axisFreeFromTool[X]->modelMat, axisFreeFromTool[X]->modelMat);
		bindData(axisFreeFromTool[X]);

		axisFreeFromTool[Y] = newCircle(radius, 50, 10);
		setMaterialAmbient(axisFreeFromTool[Y]->material, .19225, .19225, .19225, 1.0);
		setMaterialDiffuse(axisFreeFromTool[Y]->material, .50754, .50754, .50754, 1.0);
		setMaterialSpecular(axisFreeFromTool[Y]->material, .508273, .508273, .508273, 1.0);
		setMaterialShininess(axisFreeFromTool[Y]->material, .4 * 128.0);
		rotateZ(90.0, mat);
		translate(-10000, 0, -10000, axisFreeFromTool[Y]->modelMat);
		multMat4fv(mat, axisFreeFromTool[Y]->modelMat, axisFreeFromTool[Y]->modelMat);
		bindData(axisFreeFromTool[Y]);

		axisFreeFromTool[Z] = newCircle(radius, 50, 10);
		setMaterialAmbient(axisFreeFromTool[Z]->material, .19225, .19225, .19225, 1.0);
		setMaterialDiffuse(axisFreeFromTool[Z]->material, .50754, .50754, .50754, 1.0);
		setMaterialSpecular(axisFreeFromTool[Z]->material, .508273, .508273, .508273, 1.0);
		setMaterialShininess(axisFreeFromTool[Z]->material, .4 * 128.0);
		translate(-10000, -10000, 0, axisFreeFromTool[Z]->modelMat);
		multMat4fv(mat, axisFreeFromTool[Z]->modelMat, axisFreeFromTool[Z]->modelMat);
		bindData(axisFreeFromTool[Z]);
	}
	else if (ff_Method == FF_SQUARE) {
		ff_Radius = radius;

		axisFreeFromTool[X] = newPlane(radius, radius, 1.0);
		setMaterialAmbient(axisFreeFromTool[X]->material, .19225, .19225, .19225, 1.0);
		setMaterialDiffuse(axisFreeFromTool[X]->material, .50754, .50754, .50754, 1.0);
		setMaterialSpecular(axisFreeFromTool[X]->material, .508273, .508273, .508273, 1.0);
		setMaterialShininess(axisFreeFromTool[X]->material, .4 * 128.0);
		rotateY(90.0, mat);
		translate(0, -10000, -10000, axisFreeFromTool[X]->modelMat);
		multMat4fv(mat, axisFreeFromTool[X]->modelMat, axisFreeFromTool[X]->modelMat);
		bindData(axisFreeFromTool[X]);

		axisFreeFromTool[Y] = newPlane(radius, radius, 1.0);
		setMaterialAmbient(axisFreeFromTool[Y]->material, .19225, .19225, .19225, 1.0);
		setMaterialDiffuse(axisFreeFromTool[Y]->material, .50754, .50754, .50754, 1.0);
		setMaterialSpecular(axisFreeFromTool[Y]->material, .508273, .508273, .508273, 1.0);
		setMaterialShininess(axisFreeFromTool[Y]->material, .4 * 128.0);
		rotateZ(90.0, mat);
		translate(-10000, 0, -10000, axisFreeFromTool[Y]->modelMat);
		multMat4fv(mat, axisFreeFromTool[Y]->modelMat, axisFreeFromTool[Y]->modelMat);
		bindData(axisFreeFromTool[Y]);

		axisFreeFromTool[Z] = newPlane(radius, radius, 1.0);
		setMaterialAmbient(axisFreeFromTool[Z]->material, .19225, .19225, .19225, 1.0);
		setMaterialDiffuse(axisFreeFromTool[Z]->material, .50754, .50754, .50754, 1.0);
		setMaterialSpecular(axisFreeFromTool[Z]->material, .508273, .508273, .508273, 1.0);
		setMaterialShininess(axisFreeFromTool[Z]->material, .4 * 128.0);
		translate(-10000, -10000, 0, axisFreeFromTool[Z]->modelMat);
		multMat4fv(mat, axisFreeFromTool[Z]->modelMat, axisFreeFromTool[Z]->modelMat);
		bindData(axisFreeFromTool[Z]);
	}

	penButton->setVisible(true);
	eraserButton->setVisible(true);
	resetButton->setVisible(true);

	changeOPMode(FREEFORM);

	return;
}

void VoxelEditorOGLWidget::penButtonPress(void)
{
	penButton->setIcon(QIcon(("ApplicationImages/pen_selected.ico")));
	penButton->setIconSize(QSize(40, 40));
}

void VoxelEditorOGLWidget::penButtonRelease(void)
{
	penButton->setIcon(QIcon(("ApplicationImages/pen.ico")));
	penButton->setIconSize(QSize(60, 60));
	// pen
	//  make set 
	if (editedobj != NULL) {
		saveButton->setVisible(true);
		changeAxisFreeFormBufferData(windowMode, true);
		//changeEditData(windowMode, 1);
	}
	clearFreeFormBuffer(windowMode);

}

void VoxelEditorOGLWidget::eraserButtonPress(void)
{
	eraserButton->setIcon(QIcon(("ApplicationImages/eraser_selected.ico")));
	eraserButton->setIconSize(QSize(40, 40));
}

void VoxelEditorOGLWidget::eraserButtonRelease(void)
{
	eraserButton->setIcon(QIcon(("ApplicationImages/eraser.ico")));
	eraserButton->setIconSize(QSize(50, 50));
	// eraser
	//  make set 
	if (editedobj != NULL) {
		saveButton->setVisible(true);
		changeAxisFreeFormBufferData(windowMode, false);
		//changeEditData(windowMode, 0);
	}
	clearFreeFormBuffer(windowMode);
}

void VoxelEditorOGLWidget::resetButtonPress(void)
{
	resetButton->setIconSize(QSize(40, 40));
}

void VoxelEditorOGLWidget::resetButtonRelease(void)
{
	resetButton->setIconSize(QSize(50, 50));
	// reset
	clearFreeFormBuffer(windowMode);
}

void VoxelEditorOGLWidget::erosion(int neighborCount)
{
	if (editedobj == NULL) return;

	int six_neighbor_index[6][3] = {
		{ 0,1,0 },
		{ 0,-1,0 },
		{ 1,0,0 },
		{ -1,0,0 },
		{ 0,0,1 },
		{ 0,0,-1 }
	};

	int twenty_six_neighbor_index[26][3] = {
		// 9 roof neighbors
		{ 1,1,1 },
		{ 1,1,0 },
		{ 1,1,-1 },
		{ 0,1,1 },
		{ 0,1,0 },
		{ 0,1,-1 },
		{ -1,1,1 },
		{ -1,1,0 },
		{ -1,1,-1 },

		// 8 same floor neighbors
		{ 1,0,1 },
		{ 1,0,0 },
		{ 1,0,-1 },
		{ 0,0,1 },
		{ 0,0,-1 },
		{ -1,0,1 },
		{ -1,0,0 },
		{ -1,0,-1 },

		// 9 downstairs neighbors
		{ 1,-1,1 },
		{ 1,-1,0 },
		{ 1,-1,-1 },
		{ 0,-1,1 },
		{ 0,-1,0 },
		{ 0,-1,-1 },
		{ -1,-1,1 },
		{ -1,-1,0 },
		{ -1,-1,-1 }
	};


	unsigned char* tempArray = (unsigned char*)calloc(sizeof(unsigned char), EDITSPACERESOLUTION * EDITSPACERESOLUTION * EDITSPACERESOLUTION);
	int target_index, neighbor_index, neighbor_x, neighbor_y, neighbor_z;

	// ero to temp array
	for (int z = 0; z < EDITSPACERESOLUTION; z++) {
		for (int y = 0; y < EDITSPACERESOLUTION; y++) {
			for (int x = 0; x < EDITSPACERESOLUTION; x++) {
				target_index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				if (editedobj->rawData[target_index].data == VOX_EMPTY)continue;
				bool surface_flag = false;

				for (int i = 0; i < neighborCount; i++) {
					// check neighbor is not out of range
					neighbor_x = (neighborCount == SIX_NEIGHBOR) ? x + six_neighbor_index[i][X] : x + twenty_six_neighbor_index[i][X];
					neighbor_y = (neighborCount == SIX_NEIGHBOR) ? y + six_neighbor_index[i][Z] : y + twenty_six_neighbor_index[i][Y];
					neighbor_z = (neighborCount == SIX_NEIGHBOR) ? z + six_neighbor_index[i][Z] : z + twenty_six_neighbor_index[i][Z];
					if (neighbor_x < 0 || neighbor_y < 0 || neighbor_z < 0 ||
						neighbor_x >= EDITSPACERESOLUTION || neighbor_y >= EDITSPACERESOLUTION || neighbor_z >= EDITSPACERESOLUTION) {
						continue;
					}

					// judge the target voxel is not surface
					neighbor_index = neighbor_x + neighbor_y * EDITSPACERESOLUTION + neighbor_z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
					if (editedobj->rawData[neighbor_index].data == VOX_EMPTY) {
						surface_flag = true;
						break;
					}
				}

				if(surface_flag){
					tempArray[target_index] = VOX_EMPTY;
				}
				else {
					tempArray[target_index] = editedobj->rawData[target_index].data;
				}
			}
		}
	}

	// cover back to edit data
	for (int z = 0; z < EDITSPACERESOLUTION; z++) {
		for (int y = 0; y < EDITSPACERESOLUTION; y++) {
			for (int x = 0; x < EDITSPACERESOLUTION; x++) {
				target_index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				editedobj->rawData[target_index].data = tempArray[target_index];
				if (editedobj->rawData[target_index].data == VOX_EMPTY) {
					editedobj->rawData[target_index].ID = 0;
				}
				else {
					editedobj->rawData[target_index].ID = 1;
				}
			}
		}
	}
	free(tempArray);

	VBO* vbo;

	deleteVBO(&editedobj->vbo);
	vbo = createEditVoxelVBO(editedobj);
	setColorVBO(1.0, 1.0, 1.0, 1.0, vbo);
	bindData(vbo);

	saveButton->show();

	return;
}

void VoxelEditorOGLWidget::dilation(int neighborCount)
{
	if (editedobj == NULL) return;

	int six_neighbor_index[6][3] = {
		{ 0,1,0 },
		{ 0,-1,0 },
		{ 1,0,0 },
		{ -1,0,0 },
		{ 0,0,1 },
		{ 0,0,-1 }
	};

	int twenty_six_neighbor_index[26][3] = {
		// 9 roof neighbors
		{ 1,1,1 },
		{ 1,1,0 },
		{ 1,1,-1 },
		{ 0,1,1 },
		{ 0,1,0 },
		{ 0,1,-1 },
		{ -1,1,1 },
		{ -1,1,0 },
		{ -1,1,-1 },

		// 8 same floor neighbors
		{ 1,0,1 },
		{ 1,0,0 },
		{ 1,0,-1 },
		{ 0,0,1 },
		{ 0,0,-1 },
		{ -1,0,1 },
		{ -1,0,0 },
		{ -1,0,-1 },

		// 9 downstairs neighbors
		{ 1,-1,1 },
		{ 1,-1,0 },
		{ 1,-1,-1 },
		{ 0,-1,1 },
		{ 0,-1,0 },
		{ 0,-1,-1 },
		{ -1,-1,1 },
		{ -1,-1,0 },
		{ -1,-1,-1 }
	};


	unsigned char* tempArray = (unsigned char*)calloc(sizeof(unsigned char), EDITSPACERESOLUTION * EDITSPACERESOLUTION * EDITSPACERESOLUTION);

	int target_index, neighbor_index, neighbor_x, neighbor_y, neighbor_z;


	// diolate to temp array
	for (int z = 0; z < EDITSPACERESOLUTION; z++) {
		for (int y = 0; y < EDITSPACERESOLUTION; y++) {
			for (int x = 0; x < EDITSPACERESOLUTION; x++) {
				target_index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				if (editedobj->rawData[target_index].data != VOX_EMPTY)continue;
				bool surface_flag = false;

				for (int i = 0; i < neighborCount; i++) {
					// check neighbor is not out of range
					neighbor_x = (neighborCount == SIX_NEIGHBOR) ? x + six_neighbor_index[i][X] : x + twenty_six_neighbor_index[i][X];
					neighbor_y = (neighborCount == SIX_NEIGHBOR) ? y + six_neighbor_index[i][Z] : y + twenty_six_neighbor_index[i][Y];
					neighbor_z = (neighborCount == SIX_NEIGHBOR) ? z + six_neighbor_index[i][Z] : z + twenty_six_neighbor_index[i][Z];
					if (neighbor_x < 0 || neighbor_y < 0 || neighbor_z < 0 ||
						neighbor_x >= EDITSPACERESOLUTION || neighbor_y >= EDITSPACERESOLUTION || neighbor_z >= EDITSPACERESOLUTION) {
						continue;
					}

					// judge the target voxel is not surface
					neighbor_index = neighbor_x + neighbor_y * EDITSPACERESOLUTION + neighbor_z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
					if (editedobj->rawData[neighbor_index].data != VOX_EMPTY) {
						surface_flag = true;
						break;
					}
				}
				if (surface_flag) {
					tempArray[target_index] = VOX_SURFACE;
				}
			}
		}
	}

	// cover back to edit data
	for (int z = 0; z < EDITSPACERESOLUTION; z++) {
		for (int y = 0; y < EDITSPACERESOLUTION; y++) {
			for (int x = 0; x < EDITSPACERESOLUTION; x++) {
				target_index = x + y * EDITSPACERESOLUTION + z * EDITSPACERESOLUTION * EDITSPACERESOLUTION;
				if(tempArray[target_index] == VOX_SURFACE)
				editedobj->rawData[target_index].data = tempArray[target_index];

				if (editedobj->rawData[target_index].data == VOX_EMPTY) {
					editedobj->rawData[target_index].ID = 0;
				}
				else {
					editedobj->rawData[target_index].ID = 1;
				}
			}
		}
	}
	free(tempArray);

	VBO* vbo;

	deleteVBO(&editedobj->vbo);
	vbo = createEditVoxelVBO(editedobj);
	setColorVBO(1.0, 1.0, 1.0, 1.0, vbo);
	bindData(vbo);

	saveButton->show();

	return;
}

void VoxelEditorOGLWidget::opening(int erosionTimes, int diolationTimes, int neighborMode)
{
	for (int i = 0; i < erosionTimes;i++) {
		erosion(neighborMode);
	}
	for (int i = 0; i < diolationTimes; i++) {
		dilation(neighborMode);
	}
	return;
}

void VoxelEditorOGLWidget::closing(int diolationTimes, int erosionTimes, int neighborMode)
{
	for (int i = 0; i < diolationTimes; i++) {
		dilation(neighborMode);
	}
	for (int i = 0; i < erosionTimes; i++) {
		erosion(neighborMode);
	}
	return;
}

void VoxelEditorOGLWidget::saveButtonPress(void)
{
	saveButton->setIconSize(QSize(40, 40));
}

void VoxelEditorOGLWidget::saveButtonRelease(void)
{
	saveButton->setIconSize(QSize(60, 60));
	if (editedobj != NULL) {
		// check free form buffer is or not save
		if ( axisFreeformBufferChanged[X]) {
				restoreToEditData(X);
		}
		if (axisFreeformBufferChanged[Y]) {
			restoreToEditData(Y);
		}
		if (axisFreeformBufferChanged[Z]) {
			restoreToEditData(Z);
		}

		// save data
		int minRange[3], maxRange[3];

		calculateEditDataBBOX(minRange, maxRange);
		
		savebackData(minRange, maxRange);

		dataChangeCheck = true;

		saveButton->setVisible(false);
	}
}

void VoxelEditorOGLWidget::changeToViewWidget(int widgetID)
{
	if (widgetID == 0) {
		cleanEditBuffer();

		if ( dataChangeCheck == true ) {

			VBO* vbo;

			deleteVBO(&this->vItem->vdata->vbo);
			vbo = createVoxelVBO(this->vItem->vdata);
			setColorVBO(1.0, 1.0, 1.0, 1.0, vbo);
			bindData(vbo);

			createVoxelClipPlane(this->vItem->vdata);
		}
	}
}

