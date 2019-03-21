/*_______________________________________________________________________________
	GcodeGen.cpp
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
	2016.9.19    Fur handling
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cmath>
#include <GL\glew.h>
#include <GcodeGen.h>

/*_______________________________________________________________________________
	GcodeGen shader
	---------------
	0. Clean
	1. Full connect graph 
	2. Spanning tree
	3. Dijkstra
	4. noMoreHidden
	5. RayTrace
	6. Erosion
	7. Infection
	8. Commit
_______________________________________________________________________________*/

///general buffer storage///

// ort buffer section 1
GLuint SSBOraw = 0xFFFFFFFF;
GLuint SSBOfull = 0xFFFFFFFF;
GLuint SSBOlble = 0xFFFFFFFF;
GLuint SSBOedge = 0xFFFFFFFF;
GLuint SSBOhide = 0xFFFFFFFF;

// ort buffer section 2
GLuint SSBOtree = 0xFFFFFFFF;
GLuint SSBOdist = 0xFFFFFFFF;
GLuint SSBOpick = 0xFFFFFFFF;
GLuint SSBOwalk = 0xFFFFFFFF;
GLuint SSBOgtyp = 0xFFFFFFFF;

// ort buffer section 3
GLuint SSBOpath_0 = 0xFFFFFFFF;
GLuint SSBOpath_1 = 0xFFFFFFFF;
GLuint SSBOpath_2 = 0xFFFFFFFF;
GLuint SSBOimg = 0xFFFFFFFF;
GLuint SSBOcom = 0xFFFFFFFF;
GLuint SSBOsup = 0xFFFFFFFF;
GLuint SSBOTell = 0xFFFFFFFF;

///0. Clean///
GLuint shaderClean = 0xFFFFFFFF;
static const char textClean[] = {
	"#version 450 core\n"
	"layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;"

	"layout (std430, binding = 0) buffer SSBOraw{int raw[];};"
	"layout(location = 0) uniform int target;"

	"void main(void){"
	"	uvec3 dim  = gl_WorkGroupSize*gl_NumWorkGroups;"
	"	int   id   = int(gl_GlobalInvocationID.x*dim.y+gl_GlobalInvocationID.y);"
	"	raw[id] = target;"
	"}"
};

///1. Full connect graph///
GLuint shaderFullConnectGraph = 0xFFFFFFFF;
static const char textFullConnectGraph[] = {
	"#version 450 core\n"
	"layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;"

	"layout (std430, binding = 0) buffer SSBOraw{int raw[];};"
	"layout (std430, binding = 1) buffer SSBOdst{int dst[];};"
	"layout(location = 0) uniform int target;"

	"void main(void){"
	"	uvec3 dim  = gl_WorkGroupSize*gl_NumWorkGroups;"
	"	int   id   = int(gl_GlobalInvocationID.x*dim.y+gl_GlobalInvocationID.y);"
	"	int p8[8];"
	"	p8[0] = raw[id+1];"
	"	p8[1] = raw[id+dim.y+1];"
	"	p8[2] = raw[id+dim.y];"
	"	p8[3] = raw[id+dim.y-1];"
	"	p8[4] = raw[id-1];"
	"	p8[5] = raw[id-dim.y-1];"
	"	p8[6] = raw[id-dim.y];"
	"	p8[7] = raw[id-dim.y+1];"
	"	int lable = 0;"
	"	if(target==-1&&raw[id]!=0){"
	"		if (p8[0]!=0){lable+=0x01;}"
	"		if (p8[1]!=0){lable+=0x02;}"
	"		if (p8[2]!=0){lable+=0x04;}"
	"		if (p8[3]!=0){lable+=0x08;}"
	"		if (p8[4]!=0){lable+=0x10;}"
	"		if (p8[5]!=0){lable+=0x20;}"
	"		if (p8[6]!=0){lable+=0x40;}"
	"		if (p8[7]!=0){lable+=0x80;}"
	"		dst[id]=lable;"
	"	}else if(target!=-1&&raw[id]==target){"
	"		if (p8[0]==target){lable+=0x01;}"
	"		if (p8[1]==target){lable+=0x02;}"
	"		if (p8[2]==target){lable+=0x04;}"
	"		if (p8[3]==target){lable+=0x08;}"
	"		if (p8[4]==target){lable+=0x10;}"
	"		if (p8[5]==target){lable+=0x20;}"
	"		if (p8[6]==target){lable+=0x40;}"
	"		if (p8[7]==target){lable+=0x80;}"
	"		dst[id]=lable;"
	"	}"
	
	"}"
};

///2. Spanning tree///
GLuint shaderSpanningTree = 0xFFFFFFFF;
static const char textSpanningTree[] = {
	"#version 450 core\n"
	"layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;"

	"layout (std430, binding = 0) buffer SSBOsrc{int src[];};"
	"layout (std430, binding = 1) buffer SSBOdst{int dst[];};"

	"layout (std430, binding = 4) buffer SSBOpick{int pick;};"
	"layout(location = 0) uniform int target;"
	"void main(void){"
	"	uvec3 dim  = gl_WorkGroupSize*gl_NumWorkGroups;"
	"	int   id   = int(gl_GlobalInvocationID.x*dim.y+gl_GlobalInvocationID.y);"
	"	if (dst[id] == 0 && src[id] == target) { pick = id; }"
	"}"
};

///3. Dijkstra///
GLuint shaderDijkstra = 0xFFFFFFFF;
static const char textDijkstra[] = {
	"#version 450 core\n"
	"layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;"

	"layout (std430, binding = 0) buffer SSBOsrc{int src[];};"

	"layout (std430, binding = 1) buffer SSBOdist{int dist[];};"
	"layout (std430, binding = 2) buffer SSBOpick{int pick;};"

	"layout(location = 0) uniform int level;"

	"void main(void){"
	"	uvec3 dim  = gl_WorkGroupSize*gl_NumWorkGroups;"
	"	int   id   = int(gl_GlobalInvocationID.x*dim.y+gl_GlobalInvocationID.y);"
	"	int p8[8];"
	"	if(dist[id]==level){"
	"		p8[0] = int(id+1);"
	"		p8[1] = int(id+dim.y+1);"
	"		p8[2] = int(id+dim.y);"
	"		p8[3] = int(id+dim.y-1);"
	"		p8[4] = int(id-1);"
	"		p8[5] = int(id-dim.y-1);"
	"		p8[6] = int(id-dim.y);"
	"		p8[7] = int(id-dim.y+1);"
	"		for(int i=0;i<8;++i){"
	"			if(((src[id]>>i)&1)==1){"
	"				atomicMin(dist[p8[i]],level+1);"
	"				pick = 1;"
	"			}"
	"		}"
	"	}"
	"}"
};

///4. NoMoreHidden///
GLuint shaderNoMoreHidden = 0xFFFFFFFF;
static const char textNoMoreHidden[] = {
	"#version 450 core\n"
	"layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;"

	"layout (std430, binding = 0) buffer SSBOsrc{int src[];};"
	"layout (std430, binding = 1) buffer SSBOlble{int lab[];};"
	"layout (std430, binding = 2) buffer SSBOdst{int dst[];};"
	"layout (std430, binding = 3) buffer SSBOpick{int pick;};"

	"layout(location = 0) uniform int period;"

	"void main(void){"
	"	uvec3 dim  = gl_WorkGroupSize*gl_NumWorkGroups;"
	"	int   id   = int(gl_GlobalInvocationID.x*dim.y+gl_GlobalInvocationID.y);"
	"	int i = 0;"
	"	int ner = 0;"
	"	switch(period){"
	"		case 0:"
	"		for(i=0;i<8;++i){"
	"			if(((src[id]>>i)&1)==1){"
	"				ner++;"
	"			}"
	"		}"
	"		if(ner==1){"
	"			for(i=0;i<8;++i){"
	"				if("
	"					((src[id]>>i)&1)==0&&"
	"					((lab[id]>>i)&1)==1"
	"				){"
	"					dst[id]|=1<<i;"
	"					break;"
	"				}"
	"			}"
	"		}"
	"		break;"

	"		case 1:"
	"		for(i=0;i<8;++i){"
	"			if(((lab[id]>>i)&1)==1){"
	"				ner++;"
	"			}"
	"		}"
	"		if(ner==1){pick=id;}"
	"		break;"

	"		case 2:"
	"		for(i=0;i<8;++i){"
	"			if(((dst[id]>>i)&1)==1){"
	"				ner++;"
	"			}"
	"		}"
	"		if(ner!=0){pick=id;}"
	"		break;"

	"	}"
	"}"
};

///5. RayTrace///
GLuint shaderRayTrace = 0xFFFFFFFF;
static const char textRayTrace[] = {
	"#version 450 core\n"
	"layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;"

	"layout (std430, binding = 0) buffer SSBOlble{int lble[];};"
	"layout (std430, binding = 1) buffer SSBOproj{int proj[];};"

	"layout(location = 0) uniform vec2 dimen;"
	"layout(location = 1) uniform int fdir;"

	"void main(void){"
	"	ivec2 loc;"
	"	int id = 0;"
	"	if(fdir == 0){"
	"		loc = ivec2(0,gl_GlobalInvocationID.x);"
	"		while(loc.x<dimen.x&&loc.y<dimen.y){"
	"			id = loc.x*int(dimen.y) + loc.y;"
	"			if(((lble[id]>>2)&1)==0&&((lble[id]>>6)&1)==1){atomicAdd(proj[loc.y],1);}"
	"			if(((lble[id]>>2)&1)==1&&((lble[id]>>6)&1)==0){atomicAdd(proj[loc.y],1);}"
	"			loc += ivec2(1,0);"
	"		}"
	"	}else{"
	"		loc = ivec2(gl_GlobalInvocationID.x,0);"
	"		while(loc.x<dimen.x&&loc.y<dimen.y){"
	"			id = loc.x*int(dimen.y) + loc.y;"
	"			if(((lble[id]>>0)&1)==0&&((lble[id]>>4)&1)==1){atomicAdd(proj[loc.x],1);}"
	"			if(((lble[id]>>0)&1)==1&&((lble[id]>>4)&1)==0){atomicAdd(proj[loc.x],1);}"
	"			loc += ivec2(0,1);"
	"		}"
	"	}"
	"}"
};

