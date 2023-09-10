#pragma once
#include "iostream"
#include "list"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


using namespace std;

class InputStream
{
public:
	InputStream();
	~InputStream();

	FILE *fileStream;
	
	string filePath;
	string appPath;

	void closeStream();
};
class Debug_consoler
{
public:
	Debug_consoler();
	~Debug_consoler();

	void DebugMat(int rowSize,int colunmSize, float* data);

private:

};

class ShaderDataStream :
	public InputStream
{
public:
	ShaderDataStream(string fileName, GLenum type);
	~ShaderDataStream();

	string Content;
	GLenum S_Type;
};

class ConfigStream:
	public InputStream
{
public:
	ConfigStream(string fName) :InputStream()
	{
		filePath = appPath + "\\config\\" + fName;
		fopen_s(&fileStream, filePath.c_str(), "r+");
		while (!feof(fileStream))
		{
			content += fgetc(fileStream);
		}
		GetObjAttribute();
	}
	~ConfigStream();

	string content;

	int SCR_WIDTH= 800;
	int SCR_HEIGHT=600;

	void GetObjAttribute();
	

private:
	void linkValueObjByString(string *obj, string *value);

};

class FBXStream:
	public InputStream
{
public:
	FBXStream();
	~FBXStream();

private:

};

class AssimpStream :
	public InputStream
{
public:
	AssimpStream(string fName);
	~AssimpStream();

	Assimp::Importer ObjImporter;
	const aiScene *_Scene;

	void LoadObj();
private:


};

class TexTureStream:
	public InputStream
{
public:
	TexTureStream();
	~TexTureStream();

	int width;
	int high;
	int nrChannel;
	unsigned char *data;

	void LoadImg(string fileName);
	void DefaultLoadImg(int Type);

private:

};



