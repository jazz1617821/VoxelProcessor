#pragma once
extern "C" {
#include "vbo.h"
#include "voxel.h"
#include "meshmodel.h"
#include "distancefield.h"
#include "light.h"
}
#include "templatewidget.hpp"
#include "lightwidget.hpp"
#include "voxelizewidget.hpp"
#include "voxeldistancefieldwidget.hpp"
#include "voxelthicknesswidget.hpp"
#include "slicewidget.hpp"
#include "voxelclipplanewidget.hpp"
#include <GL\glew.h>
#include <QOpenGLWidget>
#include <QKeyEvent>

class VoxelViewerOGLWidget : public QOpenGLWidget {
	Q_OBJECT

public:
	VoxelViewerOGLWidget(QWidget *parent = Q_NULLPTR);
	~VoxelViewerOGLWidget();

protected:
	void initializeGL(void);
	void paintGL(void);
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);

private:
	void setupOpenGL(void);
	void setViewingMatrix(void);	// init
	void setLighting(void);
	void setClipping(VBO *vbo);
	void setMaterial(VBO obj);
	void fixView(void);
	void updateViewing(void);

	// render
	void bindData(VBO *vbo);	// after vbo created
	void drawStaticScene();
	void drawAxes(int mode);
	void drawRotCircle(int mode);
	void drawScaleAxes(int mode);
	void drawData(VBO *const vbo, int mode);
	void drawBbox(VBO *const vbo, int mode);
	void renderScene(void);
	void renderBbox(void);
	void renderOthers(void);

	// shadow mapping
	void genShadowMapping(void);
	void configureShadowMapping(void);
	void renderQuad(void);

	// multi-sampling
	void genMultiSampling(void);

	// ssao
	void genSSAO(void);
	void resizeSSAO(void);
	void configureGBufferShader(void);
	void gBufferRendering(void);
	void ssaoTextureRendering(void);
	void ssaoBlurRendering(void);
	void ssaoLightPassRendering(void);
	void shadowMappingRendering(void);

	// upadte voxel clip afer transform
	void transformVoxelClip(VoxelItem *item);

signals:
	void sendVoxelModel(VoxelModel *sc);	//sent to view manager tree
	void sendVoxelItem(VoxelItem *sc);		//sent to voxel data list
	void voxelizeVoxelModel(VoxelModel *sc, float xSize, float ySize, float zSize);
	void selectVoxel(VoxelItem *item);
	void sendVoxelObject(VoxelObject *obj);

public slots:
	// context menu
	void toggleContextMenu(const QPoint &pos);

	// get seleted voxel from voxel model manager widget
	void actionOnVoxelObject(VoxelObject *obj, int action);
	// open widget
	void openLightWidget(void);
	void openVDFWidget(void);
	void openVTHNWidget(void);
	void openSliceWidget(void);

	// actions
	void slicing(void);
	// gen df
	void genDFVBO(VoxelItem* item, float minValue, float maxValue);
	// change df state
	void changeDFEnable(int state);

	// set the clipping for the selected voxel item
	void setClipItem(VoxelItem *clipItem);
	void updateVoxelClip(int clipIdx, VoxelItem *item);

	// open file
	void openVoxelModel(const char *filepath);
	//save file
	void saveVoxelModel();

	// create Voxel model
	void createVoxelModel(void);

	// export distance fild data
	void exportDFData(VoxelData*);

	// change operation mode
	void changeToViewMode(void);
	void changeToTranslateMode(void);
	void changeToRotateMode(void);
	void changeToScaleMode(void);

	// change tab to edit view
	void changeToEditWidget(int);

private:
	LightWidget *lightWidget;
	VoxelizeWidget *voxelizeWidget;
	VoxelDistanceFieldWidget *vdfWidget;
	VoxelThicknessWidget *vthnWidget;
	SliceWidget *sliceWidget;
	VoxelClipPlaneWidget *clipWidget;
	TemplateWidget *templatewidget;

private:
	int iconSize;
	// operating mode
	int opMode;

	// camera
	float eye[3];
	float lok[3];
	float vup[3];
	float u[3];
	float v[3];
	float w[3];
	float viewTrans[3];
	float viewRot[3];
	float viewPhi;
	float viewTheta;
	float viewRadius;
	float fovy;
	float nearClip;
	float farClip;
	float curRot[16];
	float lastRot[16];
	float orthoWidth;

	// lights
	int numLights;
	Light *lights[10];
	GlobalLight *globalLight;

	// matrices
	float modelMat[16];
	float viewMat[16];
	float projectionMat[16];
	float mvMat[16];
	float mvpMat[16];
	float normalMat[16];


	// shadow
	float lightSpaceMat[16];
	GLuint depthMapFBO;
	GLuint shadowWidth;
	GLuint shadowHeight;
	GLuint depthMap;

	// multi-sampling
	GLuint multiSampleTex;
	GLuint multiSampleFBO;
	GLuint multiSampleColorBuffer;
	GLuint multiSampleDepthBuffer;

	// ssao
	GLuint kernelSize;
	GLuint noiseSize;
	GLfloat kernelRadius;
	GLfloat *ssaoKernel;
	GLfloat *ssaoNoise;
	GLuint quadVAO;
	GLuint quadVBO;

	GLuint gBuffer;
	GLuint gPositionDepth;
	GLuint gNormal;
	GLuint gAlbedo;
	GLuint rboDepth;
	GLuint noiseTexture;
	GLuint ssaoFBO;
	GLuint ssaoBlurFBO;
	GLuint ssaoColorBuffer;
	GLuint ssaoColorBufferBlur;

	// object uniform location
	GLuint colorLoc;

	// light uniform location
	GLint lightLoc0;

	// flag uniform location
	GLint wireframe;

	// shader program
	GLuint program[10];

	// mouse controll
	int oldPosX;
	int oldPosY;
	bool isMousePressed;

	// widget size
	int oldWidth;
	int oldHeight;


	// selected
	VoxelItem *selectedItem;
	VoxelItem *selectedClipItem;
	VoxelObject *selectedObject;
	bool isTransform;

	// voxel model scene
	VoxelModel *vmodel = NULL;

	// ground
	VBO *ground;

	// project cylinder
	VBO *projCylinder;

	// translate axes
	VBO *axisX;
	VBO *axisY;
	VBO *axisZ;
	Transformation *axesTsfm;
	int selectedAxis;
	float selectedModelPos[3];	// the bounding box's center of selected item

	// rotate circles
	VBO *rotCircleX;
	VBO *rotCircleY;
	VBO *rotCircleZ;
	Transformation *rotCircleTsfm;
	int selectedRotCircle;
	float selectedCirclePos[3];

	// scale axes
	VBO *scaleAxisX;
	VBO *scaleAxisY;
	VBO *scaleAxisZ;
	Transformation *scaleAxesTsfm;
	int selectedScaleAxis;
	float selectedScaleAxisPos[3];

	// the transformation for current selected model
	Transformation *modifyingTransformation;
	float modifyingMatrix[16];

	// test texture
	GLuint testTexID;
	bool activeTextureMapping;

	// effects
	bool enableShadowMapping;
	bool enableSSAO;

	// voxel obj add ptr
	VoxelObject* addPtr = NULL;
};