///6. Erosion///
GLuint shaderErosion = 0xFFFFFFFF;
static const char textErosion[] = {
	"#version 450 core\n"
	"layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;"

	"layout (std430, binding = 0) buffer SSBORaw{int raw[];};"
	"layout (std430, binding = 1) buffer SSBOImg{int img[];};"
	"layout (std430, binding = 2) buffer SSBOTell{int tell[];};"
	"layout(location = 0) uniform int target;"
	"layout(location = 1) uniform int empty;"
	"void main(void){"
	"	uvec3 dim  = gl_WorkGroupSize*gl_NumWorkGroups;"
	"	int   id   = int(gl_GlobalInvocationID.x*dim.y+gl_GlobalInvocationID.y);"
	"	int p4[4];"
	"	p4[0] = int(id+1);"
	"	p4[1] = int(id+dim.y);"
	"	p4[2] = int(id-1);"
	"	p4[3] = int(id-dim.y);"
	"	if(raw[id]!=0 && img[id]!=1){"
	"		for(int i=0;i<4;++i){"
	"			if(raw[p4[i]] == empty){"
	"				raw[id] = target;"
	"				tell[id] = target;"
	"				img[id] = 1;"
	"				break;"
	"			}"
	"		}"
	"	}"
	"}"
};

///7.infection///
GLuint shaderInfection = 0xFFFFFFFF;
static const char textInfection[] = {
	"#version 450 core\n"
	"layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;"

	"layout (std430, binding = 0) buffer SSBOraw{int raw[];};"
	"layout (std430, binding = 1) buffer SSBOdst{int dst[];};"
	"layout (std430, binding = 2) buffer SSBOpick{int pick;};"

	"layout(location = 0) uniform int period;"
	"layout(location = 1) uniform int target;"

	"void main(void){"
	"	uvec3 dim  = gl_WorkGroupSize*gl_NumWorkGroups;"
	"	int   id   = int(gl_GlobalInvocationID.x*dim.y+gl_GlobalInvocationID.y);"
	"	int i = 0;"
	"	int p8[8];"
	"	p8[0] = int(id+1);"
	"	p8[1] = int(id+dim.y+1);"
	"	p8[2] = int(id+dim.y);"
	"	p8[3] = int(id+dim.y-1);"
	"	p8[4] = int(id-1);"
	"	p8[5] = int(id-dim.y-1);"
	"	p8[6] = int(id-dim.y);"
	"	p8[7] = int(id-dim.y+1);"
	"	switch(period){"
	"		case 0:if(raw[id]==target&&dst[id]==0){pick=id;}break;"
	"		case 1:"
	"		if(raw[id] == target){dst[id] = target; break;}"
	"		for(i=0;i<8;++i){"
	"			if(raw[p8[i]] == target && raw[id]!=0){"
	"				dst[id] = target;"
	"				break;"
	"			}"
	"		}"
	"		break;"
	"		case 2:if(dst[id]==target&&raw[id]!=target){raw[id]=target;pick=1;}break;"
	"	}"
	"}"
};

///8.commit///
GLuint shaderCommit = 0xFFFFFFFF;
static const char textCommit[] = {
	"#version 450 core\n"
	"layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;"

	"layout (std430, binding = 0) buffer SSBOraw{int raw[];};"
	"layout (std430, binding = 1) buffer SSBOdst{int dst[];};"
	"layout (std430, binding = 2) buffer SSBOTell{int tell[];};"

	"layout(location = 0) uniform int target;"
	"layout(location = 1) uniform int commit;"
	"void main(void){"
	"	uvec3 dim  = gl_WorkGroupSize*gl_NumWorkGroups;"
	"	int   id   = int(gl_GlobalInvocationID.x*dim.y+gl_GlobalInvocationID.y);"
	"	if(dst[id] == target){raw[id] = commit; tell[id] = commit;}"
	"	else{raw[id] = 0;}"
	"}"
};

/*_______________________________________________________________________________
	data handle
	-----------
	0. bmp loading function
	1. bin loading function
_______________________________________________________________________________*/

///BMP file structure///
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long  LONG;

#pragma pack(push, 1)
typedef struct _tagBITMAPFILEHEADER{
	WORD bfType;       //specifies the file type
	DWORD bfSize;      //specifies the size in bytes of the bitmap file
	WORD bfReserved1;  //reserved; must be 0
	WORD bfReserved2;  //reserved; must be 0
	DWORD bOffBits;    //species the offset in bytes from the bitmapfileheader to the bitmap bits
}_BITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _tagBITMAPINFOHEADER{
	DWORD biSize;          //specifies the number of bytes required by the struct
	LONG biWidth;          //specifies width in pixels
	LONG biHeight;         //species height in pixels
	WORD biPlanes;         //specifies the number of color planes, must be 1
	WORD biBitCount;       //specifies the number of bit per pixel
	DWORD biCompression;   //spcifies the type of compression
	DWORD biSizeImage;     //size of image in bytes
	LONG biXPelsPerMeter;  //number of pixels per meter in x axis
	LONG biYPelsPerMeter;  //number of pixels per meter in y axis
	DWORD biClrUsed;       //number of colors used by th ebitmap
	DWORD biClrImportant;  //number of colors that are important
}_BITMAPINFOHEADER;
#pragma pack(pop)

///Raw data storage///
int RawSiz;
int RawDim[2];

