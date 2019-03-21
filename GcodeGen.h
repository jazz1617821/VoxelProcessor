/*_______________________________________________________________________________
	GcodeGen.h
	2.0
	Z.Y. Liu
	2016.9.15
	NTOU_CGLAB_10557007
_________________________________________________________________________________
	This project
	------------
	Gcode generate for 3D printer project.
	compile slice voxel's picture(bmp or inner memory) into g-code.
	note that most of function useing commputer shader to speed up.
	make sure your OpenGL lib support SSBO and comp shader.
	GLEW is recommended.

	Algorithm base : 
		1) Edge stroke planning
		2) Filling stroke planning
		3) GCode generation by stroke and macchine config.
_________________________________________________________________________________
	2016.7.26    project start.
	2016.9.14    project almost complete ... redux and cleaning project 
_________________________________________________________________________________
	basic header
	------------
	stdio.h		: c standard I/O
	stdlib.h	: c standard function
	string.h	: c version string
	string		: c++ version string
	cmath       : math lib

	graphic lib
	-----------
	GLEW :
	The OpenGL Extension Wrangler Library (GLEW) is a cross-platform open-source 
	C/C++ extension loading library. GLEW provides efficient run-time mechanisms 
	for determining which OpenGL extensions are supported on the target platform.
	using version 2.0

	SPECIAL THANK
	-------------
	Man and women who working on open source and lead this world to better 
	tomorrow. Thank You!
_______________________________________________________________________________*/
/// GcodeGen sign ///
#ifndef __GcodeGen_h__
#define __GcodeGen_h__
#define __GCODEGEN_H__

/// debug mode ///
#define __GCODEGEN_DEBUG__
#ifdef __GCODEGEN_DEBUG__
	#ifndef __GLEW_H__
		#ifndef __glew_h__
			#error glew.h not include
		#endif
	#endif
#endif

/// GcodeGen Limit ///
#define __GCODEGEN_IMG_SIZ__ 262144

/*_______________________________________________________________________________
	data handle
	-----------
	0. bmp loading function
	1. bin loading function
_______________________________________________________________________________*/
void GGLoadBMP(const char* file_path);
void GGLoadBIN(unsigned char* src, int src_x, int src_y);
void GGLoadBIN(unsigned int* src, int src_x, int src_y);

/*_______________________________________________________________________________
	graph function
	--------------
	0. Clean
	1. Full connect graph generate
	2. Spanning tree generate
	3. DFS path
	4. Dijkstra path
	5. fatEdge
_______________________________________________________________________________*/
void GGClean(GLuint dst, int target, int siz);
void GGFullConnectGraph(GLuint dst, GLuint raw, int target);
void GGSpanningTree(GLuint dst, GLuint src, int target);
int GGDFSPath(GLuint dst, GLuint src, int start, int end);
int GGDFScirclePath(GLuint dst, GLuint src, GLuint support, int start);
int GGDijkstraPath(GLuint dst, GLuint src, int start, int end);
void GGfatEdge(int wall, int empty);
int GGInfection(int target, int start);
void GGcommit(int target, int commit);
/*_______________________________________________________________________________
	GCode write function
	--------------------
	0. Edge stroke
	1. Fill stroke
_______________________________________________________________________________*/
int GGEdgeStroke(GLuint dst, GLuint ref, int target, int start);
int GGFillStroke(GLuint dst, GLuint ref, int target, int wall, int start, int fdir);

/*_______________________________________________________________________________
	main function
	-------------
	0. init
	1. exit
	2. main
_______________________________________________________________________________*/
#define GG_MACHINE_F_TEMP 0
#define GG_MACHINE_F_DIM_X 1 
#define GG_MACHINE_F_DIM_Y 2
#define GG_MACHINE_F_DIM_Z 3
#define GG_MACHINE_F_DIM_NOZ 4
#define GG_MACHINE_F_VOX_X 5
#define GG_MACHINE_F_VOX_Y 6
#define GG_MACHINE_F_VOX_Z 7
#define GG_MACHINE_F_FIL 8
#define GG_MACHINE_F_C_X 9
#define GG_MACHINE_F_C_Y 10
#define GG_MACHINE_I_00 0
#define GG_MACHINE_I_01 1
#define GG_MACHINE_I_S 2
#define GG_MACHINE_I_S0_00 3
#define GG_MACHINE_I_S1_00 4
#define GG_MACHINE_I_S2_00 5
#define GG_MACHINE_I_S3_00 6
#define GG_MACHINE_I_S4_00 7
#define GG_MACHINE_I_S5_00 8

#define GG_MACHINE_I_S0_01 9
#define GG_MACHINE_I_S1_01 10
#define GG_MACHINE_I_S2_01 11
#define GG_MACHINE_I_S3_01 12
#define GG_MACHINE_I_S4_01 13
#define GG_MACHINE_I_S5_01 14

void GGInit(void);
void GGExit(void);
void GGMain(int &layer, int &start, float &fill, FILE* file_gcode);
void GGMachine(int conf, float value);
void GGMachine(int conf, int value);
void GGTell(float* img);
#endif /// __GcodeGen_h__ ///