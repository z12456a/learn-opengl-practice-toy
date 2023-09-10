
#include "InputStream.h"
#include "direct.h"
#include "stb_image.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

using namespace Assimp;
using namespace std;

FBXStream::FBXStream()
{
}

FBXStream::~FBXStream()
{
}

InputStream::InputStream()
{
	appPath = _getdcwd(NULL, 0, 225);
}


InputStream::~InputStream()
{
	closeStream();
}

void InputStream::closeStream()
{
	if(!fileStream)
		_fclose_nolock(fileStream);
}

ShaderDataStream::ShaderDataStream(string fileName, GLenum type) :InputStream()
{
	filePath = appPath + "\\common\\shader\\" + fileName;
	fopen_s(&fileStream, filePath.c_str(), "r");
	while (!feof(fileStream))
	{
		Content += fgetc(fileStream);
	}
	Content[Content.length() - 1] = '\0';
	S_Type = type;
}

Debug_consoler::Debug_consoler()
{
}

Debug_consoler::~Debug_consoler()
{
}
void Debug_consoler::DebugMat(int rowSize, int colunmSize,float* data)
{
	float* fi=data;
	cout << "mat:" << endl;
	for (int i0 = 0; i0 < rowSize; i0++)
	{
		for (int i1 = 0; i1 < colunmSize; i1++)
		{
			cout << *fi << "   ";
			fi++;
		}
		cout << endl;
	}

}

ShaderDataStream::~ShaderDataStream()
{
}


ConfigStream::~ConfigStream()
{
}
void ConfigStream::GetObjAttribute()
{
	string s_line=string();
	string s_text = string();
	string s_obj = string();
	string s_value = string();
	bool IsReadingObj=true;
	for (string::iterator pi1=content.begin();pi1!=content.end();pi1++)//read text
	{
		s_text += *pi1;
		if (*pi1 == '\n')
		{
			s_line += *pi1;
			for (string::iterator pi2 = s_line.begin(); pi2 != s_line.end(); pi2++)//read obj & value
			{
				switch (IsReadingObj)
				{
				case(true):
					if (*pi2 == '=')
					{
						IsReadingObj = false;
					}
					else
					{
						s_obj += *pi2;
					}
					break;
				case(false):
					if (*pi2 == '\n')
					{
						IsReadingObj = true;
					}
					else
					{
						s_value += *pi2;
					}
					break;
				default:
					break;
				}
			}

			linkValueObjByString(&s_obj, &s_value);
			cout << s_obj.c_str()<<endl;
			cout << s_value.c_str()<<endl;
			s_obj.clear();
			s_value.clear();
			s_line.clear();
		}
		else
		{
			s_line += *pi1;
		}
	}
}
void ConfigStream::linkValueObjByString(string *obj,string *value)
{
	if (*obj=="SCR_WIDTH")
	{
		int v0=0;
		for (string::iterator pi1 = value->begin(); pi1 != value->end(); pi1++)
		{
			v0 = v0 * 10 + (*pi1-48);
		}
		SCR_WIDTH = v0;
	}
	else if(*obj =="SCR_HEIGHT" )
	{
		int v0=0;
		for (string::iterator pi1 = value->begin(); pi1 != value->end(); pi1++)
		{
			v0 = v0 * 10 + (*pi1 - 48);
		}
		SCR_HEIGHT = v0;
	}
}


AssimpStream::AssimpStream(string fName)
{
	filePath = appPath+"\\conmon\\modle\\obj\\"+fName;

}
AssimpStream::~AssimpStream()
{
	_Scene->~aiScene();
}
void AssimpStream::LoadObj()
{
	_Scene = ObjImporter.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!_Scene || _Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !_Scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << ObjImporter.GetErrorString() << endl;
		return;
	}
}


TexTureStream::TexTureStream()
{

}

void TexTureStream::LoadImg(string fileName)
{
	stbi_set_flip_vertically_on_load(true);
	filePath = appPath + "\\common\\texture\\" + fileName;
	data = stbi_load(filePath.c_str(), &width, &high, &nrChannel, 0);
	if (data)
		cout << "img :" << "done" << endl;
}

enum SP_LOAD_TEX_TYPE
{
	SP_LOAD_TEX_TYPE_Main,
	SP_LOAD_TEX_TYPE_diffuse,
	SP_LOAD_TEX_TYPE_specular,
	SP_LOAD_TEX_TYPE_normal,
	SP_LOAD_TEX_TYPE_environment,
};
void TexTureStream::DefaultLoadImg(int Type)
{
	switch (Type)
	{
	case(SP_LOAD_TEX_TYPE_Main):
#pragma region SP_LOAD_TEX_TYPE_Main
			stbi_set_flip_vertically_on_load(true);
			filePath = appPath + "\\common\\texture\\DefaultTex0.png";
			data = stbi_load(filePath.c_str(), &width, &high, &nrChannel, 0);
			if (data)
				cout << "img :"<< "done" << endl;
		break;
#pragma endregion
	case(SP_LOAD_TEX_TYPE_diffuse):
#pragma region SP_LOAD_TEX_TYPE_diffuse
		stbi_set_flip_vertically_on_load(true);
		filePath = appPath + "\\common\\texture\\DefaultTex1.png";
		data = stbi_load(filePath.c_str(), &width, &high, &nrChannel, 0);
		if (data)
			cout << "img :" << "done" << endl;
#pragma endregion
		break;
	case(SP_LOAD_TEX_TYPE_specular):
#pragma region SP_LOAD_TEX_TYPE_specular

#pragma endregion
		break;
	case(SP_LOAD_TEX_TYPE_normal):
#pragma region SP_LOAD_TEX_TYPE_normal
		stbi_set_flip_vertically_on_load(true);
		filePath = appPath + "\\common\\texture\\DefaultNorm0.jpg";
		data = stbi_load(filePath.c_str(), &width, &high, &nrChannel, 0);
		if (data)
			cout << "img :" << "done" << endl;
#pragma endregion
		break;
	case(SP_LOAD_TEX_TYPE_environment):
#pragma region SP_LOAD_TEX_TYPE_environment

#pragma endregion
		break;

	default:
		break;
	}

}

TexTureStream::~TexTureStream()
{
	if (data)
	{
		delete(data);
	}
}




