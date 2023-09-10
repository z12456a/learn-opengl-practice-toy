#pragma once
#include <stdio.h>

class baseDataStruct
{
public:
	baseDataStruct();
	~baseDataStruct();
};

class vec4
{
public:
	float x;
	float y;
	float z;
	float w;

	vec4(float x, float y, float z,float w);
	~vec4();

private:

};
class vec3
{
public:
	float x;
	float y;
	float z;

	vec3(float x, float y, float z);
	~vec3();

private:

};
class vec2
{
public:
	float x;
	float y;

	vec2(float x, float y);
	~vec2();

private:

};