///0. bmp loading function///
void GGLoadBMP(const char* file_path){
	_BITMAPFILEHEADER bitmapFileHeader;
	_BITMAPINFOHEADER bitmapInfoHeader;

	FILE* file_bmp = NULL;
	fopen_s(&file_bmp, file_path, "rb");

#ifdef __GCODEGEN_DEBUG__ 
	printf("GGLoadBMP::reading bmp %s...", file_path);
	if (file_bmp == NULL) {
		printf("\nGGLoadBMP::ERR::Unable to open bmp file ... exiting\n");
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
#endif
	if (file_bmp == NULL) {
		exit(EXIT_FAILURE);
	}

	fread_s(&bitmapFileHeader, sizeof(_BITMAPFILEHEADER), sizeof(_BITMAPFILEHEADER), 1, file_bmp);
	fread_s(&bitmapInfoHeader, sizeof(_BITMAPINFOHEADER), sizeof(_BITMAPINFOHEADER), 1, file_bmp);
	fseek(file_bmp, bitmapFileHeader.bOffBits, SEEK_SET);

	RawDim[0] = bitmapInfoHeader.biWidth;
	RawDim[1] = bitmapInfoHeader.biHeight;
	RawSiz = RawDim[0] * RawDim[1];

#ifdef __GCODEGEN_DEBUG__
	if (RawSiz > __GCODEGEN_IMG_SIZ__) {
		printf("\nGGLoadBMP::ERR::Unable to process image higher then FULL HD\n");
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
#endif
	if (RawSiz > __GCODEGEN_IMG_SIZ__) {
		exit(EXIT_FAILURE);
	}

	GGClean(SSBOraw, 0, __GCODEGEN_IMG_SIZ__);
	int* raw = (int*)glMapNamedBuffer(SSBOraw, GL_WRITE_ONLY);

	unsigned char tmp[3];
	int pad = 0;
	if ((RawDim[0] * 3) % 4 != 0) {
		pad = 4 - ((RawDim[0] * 3) % 4);
	}
	int id = 0;
	for (int j = 0; j < RawDim[1]; ++j) {
		for (int i = 0; i < RawDim[0]; ++i) {
			fread_s(&tmp, 3, 1, 3, file_bmp);
			id = i*int(RawDim[1]) + j;
			raw[id] = int(tmp[0]);
		}
		fseek(file_bmp, pad*sizeof(unsigned char), SEEK_CUR);
	}

	fclose(file_bmp);
	glUnmapNamedBuffer(SSBOraw);

#ifdef __GCODEGEN_DEBUG__
	printf("complete\n");
#endif
}

///1. bin loading function///
void GGLoadBIN(unsigned char* src, int src_x, int src_y){
	RawDim[0] = src_x;
	RawDim[1] = src_y;
	RawSiz = RawDim[0] * RawDim[1];

#ifdef __GCODEGEN_DEBUG__
	printf("GGLoadBMP::loading from bin...");
	if (RawSiz > __GCODEGEN_IMG_SIZ__) {
		printf("\nGGLoadBIN::ERR::Unable to process image higher then FULL HD\n");
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
#endif
	if (RawSiz > __GCODEGEN_IMG_SIZ__) {
		exit(EXIT_FAILURE);
	}

	GGClean(SSBOraw, 0, __GCODEGEN_IMG_SIZ__);
	int* raw = (int*)glMapNamedBuffer(SSBOraw, GL_WRITE_ONLY);

	int id = 0;
	for (int i = 0; i < RawDim[0]; ++i) {
		for (int j = 0; j < RawDim[1]; ++j) {
			id = i*int(RawDim[1]) + j;
			raw[id] = int(src[id]);
		}
	}

	glUnmapNamedBuffer(SSBOraw);

#ifdef __GCODEGEN_DEBUG__
	printf("complete\n");
#endif
}

void GGLoadBIN(unsigned int* src, int src_x, int src_y){
	RawDim[0] = src_x;
	RawDim[1] = src_y;
	RawSiz = RawDim[0] * RawDim[1];

#ifdef __GCODEGEN_DEBUG__
	printf("GGLoadBMP::loading from bin...");
	if (RawSiz > __GCODEGEN_IMG_SIZ__) {
		printf("\nGGLoadBIN::ERR::Unable to process image higher then FULL HD\n");
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
#endif
	if (RawSiz > __GCODEGEN_IMG_SIZ__) {
		exit(EXIT_FAILURE);
	}

	GGClean(SSBOraw, 0, __GCODEGEN_IMG_SIZ__);
	int* raw = (int*)glMapNamedBuffer(SSBOraw, GL_WRITE_ONLY);

	int id = 0;
	for (int i = 0; i < RawDim[0]; ++i) {
		for (int j = 0; j < RawDim[1]; ++j) {
			id = i*int(RawDim[1]) + j;
			raw[id] = src[id];
		}
	}

	glUnmapNamedBuffer(SSBOraw);

#ifdef __GCODEGEN_DEBUG__
	printf("complete\n");
#endif
}

/*_______________________________________________________________________________
	graph function
	--------------
	0. Clean
	1. Full connect graph generate
	2. Spanning tree generate
	3. DFS path
	4. Dijkstra path
	5. GGfatEdge
	6. GGInfection
	7. GGcommit
_______________________________________________________________________________*/

///0. Clean///
void GGClean(GLuint dst, int target, int siz){
	glUseProgram(shaderClean);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dst);
	glUniform1i(0, target);
	glDispatchCompute(siz, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

///1. Full connect graph generate///
void GGFullConnectGraph(GLuint dst, GLuint raw, int target){
	GGClean(dst, 0, __GCODEGEN_IMG_SIZ__);
	glUseProgram(shaderFullConnectGraph);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, raw);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, dst);
	glUniform1i(0, target);
	glDispatchCompute(RawDim[0], RawDim[1], 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}


///2. Spanning tree generate///
void GGTremauxTreeGraph(GLuint dst, GLuint src, int startP, int target, int sizX, int sizY) {
	int *_img = NULL;
	int* _src = NULL;
	int* _dst = NULL;

	int p8[8];
	int hub = 0;
	int stck = 0;
	int curP = -1;
	int next = -1;
	int next_way = -1;

	curP = startP;

	_img = (int*)glMapNamedBuffer(SSBOdist, GL_READ_WRITE);
	_dst = (int*)glMapNamedBuffer(dst, GL_READ_WRITE);
	_src = (int*)glMapNamedBuffer(src, GL_READ_WRITE);

	// Tremaux Tree construction
	if (curP != -1) {
		while (true) {
			p8[0] = curP + 1;
			p8[1] = curP + sizY + 1;
			p8[2] = curP + sizY;
			p8[3] = curP + sizY - 1;
			p8[4] = curP - 1;
			p8[5] = curP - sizY - 1;
			p8[6] = curP - sizY;
			p8[7] = curP - sizY + 1;
			hub = 0;
			for (int i = 0; i < 8; ++i) {
				if (_src[p8[i]] == target && _dst[p8[i]] == 0) {
					hub++;
					next = p8[i];
					next_way = i;
				}
			}
			
			if (hub > 1) {
				_img[stck++] = curP;
			}
			if (hub == 0 && stck != 0) {
				curP = _img[--stck];
				continue;
			}
			if (hub == 0 && stck == 0) {
				if (curP == startP) { _src[curP] = 0; }
				break;
			}
			_dst[curP] |= 1 << next_way;
			_dst[p8[next_way]] |= 1 << ((next_way + 4) % 8);
			curP = next;
		}
	}
	glUnmapNamedBuffer(SSBOdist);
	glUnmapNamedBuffer(src);
	glUnmapNamedBuffer(dst);
}


void GGSpanningTree(GLuint dst, GLuint src, int target){
	GGClean(dst, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(SSBOdist, 0, __GCODEGEN_IMG_SIZ__);

	glUseProgram(shaderSpanningTree);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, src);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, dst);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, SSBOpick);

	int* pick = NULL;
	int curp = -1;
	
	while (true){
		// find next tree
		pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
		*pick = -1;
		glUnmapNamedBuffer(SSBOpick);
		glUniform1i(0, target);
		glDispatchCompute(RawDim[0], RawDim[1], 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
		if (*pick == -1) { glUnmapNamedBuffer(SSBOpick); break; }
		curp = *pick;
		glUnmapNamedBuffer(SSBOpick);
		GGTremauxTreeGraph(dst, src, curp, target, RawDim[0], RawDim[1]);
	}

}

///3. DFS path///
int GGDFSPath(GLuint dst, GLuint src, int start, int end){
	GGClean(dst, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(SSBOtree, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(SSBOdist, 0, __GCODEGEN_IMG_SIZ__);
	
	int *path = NULL;
	int *mark = NULL;
	int *stak = NULL;
	int *edge = NULL;

	int pick = start;
	float ploc[2] = { 0, 0 };
	float eloc[2] = { 0, 0 };
	int p8[8];
	int curp = 0;
	int curs = 0;
	int hub = 0;
	int way = -1;
	float delta = -1;
	float delta_tmp = -1;

	eloc[0] = float(end % RawDim[1]);
	eloc[1] = (float(end) - eloc[0]) / float(RawDim[0]);

	path = (int*)glMapNamedBuffer(dst, GL_WRITE_ONLY);
	edge = (int*)glMapNamedBuffer(src, GL_READ_ONLY);
	mark = (int*)glMapNamedBuffer(SSBOtree, GL_READ_WRITE);
	stak = (int*)glMapNamedBuffer(SSBOdist, GL_READ_WRITE);

	while (true){
		
		// nearby node loc
		p8[0] = pick + 1;
		p8[1] = pick + RawDim[1] + 1;
		p8[2] = pick + RawDim[1];
		p8[3] = pick + RawDim[1] - 1;
		p8[4] = pick - 1;
		p8[5] = pick - RawDim[1] - 1;
		p8[6] = pick - RawDim[1];
		p8[7] = pick - RawDim[1] + 1;

		// hub computation
		//	 0: null
		//	 1: end node
		//	>1: hub
		hub = 0;
		way = -1;
		delta = -1;
		delta_tmp = -1;
		for (int i = 0; i < 8; ++i) {
			if (
				((edge[pick] >> i) & 1) == 1 && 
				((mark[pick] >> i) & 1) == 0
			){
				ploc[0] = float(p8[i] % RawDim[1]);
				ploc[1] = (float(p8[i]) - ploc[0]) / float(RawDim[0]);
				
				delta_tmp = sqrtf(
					(ploc[0] - eloc[0])*(ploc[0] - eloc[0]) +
					(ploc[1] - eloc[1])*(ploc[1] - eloc[1]));
				
				if (delta < 0 || delta_tmp < delta) {
					way = i;
					delta = delta_tmp;
				}
				hub++;
			}
		}

		// pick node update
		path[curp++] = pick;

		// break: circle complete
		if (way != -1 && p8[way] == end) {
			path[curp++] = p8[way];
			break;
		}
		// push hub into stack
		if (hub > 1) {
			stak[curs++] = curp - 1;
		}
		// pop hub from the stack
		if (hub == 0 && curs != 0) {
			curp = stak[--curs];
			pick = path[curp++];
			continue;
		}
		// break: no hub, no way
		else if (hub == 0 && curs == 0) {
			break;
		}
		mark[pick] |= 1 << way;
		mark[p8[way]] |= 1 << ((way + 4) % 8);
		pick = p8[way];
	}

	glUnmapNamedBuffer(dst);
	glUnmapNamedBuffer(src);
	glUnmapNamedBuffer(SSBOtree);
	glUnmapNamedBuffer(SSBOdist);
	return curp;
}

int GGDFScirclePath(GLuint dst, GLuint src, GLuint support, int start){
	GGClean(dst, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(SSBOtree, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(SSBOdist, 0, __GCODEGEN_IMG_SIZ__);

	int *path = NULL;
	int *mark = NULL;
	int *stak = NULL;
	int *edge = NULL;
	int *supo = NULL;

	int pick = start;
	int end = start;
	float ploc[2] = { 0, 0 };
	float eloc[2] = { 0, 0 };
	int p8[8];
	int curp = 0;
	int curs = 0;
	int hub = 0;
	int way = -1;
	int spo = -1;
	float delta = -1;
	float delta_tmp = -1;

	eloc[0] = float(end % RawDim[1]);
	eloc[1] = (float(end) - eloc[0]) / float(RawDim[0]);

	path = (int*)glMapNamedBuffer(dst, GL_WRITE_ONLY);
	edge = (int*)glMapNamedBuffer(src, GL_READ_ONLY);
	mark = (int*)glMapNamedBuffer(SSBOtree, GL_READ_WRITE);
	stak = (int*)glMapNamedBuffer(SSBOdist, GL_READ_WRITE);
	supo = (int*)glMapNamedBuffer(support, GL_READ_ONLY);

	while (true){

		// nearby node loc
		p8[0] = pick + 1;
		p8[1] = pick + RawDim[1] + 1;
		p8[2] = pick + RawDim[1];
		p8[3] = pick + RawDim[1] - 1;
		p8[4] = pick - 1;
		p8[5] = pick - RawDim[1] - 1;
		p8[6] = pick - RawDim[1];
		p8[7] = pick - RawDim[1] + 1;

		// hub computation
		//	 0: null
		//	 1: end node
		//	>1: hub
		hub = 0;
		way = -1;
		delta = -1;
		delta_tmp = -1;
		for (int i = 0; i < 8; ++i) {
			if (
				((edge[pick] >> i) & 1) == 1 &&
				((mark[pick] >> i) & 1) == 0
				){
				ploc[0] = float(p8[i] % RawDim[1]);
				ploc[1] = (float(p8[i]) - ploc[0]) / float(RawDim[0]);

				delta_tmp = sqrtf(
					(ploc[0] - eloc[0])*(ploc[0] - eloc[0]) +
					(ploc[1] - eloc[1])*(ploc[1] - eloc[1]));

				if (delta < 0 || delta_tmp < delta) {
					way = i;
					delta = delta_tmp;
				}
				hub++;
			}
		}

		// pick node update
		path[curp++] = pick;

		// break: circle complete
		if (p8[way] == end) {
			for (int i = 0; i < curp; ++i){
				if (supo[path[curp]] != 0){
					spo = i;
					break;
				}
			}
			if (spo == 0 || spo == -1){
				path[curp++] = pick;
			}else{
				for (int i = 0; i < curp; ++i){
					stak[i] = path[(i + spo) % curp];
				}
				for (int i = 0; i < curp; ++i){
					path[i] = stak[i];
				}
				path[curp++] = path[0];
			}
			break;
		}
		// push hub into stack
		if (hub > 1) {
			stak[curs++] = curp - 1;
		}
		// pop hub from the stack
		if (hub == 0 && curs != 0) {
			curp = stak[--curs];
			pick = path[curp++];
			continue;
		}
		// break: no hub, no way
		else if (hub == 0 && curs == 0) {
			break;
		}

		mark[pick] |= 1 << way;
		mark[p8[way]] |= 1 << ((way + 4) % 8);
		pick = p8[way];
	}

	glUnmapNamedBuffer(dst);
	glUnmapNamedBuffer(src);
	glUnmapNamedBuffer(SSBOtree);
	glUnmapNamedBuffer(SSBOdist);
	glUnmapNamedBuffer(SSBOsup);
	return curp;
}

///4. Dijkstra path///
int GGDijkstraPath(GLuint dst, GLuint src, int start, int end){
	GGClean(dst, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(SSBOdist, 99999999, __GCODEGEN_IMG_SIZ__);

	glUseProgram(shaderDijkstra);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, src);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBOdist);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBOpick);

	int *path = NULL;
	int *dist = NULL;
	int *pick = NULL;

	int p8[8];
	int id = 0;
	int curp = 0;
	int way = -1;
	int level = 0;
	int target = -1;
	int target_way = -1;
	int target_level = -1;

	// Dijkstra update dist
	dist = (int*)glMapNamedBuffer(SSBOdist, GL_READ_WRITE);
	dist[end] = 0;
	glUnmapNamedBuffer(SSBOdist);
	while (true){
		pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
		*pick = -1;
		glUnmapNamedBuffer(SSBOpick);

		glUniform1i(0, level++);
		glDispatchCompute(RawDim[0], RawDim[1], 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
		dist = (int*)glMapNamedBuffer(SSBOdist, GL_READ_WRITE);
		if (*pick == -1) { glUnmapNamedBuffer(SSBOpick); glUnmapNamedBuffer(SSBOdist); break; }
		else if (dist[start] == level) {  glUnmapNamedBuffer(SSBOpick); glUnmapNamedBuffer(SSBOdist); break; }
		glUnmapNamedBuffer(SSBOpick);
		glUnmapNamedBuffer(SSBOdist);
		
	}

	// return to sender
	path = (int*)glMapNamedBuffer(dst, GL_WRITE_ONLY);
	dist = (int*)glMapNamedBuffer(SSBOdist, GL_READ_WRITE);
	pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
	id = start;
	if (*pick != -1){
		while (id != end){

			p8[0] = id + 1;
			p8[1] = id + RawDim[1] + 1;
			p8[2] = id + RawDim[1];
			p8[3] = id + RawDim[1] - 1;
			p8[4] = id - 1;
			p8[5] = id - RawDim[1] - 1;
			p8[6] = id - RawDim[1];
			p8[7] = id - RawDim[1] + 1;
			
			target_level = dist[p8[0]];
			target = p8[0];
			
			way = 0;
			for (int i = 1; i < 8; ++i) {
				if (dist[p8[i]] < target_level) {
					way = i;
					target = p8[i];
					target_level = dist[p8[i]];
				}
			}
			if (target_way != way) {
				path[curp++] = id;
			}
			target_way = way;
			id = target;
		}
	}else if (*pick == -1){
		path[curp++] = start;
	}
	path[curp++] = end;
	glUnmapNamedBuffer(dst);
	glUnmapNamedBuffer(SSBOdist);
	glUnmapNamedBuffer(SSBOpick);
	
	return curp;
}

///5. GGfatEdge///
void GGfatEdge(int wall, int empty) {
	if (empty == 0) {
		GGClean(SSBOimg, 0, __GCODEGEN_IMG_SIZ__);
	}
	glUseProgram(shaderErosion);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBOraw);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBOimg);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBOTell);
	glUniform1i(0, wall);
	glUniform1i(1, empty);
	glDispatchCompute(RawDim[0], RawDim[1], 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
}

///6. GGInfection///
int GGInfection(int target, int start){
	GGClean(SSBOcom, 0, __GCODEGEN_IMG_SIZ__);

	int seg = start;
	int seg_ret = 0;

	int* pick = NULL;
	int* raw = NULL;

	glUseProgram(shaderInfection);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBOraw);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBOcom);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBOpick);
	while (true) {
		pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
		*pick = -1;
		glUnmapNamedBuffer(SSBOpick);

		glUniform1i(0, 0);
		glUniform1i(1, target);
		glDispatchCompute(RawDim[0], RawDim[1], 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
		if (*pick == -1) { glUnmapNamedBuffer(SSBOpick); break; }

		raw = (int*)glMapNamedBuffer(SSBOraw, GL_WRITE_ONLY);
		raw[*pick] = seg;
		glUnmapNamedBuffer(SSBOpick);
		glUnmapNamedBuffer(SSBOraw);

		while (true) {
			glUniform1i(0, 1);
			glUniform1i(1, seg);
			glDispatchCompute(RawDim[0], RawDim[1], 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

			pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
			*pick = -1;
			glUnmapNamedBuffer(SSBOpick);

			glUniform1i(0, 2);
			glUniform1i(1, seg);
			glDispatchCompute(RawDim[0], RawDim[1], 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

			pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
			if (*pick == -1) { glUnmapNamedBuffer(SSBOpick); break; }
			glUnmapNamedBuffer(SSBOpick);
		}
		seg++;
		seg_ret++;
	}
	return seg_ret;
}

///7. GGcommit///
void GGcommit(int target, int commit) {
	glUseProgram(shaderCommit);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBOraw);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBOcom);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBOTell);
	glUniform1i(0, target);
	glUniform1i(1, commit);
	glDispatchCompute(RawDim[0], RawDim[1], 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

/*_______________________________________________________________________________
	GCode write function
	--------------------
	0. Edge stroke
	1. Fill stroke
_______________________________________________________________________________*/

///0. Edge stroke///
int GGEdgeStroke(GLuint dst, GLuint ref, int target, int start){
	GGClean(SSBOhide, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(SSBOpath_2, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(dst, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(ref, 0, __GCODEGEN_IMG_SIZ__);

	// construct spannong tree 
	GGFullConnectGraph(SSBOlble, SSBOraw, target);
	GGFullConnectGraph(SSBOfull, SSBOraw, -1);
	GGSpanningTree(SSBOedge, SSBOraw, target);
	glUseProgram(shaderNoMoreHidden);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBOedge);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBOlble);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBOhide);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, SSBOpick);

	// findding hidden edge
	glUniform1i(0, 0);
	glDispatchCompute(RawDim[0], RawDim[1], 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// NO MORE HIDDEN !!!
	int* _dst = NULL;
	int* _ref = NULL;
	int* _raw = NULL;
	int* pick = NULL;
	int* edge = NULL;
	int* hide = NULL;
	int* lble = NULL;
	int* path_0 = NULL;
	int* path_1 = NULL;
	int* path_2 = NULL;

	int p8_0[8];
	int p8_1[8];
	int rfnd = -1;
	int post = -1;
	int tur0 = -1;
	int tur1 = -1;
	int curp = 0;
	int hide_curp = -1;
	int poup = start;
	int cont = 0;
	int pre_step = -1;
	int pre_curp = -1;
	int pre_poup = -1;
	int fil_count = -1;

	int way = -1;
	int fil = 0;
	int target_way = -1;
	int target_fil = 0;

	// fur handling
	while (true) {

		// I FOUND YOU !!!
		pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
		*pick = -1;
		glUnmapNamedBuffer(SSBOpick);

		glUseProgram(shaderNoMoreHidden);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBOedge);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBOlble);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBOhide);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, SSBOpick);
		glUniform1i(0, 1);
		glDispatchCompute(RawDim[0], RawDim[1], 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// NO MORE LOST CHILD TuT
		pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
		if (*pick == -1) { glUnmapNamedBuffer(SSBOpick); break; }
		curp = *pick;
		glUnmapNamedBuffer(SSBOpick);
		lble = (int*)glMapNamedBuffer(SSBOlble, GL_READ_WRITE);
		lble[curp] = 0;
		glUnmapNamedBuffer(SSBOlble);


		// FIND LOSD CHILD
		tur1 = GGDijkstraPath(SSBOpath_1, SSBOfull, poup, curp);
		path_1 = (int*)glMapNamedBuffer(SSBOpath_1, GL_READ_WRITE);
		_dst = (int*)glMapNamedBuffer(dst, GL_WRITE_ONLY);
		for (int i = 0; i < tur1; ++i) {
			_dst[cont++] = path_1[i];
			poup = path_1[i];
		}
		glUnmapNamedBuffer(SSBOpath_1);
		glUnmapNamedBuffer(dst);
		
		// FIND HOME
		_raw = (int*)glMapNamedBuffer(SSBOraw, GL_READ_ONLY);
		post = -1;
		while (true) {
			rfnd = 0;
			way = -1;
			p8_0[0] = curp + 1;
			p8_0[1] = curp + RawDim[1] + 1;
			p8_0[2] = curp + RawDim[1];
			p8_0[3] = curp + RawDim[1] - 1;
			p8_0[4] = curp - 1;
			p8_0[5] = curp - RawDim[1] - 1;
			p8_0[6] = curp - RawDim[1];
			p8_0[7] = curp - RawDim[1] + 1;
			for (int j = 0; j < 8; ++j) {
				if (_raw[p8_0[j]] == target && p8_0[j] != post) {
					rfnd++;
					way = j;
				}
			}
			if (rfnd == 1) {
				post = curp;
				curp = p8_0[way];
			}else if (rfnd == 0 || rfnd > 0) {
				break;
			}
		}
		glUnmapNamedBuffer(SSBOraw);

		// TAKE YOU HOME
		tur0 = GGDFSPath(SSBOpath_0, SSBOedge, poup, curp);
		_dst = (int*)glMapNamedBuffer(dst, GL_WRITE_ONLY);
		_ref = (int*)glMapNamedBuffer(ref, GL_WRITE_ONLY);
		path_0 = (int*)glMapNamedBuffer(SSBOpath_0, GL_READ_WRITE);
		path_2 = (int*)glMapNamedBuffer(SSBOpath_2, GL_READ_WRITE);

		way = -1;
		fil = 0;
		target_way = -1;
		target_fil = 0;
		for (int i = 0; i < tur0; ++i) {
			p8_1[0] = path_0[i] + 1;
			p8_1[1] = path_0[i] + RawDim[1] + 1;
			p8_1[2] = path_0[i] + RawDim[1];
			p8_1[3] = path_0[i] + RawDim[1] - 1;
			p8_1[4] = path_0[i] - 1;
			p8_1[5] = path_0[i] - RawDim[1] - 1;
			p8_1[6] = path_0[i] - RawDim[1];
			p8_1[7] = path_0[i] - RawDim[1] + 1;

			if (tur0 > 15) {
				for (int j = 0; j < 8; ++j) {
					if (p8_1[j] == curp) {
						way = j;
						break;
					}
				}
				if (((path_2[path_0[i]] >> way) & 1) == 0 && way != -1) {
					path_2[path_0[i]] |= 1 << way;
					path_2[p8_1[way]] |= 1 << ((way + 4) % 8);
					fil = 1;
					if (fil == target_fil && way != target_way) {
						_dst[cont] = curp;
						_ref[cont++] = 1;
					}
					else if (fil != target_fil) {
						_dst[cont++] = curp;
					}
				}
				else if (((path_2[path_0[i]] >> way) & 1) == 1 && way != -1) {
					fil = -1;
					if (fil == target_fil && way != target_way) {
						_dst[cont++] = curp;
					}
					else if (fil != target_fil) {
						_dst[cont] = curp;
						_ref[cont++] = 1;
					}
				}
				poup = curp;
				curp = path_0[i];
				target_way = way;
				target_fil = fil;
			}
		}
		if (fil == 1) {
			if (fil == target_fil && way == target_way) {
				_dst[cont] = curp;
				_ref[cont++] = 1;
			}
		}
		else if (fil == -1) {
			if (fil == target_fil && way == target_way) {
				_dst[cont++] = curp;
			}
		}

		glUnmapNamedBuffer(dst);
		glUnmapNamedBuffer(ref);
		glUnmapNamedBuffer(SSBOpath_0);
		glUnmapNamedBuffer(SSBOpath_2);
	}

	// Cycle handling
	while (true){
		
		// I FOUND YOU !!!
		pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
		*pick = -1;
		glUnmapNamedBuffer(SSBOpick);

		glUseProgram(shaderNoMoreHidden);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBOedge);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBOlble);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBOhide);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, SSBOpick);
		glUniform1i(0, 2);
		glDispatchCompute(RawDim[0], RawDim[1], 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// ANNIHILATION COMPLETE !!!
		pick = (int*)glMapNamedBuffer(SSBOpick, GL_READ_WRITE);
		if (*pick == -1) { glUnmapNamedBuffer(SSBOpick); break; }

		// I MARK YOU !!!
		edge = (int*)glMapNamedBuffer(SSBOedge, GL_READ_WRITE);
		hide = (int*)glMapNamedBuffer(SSBOhide, GL_READ_WRITE);
		curp = *pick;
		p8_0[0] = curp + 1;
		p8_0[1] = curp + RawDim[1] + 1;
		p8_0[2] = curp + RawDim[1];
		p8_0[3] = curp + RawDim[1] - 1;
		p8_0[4] = curp - 1;
		p8_0[5] = curp - RawDim[1] - 1;
		p8_0[6] = curp - RawDim[1];
		p8_0[7] = curp - RawDim[1] + 1;
		hide_curp = -1;
		rfnd = -1;
		for (int i = 0; i < 8; ++i) {
			if ((hide[*pick] >> i) & 1 == 1) {
				rfnd = i;
				hide_curp = curp;
				edge[*pick] |= 1 << i;
				hide[*pick] &= ~(1 << i);
				edge[p8_0[i]] |= 1 << ((i + 4) % 8);
				hide[p8_0[i]] &= ~(1 << ((i + 4) % 8));
				break;
			}
		}

		// find path 
		pre_step = cont;
		pre_curp = curp;
		pre_poup = poup;
		fil_count = 0;
		glUnmapNamedBuffer(SSBOpick);
		glUnmapNamedBuffer(SSBOedge);
		glUnmapNamedBuffer(SSBOhide);

		tur0 = GGDFScirclePath(SSBOpath_0, SSBOedge, SSBOsup, curp);
		path_0 = (int*)glMapNamedBuffer(SSBOpath_0, GL_READ_ONLY);
		curp = path_0[0];
		glUnmapNamedBuffer(SSBOpath_0);

		if (tur0 > 5){
			tur1 = GGDijkstraPath(SSBOpath_1, SSBOfull, poup, curp);
			path_1 = (int*)glMapNamedBuffer(SSBOpath_1, GL_READ_WRITE);
			_dst = (int*)glMapNamedBuffer(dst, GL_WRITE_ONLY);
			for (int i = 0; i < tur1; ++i){
				_dst[cont++] = path_1[i];
				poup = path_1[i];
			}
			glUnmapNamedBuffer(SSBOpath_1);
			glUnmapNamedBuffer(dst);
		}

		_dst = (int*)glMapNamedBuffer(dst, GL_WRITE_ONLY);
		_ref = (int*)glMapNamedBuffer(ref, GL_WRITE_ONLY);
		edge = (int*)glMapNamedBuffer(SSBOedge, GL_READ_WRITE);
		path_0 = (int*)glMapNamedBuffer(SSBOpath_0, GL_READ_WRITE);
		path_2 = (int*)glMapNamedBuffer(SSBOpath_2, GL_READ_WRITE);

		way = -1;
		fil = 0;
		target_way = -1;
		target_fil = 0;
		for (int i = 0; i < tur0; ++i){
			p8_1[0] = path_0[i] + 1;
			p8_1[1] = path_0[i] + RawDim[1] + 1;
			p8_1[2] = path_0[i] + RawDim[1];
			p8_1[3] = path_0[i] + RawDim[1] - 1;
			p8_1[4] = path_0[i] - 1;
			p8_1[5] = path_0[i] - RawDim[1] - 1;
			p8_1[6] = path_0[i] - RawDim[1];
			p8_1[7] = path_0[i] - RawDim[1] + 1;

			if (tur0 > 5){
				for (int j = 0; j < 8; ++j) {
					if (p8_1[j] == curp) {
						way = j;
						break;
					}
				}
				if (((path_2[path_0[i]] >> way) & 1) == 0 && way != -1) {
					fil_count++;
					path_2[path_0[i]] |= 1 << way;
					path_2[p8_1[way]] |= 1 << ((way + 4) % 8);
					fil = 1;
					if (fil == target_fil && way != target_way) {
						_dst[cont] = curp;
						_ref[cont++] = 1;
					}else if (fil != target_fil) {
						_dst[cont++] = curp;
					}
				}else if (((path_2[path_0[i]] >> way) & 1) == 1 && way != -1){
					fil = -1;
					if (fil == target_fil && way != target_way) {
						_dst[cont++] = curp;
					}else if (fil != target_fil) {
						_dst[cont] = curp;
						_ref[cont++] = 1;
					}
				}
				poup = curp;
				curp = path_0[i];
				target_way = way;
				target_fil = fil;
			}
		}
		if (fil == 1) {
			if (fil == target_fil && way == target_way) {
				_dst[cont] = curp;
				_ref[cont++] = 1;
			}
		}else if (fil == -1) {
			if (fil == target_fil && way == target_way) {
				_dst[cont++] = curp;
			}
		}
		edge[hide_curp] &= ~(1 << rfnd);
		edge[p8_0[rfnd]] &= ~(1 << ((rfnd + 4) % 8));

		if (fil_count <= 4) {
			cont = pre_step;
			curp = pre_curp;
			poup = pre_poup;
		}

		glUnmapNamedBuffer(dst);
		glUnmapNamedBuffer(ref);
		glUnmapNamedBuffer(SSBOedge);
		glUnmapNamedBuffer(SSBOpath_0);
		glUnmapNamedBuffer(SSBOpath_2);
	}

	return cont;
}

