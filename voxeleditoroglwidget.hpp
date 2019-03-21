#pragma once
extern "C" {
#include "vbo.h"
#include "voxel.h"
#include "meshmodel.h"
#include "distancefield.h"
#include "light.h"
#include "editvoxel.h"
#include "editvoxelvbo.h"
}
#include "lightwidget.hpp"
#include "freeformwidget.hpp"
#include "smoothingwidget.hpp"
#include "voxelizewidget.hpp"
#include "voxeldatalistmanagerwidget.hpp"
#include <GL\glew.h>
#include <QOpenGLWidget>
#include <QSlider>
#include <QSpinBox>

class VoxelEditorOGLWidget : public QOpenGLWidget {
	Q_OBJECT

public:
	VoxelEditorOGLWidget(QWidget *parent = Q_NULLPTR);
	~VoxelEditorOGLWidget();

protected:
	// opengl function
	void initializeGL(void);
	void paintGL(void);
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

private:
	// extended opengl function
	void setupOpenGL(void);
	void setViewingMatrix(void);	// init
	void setLighting(void);
	void setMaterial(VBO obj);
	void fixView(void);
	void updateViewing(int);
	void createRangeBox(int);
	void createDirectionArrow(int);

	// render function
	void bindData(VBO *vbo);	// after vbo created
	void drawStaticScene(int mode);
	void drawData(VBO *const vbo, int mode, int windowmode);
	void renderScene(int mode);
	void showThreeDWindow(int);
	void showXAxisWindow(int);
	void showYAxisWindow(int);
	void showZAxisWindow(int);

	// data process 
	void initEditData(void);
	void cleanEditBuffer(void);
	// copy edit data to 512*512*512 space
	void copyData(void);
	void calculateConnetComponent(void);
	// make slice
	void mallocSlice(void);
	void setSliceData(int);
	//calculate bbox
	void calculateEditDataBBOX(int*,int*);

	// free form buffer method
	void setFreeFormBuffer(float*);
	void changeEditData(int, int);
	void changeAxisFreeFormBufferData(int, bool);
	bool restoreToEditData(int);
	void clearFreeFormBuffer(int);

	// change operation mode
	void changeOPMode(int);

	// save back to original data
	void savebackData(int*,int*);

signals:
	
	void sendVoxelObject(VoxelObject *obj);

	public slots:
	// open widget
	void openLightWidget(void);
	void openControlWidget(void);
	void openSelectionWidget(void);
	void openFreeFormWidget(void);
	void openSmoothingWidget(void);
	void recalculateConnectingComponent(void);

	// accept data item
	void acceptVoxelItem(QTreeWidgetItem*, int);

	// slice layer value change
	void xAxisSliderChangerd(int);
	void yAxisSliderChangerd(int);
	void zAxisSliderChangerd(int);
	void xAxisSpinboxChangerd(int);
	void yAxisSpinboxChangerd(int);
	void zAxisSpinboxChangerd(int);

	void changeNumOfAxisLayer(int);

	// widget
	void setFreeFormParameter(int, int);

	// free form button slot
	void penButtonPress(void);
	void penButtonRelease(void);
	void eraserButtonPress(void);
	void eraserButtonRelease(void);
	void resetButtonPress(void);
	void resetButtonRelease(void);

	// smoothing widget slot
	void erosion(int neighborMode);
	void dilation(int neighborMode);
	void opening(int erosionTimes, int dilationTimes,int neighborMode);
	void closing(int dilationTimes, int erosionTimes,int neighborMode);

	// save button slot
	void saveButtonPress(void);
	void saveButtonRelease(void);

	// change tab to voxel view
	void changeToViewWidget(int);

private:
	LightWidget *lightWidget;
	FreeFormWidget *freeformwidget;
	SmoothingWidget *smoothingwidget;

private:

	// camera
	float eye[3];
	float lok[3];
	float vup[3];
	float xAxis_eye[3];
	float xAxis_lok[3];
	float xAxis_vup[3];
	float yAxis_eye[3];
	float yAxis_lok[3];
	float yAxis_vup[3];
	float zAxis_eye[3];
	float zAxis_lok[3];
	float zAxis_vup[3];
	float u[3];
	float v[3];
	float w[3];
	float viewTrans[3];
	float viewRot[3];
	float viewPhi;
	float viewTheta;
	float viewRadius;
	float fovy;
	float xAxis_width;
	float yAxis_width;
	float zAxis_width;
	float nearClip;
	float farClip;
	float curRot[16];
	float lastRot[16];

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

	float xAxis_viewMat[16];
	float xAxis_projectionMat[16];
	float xAxis_mvMat[16];
	float xAxis_mvpMat[16];
	float xAxis_normalMat[16];

	float yAxis_viewMat[16];
	float yAxis_projectionMat[16];
	float yAxis_mvMat[16];
	float yAxis_mvpMat[16];
	float yAxis_normalMat[16];

	float zAxis_viewMat[16];
	float zAxis_projectionMat[16];
	float zAxis_mvMat[16];
	float zAxis_mvpMat[16];
	float zAxis_normalMat[16];

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

	// window mode
	int windowMode;
	// operating mode
	int opMode;
	// free form parameter
	int ff_Method = FF_CIRCLE;
	int ff_Radius = 1;
	float mouse1_2Position[2][3];	// record first position and second position
	int ff_oldPosX;
	int ff_oldPosY;

	// standard rendering object and object parameter\
	// standard vbo
	VBO *rangeBox[12];
	VBO *directionArrow[6];
	// slider and spinbox 
	QSlider* xAxisLayer_slider;
	QSlider* yAxisLayer_slider;
	QSlider* zAxisLayer_slider;
	QSpinBox* xAxisLayer_spinbox;
	QSpinBox* yAxisLayer_spinbox;
	QSpinBox* zAxisLayer_spinbox;
	int slider_padding;

	// freeform button
	QPushButton *penButton;
	QPushButton *eraserButton;
	QPushButton *resetButton;

	// edit voxel item
	VoxelItem *vItem = NULL;
	EditVoxelData *editedobj = NULL;		// a sapce allocate data to edit , the size is define in .cpp: EDITSPACERESOLUTION * EDITSPACERESOLUTION * EDITSPACERESOLUTION
	int numOfAxisLayer[3];
	EditVoxelData* axisLayerBuffer[3] = { NULL,NULL,NULL };
	double axisLayerBufferColor[3][4] = {
		{ 0.38, 1.0, 1.0, 1.0 },
		{ 1.0, 0.38, 1.0, 1.0 },
		{ 1.0, 1.0, 0.38, 1.0 }
	};
	int num_connecting_component;

	// method buffer
	EditVoxelData* copyBuffer = NULL;

	bool editDataChanged;
	bool axisFreeformBufferChanged[3] = { false, false ,false };
	EditVoxelData* axisFreeformBuffer[3] = { NULL,NULL,NULL };

	VBO* axisFreeFromTool[3] = { NULL,NULL,NULL };


	// save button
	QPushButton *saveButton;
	bool dataChangeCheck;
};
