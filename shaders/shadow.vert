#version 430 core

layout(location = 0) in vec3 vPosition;

layout(location = 0) uniform mat4 modelMat;
layout(location = 6) uniform mat4 lightSpaceMat;
layout(location = 8) uniform vec4 clipPlane[6];

void main()
{
    gl_Position = lightSpaceMat * modelMat * vec4(vPosition, 1.0f);

	vec3 fragPos = vec3(modelMat * vec4(vPosition, 1.0));
	// clip plane
	gl_ClipDistance[0] = dot(vec4(fragPos, 1.0), clipPlane[0]);
	gl_ClipDistance[1] = dot(vec4(fragPos, 1.0), clipPlane[1]);
	gl_ClipDistance[2] = dot(vec4(fragPos, 1.0), clipPlane[2]);
	gl_ClipDistance[3] = dot(vec4(fragPos, 1.0), clipPlane[3]);
	gl_ClipDistance[4] = dot(vec4(fragPos, 1.0), clipPlane[4]);
	gl_ClipDistance[5] = dot(vec4(fragPos, 1.0), clipPlane[5]);
}