///1. Fill stroke///
int GGFillStroke(GLuint dst, GLuint ref, int target, int wall, int start, int fdir){
	GGClean(SSBOpath_0, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(dst, 0, __GCODEGEN_IMG_SIZ__);
	GGClean(ref, 0, __GCODEGEN_IMG_SIZ__);
	GGFullConnectGraph(SSBOlble, SSBOraw, target);
	GGFullConnectGraph(SSBOfull, SSBOraw, -1);

	int* _dst = NULL;
	int* _ref = NULL;
	int* raw = NULL;
	int* proj = NULL;
	int* path_1 = NULL;

	// Dead Ray Tracing
	glUseProgram(shaderRayTrace);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBOlble);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBOpath_0);
	glUniform2f(0, float(RawDim[0]), float(RawDim[1]));
	if (fdir == 0){ 
		glUniform1i(1, 0);
		glDispatchCompute(RawDim[1], 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
	else{
		glUniform1i(1, 1);
		glDispatchCompute(RawDim[0], 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}


	// find path
	int s = 0;
	int e = 0;
	int empt = -1;
	int line = -1;
	int chse = -1;
	int tur0 = -1;
	int curp = -1;
	int poup = start;
	int cont = 0;
	int p9_0[9];
	int dirc = 1;

	if (fdir == 0){
		e = RawDim[1] - 1;
	}
	else{
		e = RawDim[0] - 1;
	}

	_dst = (int*)glMapNamedBuffer(dst, GL_WRITE_ONLY);
	_ref = (int*)glMapNamedBuffer(ref, GL_WRITE_ONLY);
	proj = (int*)glMapNamedBuffer(SSBOpath_0, GL_READ_WRITE);
	raw = (int*)glMapNamedBuffer(SSBOraw, GL_READ_WRITE);
	if (fdir == 0){
		while (s != e || (s == e && proj[s] >= 1)) {
			s = 0;
			e = RawDim[1] - 1;
			empt = -1;

			// locator
			while (proj[s] < 1 && s < RawDim[1] - 1) { ++s; };
			while (proj[e] < 1 && e > s) { --e; };
			line = s;
			if (proj[line] > 0) {
				for (int i = 0; i < RawDim[0] - 1; ++i) {
					curp = i*RawDim[1] + line;
					if (raw[curp] == target) {
						empt = 1;
						break;
					}
				}
			}
			if (empt == -1 && s == e) {
				break;
			}
			if (empt == -1 && proj[line] > 0) {
				proj[line]--;
				continue;
			}



			// Dijkstra walking
			tur0 = GGDijkstraPath(SSBOpath_1, SSBOfull, poup, curp);
			path_1 = (int*)glMapNamedBuffer(SSBOpath_1, GL_READ_WRITE);
			for (int i = 0; i < tur0; ++i){
				curp = path_1[i];
				_dst[cont++] = path_1[i];
				poup = path_1[i];
			}
			glUnmapNamedBuffer(SSBOpath_1);
			/*
			[8][5][7][0][1][3][4]
			[c][c][6][x][2]
			*/
			// scanline walking
			while (true) {
				raw[curp] = target + 1;
				chse = -1;
				p9_0[0] = curp + 1;
				p9_0[1] = curp + RawDim[1] + 1;
				p9_0[2] = curp + RawDim[1];
				p9_0[3] = curp + 2 * RawDim[1] + 1;
				p9_0[4] = curp + 3 * RawDim[1] + 1;
				p9_0[5] = curp - 2 * RawDim[1] + 1;
				p9_0[6] = curp - RawDim[1];
				p9_0[7] = curp - RawDim[1] + 1;
				p9_0[8] = curp - 3 * RawDim[1] + 1;

				if (dirc == 1 && raw[p9_0[2]] == target) { chse = 2; }
				else if (dirc == -1 && raw[p9_0[6]] == target) { chse = 6; }
				else if (dirc == 1 && (raw[p9_0[2]] == wall || raw[p9_0[2]] == target + 1)) {
					if (raw[p9_0[6]] == target) { dirc = -1; chse = 6; }
					else if (raw[p9_0[4]] == target) { chse = 4; }
					else if (raw[p9_0[3]] == target) { chse = 3; }
					else if (raw[p9_0[1]] == target) { chse = 1; }
					else if (raw[p9_0[0]] == target) { chse = 0; }
					else if (raw[p9_0[7]] == target) { chse = 7; }
					else if (raw[p9_0[5]] == target) { chse = 5; }
					else if (raw[p9_0[8]] == target) { chse = 8; }
					if (chse != -1 && chse != 6) {
						proj[line++] -= 1;
						if (raw[p9_0[chse] + RawDim[1]] != target) { proj[line] -= 1; }
						if (raw[p9_0[chse] - RawDim[1]] == target) { proj[line] += 1; }
						_dst[cont] = curp;
						_ref[cont++] = 1;
						_dst[cont++] = p9_0[chse];
						poup = p9_0[chse];
						dirc *= -1;
					}
				}
				else if (dirc == -1 && (raw[p9_0[6]] == wall || raw[p9_0[6]] == target + 1)) {
					if (raw[p9_0[2]] == target) { dirc = 1; chse = 2; }
					else if (raw[p9_0[8]] == target) { chse = 8; }
					else if (raw[p9_0[5]] == target) { chse = 5; }
					else if (raw[p9_0[7]] == target) { chse = 7; }
					else if (raw[p9_0[0]] == target) { chse = 0; }
					else if (raw[p9_0[1]] == target) { chse = 1; }
					else if (raw[p9_0[3]] == target) { chse = 3; }
					else if (raw[p9_0[4]] == target) { chse = 4; }
					if (chse != -1 && chse != 2) {
						proj[line++] -= 1;
						if (raw[p9_0[chse] - RawDim[1]] != target) { proj[line] -= 1; }
						if (raw[p9_0[chse] + RawDim[1]] == target) { proj[line] += 1; }
						_dst[cont] = curp;
						_ref[cont++] = 1;
						_dst[cont++] = p9_0[chse];
						poup = p9_0[chse];
						dirc *= -1;
					}
				}

				if (chse == -1) {
					if (raw[p9_0[2]] != target && raw[p9_0[6]] != target) { proj[line] -= 1; }
					_dst[cont] = curp;
					_ref[cont++] = 1;
					poup = curp;
					break;
				}
				curp = p9_0[chse];
			}
		}
	}else{
		while (s != e || (s == e && proj[s] >= 1)) {
			s = 0;
			e = RawDim[0] - 1;
			empt = -1;

			// locator
			while (proj[s] < 1 && s < RawDim[0] - 1) { ++s; };
			while (proj[e] < 1 && e > s) { --e; };
			line = s;
			if (proj[line] > 0) {
				for (int i = 0; i < RawDim[1] - 1; ++i) {
					curp = line*RawDim[1] + i;
					if (raw[curp] == target) {
						empt = 1;
						break;
					}
				}
			}
			if (empt == -1 && s == e) {
				break;
			}
			if (empt == -1 && proj[line] > 0) {
				proj[line]--;
				continue;
			}


			// Dijkstra walking
			tur0 = GGDijkstraPath(SSBOpath_1, SSBOfull, poup, curp);
			path_1 = (int*)glMapNamedBuffer(SSBOpath_1, GL_READ_WRITE);
			for (int i = 0; i < tur0; ++i){
				curp = path_1[i];
				_dst[cont++] = path_1[i];
				poup = path_1[i];
			}
			glUnmapNamedBuffer(SSBOpath_1);
			/*
			[c][4]
			[c][3]
			[2][1]
			[x][0]
			[6][7]
			[C][5]
			[C][8]
			*/
			// scanline walking
			while (true) {
				raw[curp] = target + 1;
				chse = -1;
				p9_0[0] = curp + RawDim[1];
				p9_0[1] = curp + RawDim[1] + 1;
				p9_0[2] = curp + 1;
				p9_0[3] = curp + RawDim[1] + 2;
				p9_0[4] = curp + RawDim[1] + 3;
				p9_0[5] = curp + RawDim[1] - 2;
				p9_0[6] = curp - 1;
				p9_0[7] = curp + RawDim[1] - 1;
				p9_0[8] = curp + RawDim[1] - 3;

				if (dirc == 1 && raw[p9_0[2]] == target) { chse = 2; }
				else if (dirc == -1 && raw[p9_0[6]] == target) { chse = 6; }
				else if (dirc == 1 && (raw[p9_0[2]] == wall || raw[p9_0[2]] == target + 1)) {
					if (raw[p9_0[6]] == target) { dirc = -1; chse = 6; }
					else if (raw[p9_0[4]] == target) { chse = 4; }
					else if (raw[p9_0[3]] == target) { chse = 3; }
					else if (raw[p9_0[1]] == target) { chse = 1; }
					else if (raw[p9_0[0]] == target) { chse = 0; }
					else if (raw[p9_0[7]] == target) { chse = 7; }
					else if (raw[p9_0[5]] == target) { chse = 5; }
					else if (raw[p9_0[8]] == target) { chse = 8; }
					if (chse != -1 && chse != 6) {
						proj[line++] -= 1;
						if (raw[p9_0[chse] + 1] != target) { proj[line] -= 1; }
						if (raw[p9_0[chse] - 1] == target) { proj[line] += 1; }
						_dst[cont] = curp;
						_ref[cont++] = 1;
						_dst[cont++] = p9_0[chse];
						poup = p9_0[chse];
						dirc *= -1;
					}
				}
				else if (dirc == -1 && (raw[p9_0[6]] == wall || raw[p9_0[6]] == target + 1)) {
					if (raw[p9_0[2]] == target) { dirc = 1; chse = 2; }
					else if (raw[p9_0[8]] == target) { chse = 8; }
					else if (raw[p9_0[5]] == target) { chse = 5; }
					else if (raw[p9_0[7]] == target) { chse = 7; }
					else if (raw[p9_0[0]] == target) { chse = 0; }
					else if (raw[p9_0[1]] == target) { chse = 1; }
					else if (raw[p9_0[3]] == target) { chse = 3; }
					else if (raw[p9_0[4]] == target) { chse = 4; }
					if (chse != -1 && chse != 2) {
						proj[line++] -= 1;
						if (raw[p9_0[chse] - 1] != target) { proj[line] -= 1; }
						if (raw[p9_0[chse] + 1] == target) { proj[line] += 1; }
						_dst[cont] = curp;
						_ref[cont++] = 1;
						_dst[cont++] = p9_0[chse];
						poup = p9_0[chse];
						dirc *= -1;
					}
				}

				if (chse == -1) {
					if (raw[p9_0[2]] != target && raw[p9_0[6]] != target) { proj[line] -= 1; }
					_dst[cont] = curp;
					_ref[cont++] = 1;
					poup = curp;
					break;
				}
				curp = p9_0[chse];
			}
		}
	}

	glUnmapNamedBuffer(dst);
	glUnmapNamedBuffer(ref);
	glUnmapNamedBuffer(SSBOraw);
	glUnmapNamedBuffer(SSBOpath_0);
	return cont;
}

/*_______________________________________________________________________________
	main function
	-------------
	0. init
	1. exit
	2. main
_______________________________________________________________________________*/

/// machine setting
/*
float ggmlh = 0.3f;
float ggmcx = 100.0f;
float ggmcy = 100.0f;
int   ggmfs = 900;
int   ggmfr = 9000;
float ggmfo = 0.048f;
int   ggm0s = 900;
int   ggm0r = 9000;
float ggm0o = 0.048f;
int   ggm1s = 900;
int   ggm1r = 9000;
float ggm1o = 0.048f;
int   ggm2s = 900;
int   ggm2r = 9000;
float ggm2o = 0.048f;
float ggmxr = 0.45f;
float ggmyr = 0.45f;
*/
float ggFmachine[36];
int   ggImachine[36];




void GGMachine(int conf, float value) {
	ggFmachine[conf] = value;
}

void GGMachine(int conf, int value) {
	ggImachine[conf] = value;
}

void GGMachineEoutput(int type, float* curl, float* poul,  int curp) {
	float delt = 0;
	curl[0] = float(curp % RawDim[1]) - ggFmachine[9];
	curl[1] = (float(curp) - curl[0] + ggFmachine[9]) / float(RawDim[1]) - ggFmachine[10];
	curl[0] *= ggFmachine[5];
	curl[1] *= ggFmachine[6];
	delt = sqrtf((curl[0] - poul[0])*(curl[0] - poul[0]) + (curl[1] - poul[1])*(curl[1] - poul[1]));
	curl[3] = delt;
	if (type == 1) {
		curl[2] = ggFmachine[5] * ggFmachine[7] * delt / (3.1415926f *  ggFmachine[8] * ggFmachine[8]);
	}
	poul[0] = curl[0];
	poul[1] = curl[1];
}


GLint GGShaderCreate(const char* _shaderStream, const char* filepath) {

	FILE* GGsysShader = NULL;
	void* GGsysBinary = NULL;
	GLint GGsysLength;
	GLint GGsysFormat;
	GLenum GGsysFormat_e;
	int _shaderLogLenght;
	char _shaderLog[4096];
	GLint _shader;
	GLint _program;
	GLint success;

	_shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(_shader, 1, &_shaderStream, NULL);
	glCompileShader(_shader);

	glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(_shader, 4096, &_shaderLogLenght, _shaderLog);
		printf("%s\n", _shaderLog);
	}

	if (success) {
		_program = glCreateProgram();
		glAttachShader(_program, _shader);
		glProgramParameteri(_program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
		glLinkProgram(_program);
	}

	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(_program, 4096, &_shaderLogLenght, _shaderLog);
		printf("%s\n", _shaderLog);
	}

	if (filepath != NULL && success) {
		glGetProgramiv(_program, GL_PROGRAM_BINARY_LENGTH, &GGsysLength);
		glGetProgramiv(_program, GL_PROGRAM_BINARY_FORMATS, &GGsysFormat);
		GGsysFormat_e = GGsysFormat;
		GGsysBinary = (void*)malloc(GGsysLength);
		glGetProgramBinary(_program, GGsysLength, NULL, &GGsysFormat_e, GGsysBinary);

		fopen_s(&GGsysShader, filepath, "wb");
		fwrite(GGsysBinary, GGsysLength, 1, GGsysShader);
		fwrite(&GGsysFormat_e, sizeof(GLenum), 1, GGsysShader);
		fclose(GGsysShader);
		free(GGsysBinary);
	}

	return _program;
}

GLint GGShaderLoad(const char* filepath) {
	FILE* GGsysShader = NULL;
	void* GGsysBinary = NULL;
	GLint GGsysLength;
	GLenum GGsysFormat_e;
	GLint _shader = -1;

	fopen_s(&GGsysShader, filepath, "rb");
	fseek(GGsysShader, 0, SEEK_END);
	GGsysLength = (GLint)ftell(GGsysShader) - sizeof(GLenum);
	GGsysBinary = (void*)malloc(GGsysLength);
	fseek(GGsysShader, 0, SEEK_SET);
	fread_s(GGsysBinary, GGsysLength, GGsysLength, 1, GGsysShader);
	fread_s(&GGsysFormat_e, sizeof(GLenum), sizeof(GLenum), 1, GGsysShader);
	fclose(GGsysShader);

	_shader = glCreateProgram();
	glProgramBinary(_shader, GGsysFormat_e, GGsysBinary, GGsysLength);
	free(GGsysBinary);

	return _shader;
}

///0. init///
void GGInit(void){
	FILE* GGsysConfig = NULL;
	char  GGsysFlg[0x10];

	//fopen_s(&GGsysConfig, "sysConfig.bin", "rb");
	//fread_s(GGsysFlg, 0x10, 1, 16, GGsysConfig);
	//fclose(GGsysConfig);

	// --------------- general buffer storage --------------- 
	
	// ort buffer section 1
	glGenBuffers(1, &SSBOraw);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOraw);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOfull);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOfull);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOlble);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOlble);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOedge);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOedge);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOhide);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOhide);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	// ort buffer section 2
	glGenBuffers(1, &SSBOpick);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOpick);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int), NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOtree);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOtree);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOdist);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOdist);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOwalk);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOwalk);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOgtyp);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOgtyp);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	// ort buffer section 3
	glGenBuffers(1, &SSBOpath_0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOpath_0);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOpath_1);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOpath_1);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOpath_2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOpath_2);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOimg);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOimg);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOcom);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOcom);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOsup);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOsup);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &SSBOTell);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBOTell);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*__GCODEGEN_IMG_SIZ__, NULL, GL_DYNAMIC_COPY);

	shaderClean = GGShaderCreate(textClean, "shaderClean.bin");
	shaderFullConnectGraph = GGShaderCreate(textFullConnectGraph, "shaderFullConnectGraph.bin");
	shaderSpanningTree = GGShaderCreate(textSpanningTree, "shaderSpanningTree.bin");
	shaderDijkstra = GGShaderCreate(textDijkstra, "shaderDijkstra.bin");
	shaderNoMoreHidden = GGShaderCreate(textNoMoreHidden, "shaderNoMoreHidden.bin");
	shaderRayTrace = GGShaderCreate(textRayTrace, "shaderRayTrace.bin");
	shaderErosion = GGShaderCreate(textErosion, "shaderErosion.bin");
	shaderInfection = GGShaderCreate(textInfection, "shaderInfection.bin");
	shaderCommit = GGShaderCreate(textCommit, "shaderCommit.bin");
	GGsysFlg[0] = 1;
	//fopen_s(&GGsysConfig, "sysConfig.bin", "wb");
	//fwrite(GGsysFlg, sizeof(char), 16, GGsysConfig);
	//fclose(GGsysConfig);
	/*
	if (GGsysFlg[0] == 0) {
		shaderClean = GGShaderCreate(textClean, "shaderClean.bin");
		shaderFullConnectGraph = GGShaderCreate(textFullConnectGraph, "shaderFullConnectGraph.bin");
		shaderSpanningTree = GGShaderCreate(textSpanningTree, "shaderSpanningTree.bin");
		shaderDijkstra = GGShaderCreate(textDijkstra, "shaderDijkstra.bin");
		shaderNoMoreHidden = GGShaderCreate(textNoMoreHidden, "shaderNoMoreHidden.bin");
		shaderRayTrace = GGShaderCreate(textRayTrace, "shaderRayTrace.bin");
		shaderErosion = GGShaderCreate(textErosion, "shaderErosion.bin");
		shaderInfection = GGShaderCreate(textInfection, "shaderInfection.bin");
		shaderCommit = GGShaderCreate(textCommit, "shaderCommit.bin");
		GGsysFlg[0] = 1;
		fopen_s(&GGsysConfig, "sysConfig.bin", "wb");
		fwrite(GGsysFlg, sizeof(char), 16, GGsysConfig);
		fclose(GGsysConfig);
	}else if (GGsysFlg[0] == 1) {
		shaderClean = GGShaderLoad("shaderClean.bin");
		shaderFullConnectGraph = GGShaderLoad("shaderFullConnectGraph.bin");
		shaderSpanningTree = GGShaderLoad("shaderSpanningTree.bin");
		shaderDijkstra = GGShaderLoad("shaderDijkstra.bin");
		shaderNoMoreHidden = GGShaderLoad("shaderNoMoreHidden.bin");
		shaderRayTrace = GGShaderLoad("shaderRayTrace.bin");
		shaderErosion = GGShaderLoad("shaderErosion.bin");
		shaderInfection = GGShaderLoad("shaderInfection.bin");
		shaderCommit = GGShaderLoad("shaderCommit.bin");
	}
	*/
	// machine setting
	ggFmachine[0] = 210.0f;
	ggFmachine[1] = 100;
	ggFmachine[2] = 100;
	ggFmachine[3] = 100;
	ggFmachine[4] = 0.4f;
	ggFmachine[5] = 0.45f;
	ggFmachine[6] = 0.45f;
	ggFmachine[7] = 0.3f;
	ggFmachine[8] = 1.75f;
	ggFmachine[9] = 0.0f;
	ggFmachine[10] = 0.0f;

	ggImachine[0] = 9000;
	ggImachine[1] = 900;
	ggImachine[2] = 3;
	ggImachine[3] = 9000;
	ggImachine[4] = 900;
	ggImachine[5] = 9000;
	ggImachine[6] = 900;
	ggImachine[7] = 9000;
	ggImachine[8] = 900;
	ggImachine[9] = 9000;
	ggImachine[10] = 900;
	ggImachine[11] = 9000;
	ggImachine[12] = 900;
	ggImachine[13] = 9000;
	ggImachine[14] = 900;
}

///1. exit///
void GGExit(void){
	glDeleteShader(shaderClean);
	glDeleteShader(shaderDijkstra);
	glDeleteShader(shaderFullConnectGraph);
	glDeleteShader(shaderNoMoreHidden);
	glDeleteShader(shaderRayTrace);
	glDeleteShader(shaderSpanningTree);
}

///2. main///
void GGMain(int &layer, int &start, float &fill, FILE* file_gcode){
	if (layer == 0){
		fprintf_s(file_gcode, ";---Start----------------------------------------\n");
		fprintf_s(file_gcode, "M109 S%d\n", ggFmachine[0]);
		fprintf_s(file_gcode, "M107\n");
		fprintf_s(file_gcode, "M082\n");
		fprintf_s(file_gcode, "G90 \n");
		fprintf_s(file_gcode, "G21 \n");
		fprintf_s(file_gcode,
			"G28 X0 Y0\n"
			"G28 Z0\n"
			"G92 E0\n"
			"G01 F200 E3\n"
			"G92 E0\n"
			"\n"
		);
		fprintf_s(file_gcode, ";---Print----------------------------------------\n");
	}
	
	int curp = 0;
	int poup = start;
	int tur0 = -1;
	int seg;
	float curl[4] = { 0, 0 ,0, 0};
	float poul[2] = { 0, 0 };
	float _fil = fill;
	float _z = float(layer)*ggFmachine[7] + ggFmachine[7];
	float delt = 0;
	
	GGClean(SSBOTell, 0, __GCODEGEN_IMG_SIZ__);

	GGClean(SSBOsup, 0, __GCODEGEN_IMG_SIZ__);
	glCopyNamedBufferSubData(SSBOcom, SSBOsup, 0, 0, sizeof(int)*__GCODEGEN_IMG_SIZ__);

	fprintf(file_gcode, "G00 F%d Z%f \n", ggImachine[0], _z);

	int* walk = NULL;
	int* gtyp = NULL;

	seg = GGInfection(255, 256);

	for (int seg_i = 0; seg_i < seg; seg_i++) {

		// commit change to raw
		GGcommit(256 + seg_i, 255);

		// wall outer
		GGfatEdge(128, 0);
		tur0 = GGEdgeStroke(SSBOwalk, SSBOgtyp, 128, poup);
		walk = (int*)glMapNamedBuffer(SSBOwalk, GL_READ_ONLY);
		gtyp = (int*)glMapNamedBuffer(SSBOgtyp, GL_READ_ONLY);

		for (int i = 0; i < tur0; ++i) {
			curp = walk[i];
			if (gtyp[i] == 0) {
				GGMachineEoutput(0, curl, poul, curp);
				if (curl[3] < ggFmachine[GG_MACHINE_F_VOX_X] * 2.0f) {
					fprintf(file_gcode, "G00 F%d X%f Y%f \n", ggImachine[9], curl[0], curl[1]);
				}
				else {
					fprintf(file_gcode, "G00 F%d X%f Y%f \n", ggImachine[3], curl[0], curl[1]);
				}
			}
			else if (gtyp[i] == 1) {
				GGMachineEoutput(1, curl, poul, curp);
				_fil += curl[2];
				fprintf(file_gcode, "G01 F%d X%f Y%f E%f \n", ggImachine[9], curl[0], curl[1], _fil);
			}
			poup = curp;
		}
		glUnmapNamedBuffer(SSBOwalk);
		glUnmapNamedBuffer(SSBOgtyp);

		// wall mid
		GGfatEdge(127, 128);
		tur0 = GGEdgeStroke(SSBOwalk, SSBOgtyp, 127, poup);
		walk = (int*)glMapNamedBuffer(SSBOwalk, GL_READ_ONLY);
		gtyp = (int*)glMapNamedBuffer(SSBOgtyp, GL_READ_ONLY);

		for (int i = 0; i < tur0; ++i) {
			curp = walk[i];
			if (gtyp[i] == 0) {
				GGMachineEoutput(0, curl, poul, curp);
				if (curl[3] < ggFmachine[GG_MACHINE_F_VOX_X] * 2.0f) {
					fprintf(file_gcode, "G00 F%d X%f Y%f \n", ggImachine[10], curl[0], curl[1]);
				}
				else {
					fprintf(file_gcode, "G00 F%d X%f Y%f \n", ggImachine[4], curl[0], curl[1]);
				}
			}
			else if (gtyp[i] == 1) {
				GGMachineEoutput(1, curl, poul, curp);
				_fil += curl[2];
				fprintf(file_gcode, "G01 F%d X%f Y%f E%f \n", ggImachine[10], curl[0], curl[1], _fil);
			}
			poup = curp;
		}

		glUnmapNamedBuffer(SSBOwalk);
		glUnmapNamedBuffer(SSBOgtyp);

		// fillment
		if ((layer % 2) == 0){
			tur0 = GGFillStroke(SSBOwalk, SSBOgtyp, 255, 127, curp, 0);
		}else{
			tur0 = GGFillStroke(SSBOwalk, SSBOgtyp, 255, 127, curp, 1);
		}
		

		walk = (int*)glMapNamedBuffer(SSBOwalk, GL_READ_ONLY);
		gtyp = (int*)glMapNamedBuffer(SSBOgtyp, GL_READ_ONLY);

		for (int i = 0; i < tur0; ++i) {
			curp = walk[i];
			if (gtyp[i] == 0) {
				GGMachineEoutput(0, curl, poul, curp);
				if (curl[3] < ggFmachine[GG_MACHINE_F_VOX_X] * 2.0f) {
					fprintf(file_gcode, "G00 F%d X%f Y%f \n", ggImachine[1], curl[0], curl[1]);
				}
				else {
					fprintf(file_gcode, "G00 F%d X%f Y%f \n", ggImachine[0], curl[0], curl[1]);
				}
			}
			else if (gtyp[i] == 1) {
				GGMachineEoutput(1, curl, poul, curp);
				_fil += curl[2];
				fprintf(file_gcode, "G01 F%d X%f Y%f E%f \n", ggImachine[1], curl[0], curl[1], _fil);
			}
			poup = curp;
		}

		glUnmapNamedBuffer(SSBOwalk);
		glUnmapNamedBuffer(SSBOgtyp);

		// wall inner
		GGfatEdge(126, 127);
		tur0 = GGEdgeStroke(SSBOwalk, SSBOgtyp, 126, poup);

		walk = (int*)glMapNamedBuffer(SSBOwalk, GL_READ_ONLY);
		gtyp = (int*)glMapNamedBuffer(SSBOgtyp, GL_READ_ONLY);

		for (int i = 0; i < tur0; ++i) {
			curp = walk[i];
			if (gtyp[i] == 0) {
				GGMachineEoutput(0, curl, poul, curp);
				if (curl[3] < ggFmachine[GG_MACHINE_F_VOX_X] * 2.0f) {
					fprintf(file_gcode, "G00 F%d X%f Y%f \n", ggImachine[11], curl[0], curl[1]);
				}
				else {
					fprintf(file_gcode, "G00 F%d X%f Y%f \n", ggImachine[5], curl[0], curl[1]);
				}
			}
			else if (gtyp[i] == 1) {
				GGMachineEoutput(1, curl, poul, curp);
				_fil += curl[2];
				fprintf(file_gcode, "G01 F%d X%f Y%f E%f \n", ggImachine[11], curl[0], curl[1], _fil);
			}
			poup = curp;
		}

		glUnmapNamedBuffer(SSBOwalk);
		glUnmapNamedBuffer(SSBOgtyp);
	}
	start = curp;
	fill = _fil;
	layer++;
}



void GGTell(float* img) {
	int id = -1;
	int* _raw = (int*)glMapNamedBuffer(SSBOTell, GL_READ_ONLY);
	for (int i = 0; i < RawDim[0]; ++i) {
		for (int j = 0; j < RawDim[1]; ++j) {
			id = i*int(RawDim[1]) + j;
			img[id] = float(_raw[id]) / 256.0f;
		}
	}
	glUnmapNamedBuffer(SSBOTell);
}