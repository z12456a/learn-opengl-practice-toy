#include "MainShaderPractice.h"
#include "InputStream.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/euler_angles.hpp"


#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "thread"
#include "windows.h"
using namespace std;

MainShaderPractice::MainShaderPractice()
{
}
MainShaderPractice::~MainShaderPractice()
{
}

SP_VAObj::SP_VAObj(MainShaderPractice *IDList)
{
	spID = IDList->VAOID;
	IDList->VAOID++;
}
SP_VAObj::~SP_VAObj()
{
}

SP_VBObj::SP_VBObj(MainShaderPractice *IDList)
{
	spID = IDList->EBOID;
	IDList->EBOID++;
}
SP_VBObj::~SP_VBObj()
{
	
}

SP_EBObj::SP_EBObj(MainShaderPractice *IDList)
{
	spID = IDList->EBOID;
	IDList->EBOID++;
}
SP_EBObj::~SP_EBObj()
{
}

SP_Sampler::SP_Sampler(MainShaderPractice *ListID)
{
}
SP_Sampler::~SP_Sampler()
{
}

SP_Mesh::~SP_Mesh()
{
	Vertices.clear();
	Elements.clear();

	delete(_VAO);
	delete(_VBO);
	delete(_EBO);
}

SP_Program::SP_Program()
{
}

SP_Program::SP_Program(MainShaderPractice *IDList, list<ShaderDataStream> *shaderSources)
{
	ID = IDList->ProgramID;
	IDList->ProgramID++;
	ShaderSources = *shaderSources;
}

SP_Program::SP_Program(MainShaderPractice *IDList)
{
	ID = IDList->ProgramID;
	IDList->ProgramID++;
	ShaderSources = list<ShaderDataStream>();
}

SP_Program::~SP_Program()
{

}
void SP_Program::consoleExpection()
{
	for (list<ShaderDataStream>::iterator pi = ShaderSources.begin(); pi == ShaderSources.end(); pi++)
	{
		cout << pi->Content.c_str() << endl;
	}
}
void SP_Program::programDestroy()
{

}
void SP_Program::Init()
{
	ID = glCreateProgram();
	attachAllShader();
	glLinkProgram(ID);
	cout << "LinkProgram success " << endl;
}
void SP_Program::DefaulInit()
{
	ID = glCreateProgram();
	ShaderDataStream dvss = ShaderDataStream("DefaultVs.vert", GL_VERTEX_SHADER);
	ShaderDataStream dfss = ShaderDataStream("DefaultFs.frag", GL_FRAGMENT_SHADER);


	const char *dvs = dvss.Content.c_str();
	int vsID=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1, &dvs, NULL);
	glCompileShader(vsID);
	int vsEID;
	glGetShaderiv(vsID, GL_COMPILE_STATUS, &vsEID);
	if (!vsEID)
	{
		char infoLog[255];
		glGetShaderInfoLog(vsID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout <<"content:" << std::endl << dvss.Content.c_str() << std::endl;
	}
	glAttachShader(ID, vsID);
	const char *dfs = dfss.Content.c_str();
	int fsID = glCreateShader(GL_FRAGMENT_SHADER);


	glShaderSource(fsID, 1, &dfs, NULL);
	glCompileShader(fsID);
	int fsEID;
	glGetShaderiv(fsID, GL_COMPILE_STATUS, &fsEID);
	if (!fsEID)
	{
		char infoLog[255];
		glGetShaderInfoLog(fsID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout << "content:" << std::endl << dfss.Content.c_str() << std::endl;
	}
	glAttachShader(ID, fsID);

	glLinkProgram(ID);
}
void SP_Program::attachAllShader()
{
	for (list<ShaderDataStream>::iterator pi = ShaderSources.begin(); pi != ShaderSources.end(); pi++)
	{
		int expID;
		int sID = glCreateShader(pi->S_Type);
		const char *ss = pi->Content.c_str();
		glShaderSource(sID, 1, &ss, NULL);
		glCompileShader(sID);
		glGetShaderiv(sID, GL_COMPILE_STATUS, &expID);
		if (!expID)
		{
			char infoLog[255];
			glGetShaderInfoLog(sID, 512, NULL, infoLog);
			switch (pi->S_Type)
			{
			case(GL_VERTEX_SHADER):
				std::cout << "ERROR::SHADER::GL_VERTEX_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
				break;
			case(GL_FRAGMENT_SHADER):
				std::cout << "ERROR::SHADER::GL_FRAGMENT_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
				break;
			case(GL_GEOMETRY_SHADER):
				std::cout << "ERROR::SHADER::GL_FRAGMENT_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
				break;
			case(GL_TESS_CONTROL_SHADER):
				std::cout << "ERROR::SHADER::GL_TESS_CONTROL_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
				break;
			default:
				break;
			}
		
		}
		glAttachShader(ID, sID);
		cout << "attachShader success" << endl;
	}

}

void  SP_Program::setBool(const std::string &name, bool value) 
{
	glUseProgram(ID);
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void  SP_Program::setInt(const std::string &name, int value) 
{
	glUseProgram(ID);
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void SP_Program::setFloat(const std::string &name, float value) 
{
	glUseProgram(ID);
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void SP_Program::setVec2(const std::string &name, const glm::vec2 &value) 
{
	glUseProgram(ID);
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void SP_Program::setVec2(const std::string &name, float x, float y) 
{
	glUseProgram(ID);
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void SP_Program::setVec3(const std::string &name, const glm::vec3 &value) 
{
	glUseProgram(ID);
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void SP_Program::setVec3(const std::string &name, float x, float y, float z) 
{
	glUseProgram(ID);
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void SP_Program::setVec4(const std::string &name, const glm::vec4 &value) 
{
	glUseProgram(ID);
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void SP_Program::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUseProgram(ID);
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void SP_Program::setMat2(const std::string &name, const glm::mat2 &mat) 
{
	glUseProgram(ID);
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void SP_Program::setMat3(const std::string &name, const glm::mat3 &mat) 
{
	glUseProgram(ID);
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void SP_Program:: setMat4(const std::string &name, const glm::mat4 &mat) 
{
	glUseProgram(ID);
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

SP_Transform::SP_Transform(MainShaderPractice *IDList, SP_Transform *parent)
{
	ID = IDList->TransformID;
	IDList->TransformID++;

	_ParentTrns = parent;


}
SP_Transform::~SP_Transform()
{

}

void SP_VAObj::Init()
{ 
}

SP_Mesh::SP_Mesh(MainShaderPractice *IDList)
{
	ID = IDList->MeshID;
	IDList->MeshID++;
	_VAO = new SP_VAObj(IDList);
	_VBO = new SP_VBObj(IDList);
	_EBO = new SP_EBObj(IDList);

}
void  SP_Mesh::SetupAllTangent()
{
	SP_Vertex vVal3[3];

	//if (Elements.size()!=0)
	//{
	//}
	//else
	
		//triangle
		int vVal3_i0 = 0;
		for (vector<SP_Vertex>::iterator vi = Vertices.begin(); vi != Vertices.end(); vi++)
		{
			vVal3[vVal3_i0] = *vi;
			vVal3_i0++;
			if (vVal3_i0==3)
			{
				float du12 = vVal3[0].TexCoords.x - vVal3[1].TexCoords.x;
				float dv12 = vVal3[0].TexCoords.y - vVal3[1].TexCoords.y;
				float du23 = vVal3[1].TexCoords.x - vVal3[2].TexCoords.x;
				float dv23 = vVal3[1].TexCoords.y - vVal3[2].TexCoords.y;


				glm::mat2 dUVmat = glm::mat2(du12, dv12, du23, dv23);
				glm::mat2x3 EdgeMat = glm::mat2x3(vVal3[0].Position- vVal3[1].Position, vVal3[1].Position - vVal3[2].Position);

				glm::mat2x3 TBmat = EdgeMat*glm::inverse(dUVmat);

				//for (int i1 = 0; i1 < 2; i1++)
				//{
				//	for (int i2 = 0; i2 < 3; i2++)
				//	{
				//		cout<<std::to_string(TBmat[i1][i2])  <<"  ";
				//	}
				//	cout << endl;
				//}

				for (int i = 0; i < 3; i++)
				{
					vVal3[i].Tangent = TBmat[0];
					vVal3[i].BiTangent = TBmat[1];
				}
				vVal3_i0 = 0;
			}
		}
	
}
void  SP_Mesh::MeshInit_Cube()
{
	SP_Vertex vi;
#pragma region MyRegion
	//face1
	//1
	vi.Position = glm::vec3(-0.5f, -0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, -1.0f);
	Vertices.push_back(vi);
	//2
	vi.Position = glm::vec3(0.5f, 0.5f, -0.5f);	vi.TexCoords = glm::vec2(1.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, -1.0f);
	Vertices.push_back(vi);
	//3
	vi.Position = glm::vec3(0.5f, -0.5f, -0.5f);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, -1.0f);
	Vertices.push_back(vi);

	//4
	vi.Position = glm::vec3(0.5f, 0.5f, -0.5f);	vi.TexCoords = glm::vec2(1.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, -1.0f);
	Vertices.push_back(vi);
	//5
	vi.Position = glm::vec3(-0.5f, -0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, -1.0f);
	Vertices.push_back(vi);
	//6
	vi.Position = glm::vec3(-0.5f, 0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, -1.0f);
	Vertices.push_back(vi);

	//face2
	//7
	vi.Position = glm::vec3(-0.5f, -0.5f, 0.5f);	vi.TexCoords = glm::vec2(0.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	Vertices.push_back(vi);
	//9
	vi.Position = glm::vec3(0.5f, -0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	Vertices.push_back(vi);
	//8
	vi.Position = glm::vec3(0.5f, 0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	Vertices.push_back(vi);

	//10
	vi.Position = glm::vec3(0.5f, 0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	Vertices.push_back(vi);
	//12
	vi.Position = glm::vec3(-0.5f, 0.5f, 0.5f);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	Vertices.push_back(vi);
	//11
	vi.Position = glm::vec3(-0.5f, -0.5f, 0.5f);	vi.TexCoords = glm::vec2(0.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	Vertices.push_back(vi);


	//face3
	//13
	vi.Position = glm::vec3(-0.5f, 0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);
	//14
	vi.Position = glm::vec3(-0.5f, 0.5f, -0.5f);	vi.TexCoords = glm::vec2(1.0f, 1.0f);	vi.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);
	//15
	vi.Position = glm::vec3(-0.5f, -0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);
	//16
	vi.Position = glm::vec3(-0.5f, -0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);
	//17
	vi.Position = glm::vec3(-0.5f, -0.5f, 0.5f);	vi.TexCoords = glm::vec2(0.0f, 0.0f);	vi.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);
	//18
	vi.Position = glm::vec3(-0.5f, 0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);

	//face4
	//19
	vi.Position = glm::vec3(0.5f, 0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);
	//20
	vi.Position = glm::vec3(0.5f, -0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);
	//21
	vi.Position = glm::vec3(0.5f, 0.5f, -0.5f);	vi.TexCoords = glm::vec2(1.0f, 1.0f);	vi.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);

	//22
	vi.Position = glm::vec3(0.5f, -0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);
	//23
	vi.Position = glm::vec3(0.5f, 0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);
	//24
	vi.Position = glm::vec3(0.5f, -0.5f, 0.5f);	vi.TexCoords = glm::vec2(0.0f, 0.0f);	vi.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
	Vertices.push_back(vi);


	//face5
	//25
	vi.Position = glm::vec3(-0.5f, -0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
	Vertices.push_back(vi);
	//26
	vi.Position = glm::vec3(0.5f, -0.5f, -0.5f);	vi.TexCoords = glm::vec2(1.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
	Vertices.push_back(vi);
	//27
	vi.Position = glm::vec3(0.5f, -0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
	Vertices.push_back(vi);
	//28
	vi.Position = glm::vec3(0.5f, -0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
	Vertices.push_back(vi);
	//29
	vi.Position = glm::vec3(-0.5f, -0.5f, 0.5f);	vi.TexCoords = glm::vec2(0.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
	Vertices.push_back(vi);
	//30
	vi.Position = glm::vec3(-0.5f, -0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
	Vertices.push_back(vi);

	//face6
	//31
	vi.Position = glm::vec3(-0.5f, 0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);
	//33
	vi.Position = glm::vec3(0.5f, 0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);
	//32
	vi.Position = glm::vec3(0.5f, 0.5f, -0.5f);	vi.TexCoords = glm::vec2(1.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);

	//34
	vi.Position = glm::vec3(0.5f, 0.5f, 0.5f);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);
	//36
	vi.Position = glm::vec3(-0.5f, 0.5f, -0.5f);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);
	//35
	vi.Position = glm::vec3(-0.5f, 0.5f, 0.5f);	vi.TexCoords = glm::vec2(0.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);


#pragma endregion

}
void SP_Mesh::MeshInit_Plane()
{
	SP_Vertex vi;
	//face1
	//1
	vi.Position = glm::vec3(5, 0, -5);	vi.TexCoords = glm::vec2(0.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);
	//2
	vi.Position = glm::vec3(-5, 0, -5);	vi.TexCoords = glm::vec2(1.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);
	//3
	vi.Position = glm::vec3(-5,0, 5);	vi.TexCoords = glm::vec2(1.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);
	//4
	vi.Position = glm::vec3(-5,0, 5);	vi.TexCoords = glm::vec2(1.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);
	//5
	vi.Position = glm::vec3(5,0, 5);	vi.TexCoords = glm::vec2(0.0f, 1.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);
	//6
	vi.Position = glm::vec3(5,0, -5);	vi.TexCoords = glm::vec2(0.0f, 0.0f);	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertices.push_back(vi);

}
void SP_Mesh::MeshInit_Shpere()
{

	const int tessellationZ=40;
	float rUnit = glm::radians((float)360/ (float)tessellationZ);
	float Pi = glm::pi<float>();
	glm::vec3 vList[(tessellationZ / 2)+1][tessellationZ];

	for (int vZ=0;vZ<(tessellationZ/2)+1;vZ++)
	{
		for (int vX = 0; vX < tessellationZ; vX ++)
		{
			vList[vZ][vX] = glm::vec3(glm::sin(vZ*rUnit)*glm::sin(vX*rUnit) , glm::cos(vZ*rUnit), glm::sin(vZ*rUnit)*glm::cos(vX*rUnit))*0.5f;
			//cout  << "vaaa:  "<<std::to_string(vList[vZ][vX].x) << "  " << std::to_string(vList[vZ][vX].y)<< "  " << std::to_string (vList[vZ][vX].z)<< endl;
		}
	}

	//vX+1==tessellationZ?0: vX+1
	for (int vZi = 0; vZi < tessellationZ / 2; vZi++)
	{
		SP_Vertex vi;
		for (int vXi = 0; vXi < tessellationZ; vXi++)
		{
			int vXnext= (vXi + 1 == tessellationZ ? 0 : vXi + 1);
			//face1
			vi.Position = vList[vZi][vXi];
			vi.TexCoords = glm::vec2((float)vZi /((float)tessellationZ / 2), (float)vXi / (float)tessellationZ);	
			vi.Normal = vList[vZi][vXi];
			Vertices.push_back(vi);

			vi.Position = vList[vZi + 1][vXi];
			vi.TexCoords = glm::vec2((float)(vZi + 1) / ((float)tessellationZ / 2), (float)vXi / (float)tessellationZ);	
			vi.Normal = vList[vZi + 1][vXi];
			Vertices.push_back(vi);

			vi.Position = vList[vZi][vXnext];
			vi.TexCoords = glm::vec2((float)vZi / ((float)tessellationZ / 2), (float)(vXi + 1) / (float)tessellationZ);
			vi.Normal = vList[vZi][vXnext];
			Vertices.push_back(vi);

			//face2
			vi.Position = vList[vZi + 1][vXi];
			vi.TexCoords = glm::vec2((float)(vZi + 1) / ((float)tessellationZ / 2), (float)vXi / (float)tessellationZ);	
			vi.Normal = glm::vec3(vList[vZi + 1][vXi].x, vList[vZi + 1][vXi].y, vList[vZi + 1][vXi].z);
			Vertices.push_back(vi);

			vi.Position = vList[vZi + 1][vXnext];
			vi.TexCoords = glm::vec2((float)(vZi + 1 )/ ((float)tessellationZ / 2), (float)(vXi + 1) / (float)tessellationZ);
			vi.Normal = vList[vZi + 1][vXnext];
			Vertices.push_back(vi);

			vi.Position = vList[vZi][vXnext];
			vi.TexCoords = glm::vec2((float)vZi/ ((float)tessellationZ / 2), (float)(vXi + 1) / (float)tessellationZ);
			vi.Normal = vList[vZi][vXnext];
			Vertices.push_back(vi);

			//cout << "ZX:"<<std::to_string(vZi) <<"   "<< std::to_string(vXi)<<"   "<< std::to_string(vXnext) <<endl;
		}                  
	}

}

void SP_Mesh::MeshInit_Plane2()
{
	SP_Vertex vi;
	float TessNum = 100;
	for (int i=0;i< TessNum;i++)
	{
		for (int j = 0; j < TessNum; j++)
		{
			//face1
			//1
			vi.Position = glm::vec3(i + 1, 0, j);	vi.TexCoords = glm::vec2(1/ TessNum * (i + 1), 1 / TessNum * (j));	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			Vertices.push_back(vi);
			//Vertices.push_back(SP_Vertex{ glm::vec3(1,1,1),glm::vec3(1,1,1) ,glm::vec2(1,1) ,glm::vec3(1,1,1) ,glm::vec3(1,1,1) });
			//2
			vi.Position = glm::vec3(i, 0, j);	vi.TexCoords = glm::vec2(1 / TessNum * (i), 1 / TessNum * (j));	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			Vertices.push_back(vi);
			//3
			vi.Position = glm::vec3(i, 0, j + 1);	vi.TexCoords = glm::vec2(1 / TessNum * (i), 1 / TessNum * (j+1));	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			Vertices.push_back(vi);
			//4
			vi.Position = glm::vec3(i, 0, j + 1);	vi.TexCoords = glm::vec2(1 / TessNum * (i), 1 / TessNum * (j + 1));	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			Vertices.push_back(vi);
			//5
			vi.Position = glm::vec3(i + 1, 0, j + 1);	vi.TexCoords = glm::vec2(1 / TessNum * (i+1), 1 / TessNum * (j + 1));	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			Vertices.push_back(vi);
			//6
			vi.Position = glm::vec3(i + 1, 0, j);	vi.TexCoords = glm::vec2(1 / TessNum * (i + 1), 1 / TessNum * (j ));	vi.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			Vertices.push_back(vi);
		}
	}


}

void SP_Mesh::DefaultMeshInit(int DefaultMeshType)
{
	switch (DefaultMeshType)
	{
	case(SP_DefaultMesh_CUBE):
		MeshInit_Cube();
		break;
	case(SP_DefaultMesh_PLANE):
		MeshInit_Plane();
		break;
	case(SP_DefaultMesh_SHPERE):
		MeshInit_Shpere();
		break;
	default:
		break;
	}


	SetupAllTangent();

	glGenVertexArrays(1, &(_VAO->glID));
	glGenBuffers(1, &(_VBO->glID));

	glBindVertexArray(_VAO->glID);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO->glID);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size()*sizeof(SP_Vertex), &(Vertices.front()), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::Position));
	//n=1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::TexCoords));
	//t=2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::Normal));
	//c=3
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::Tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::BiTangent));

	glBindVertexArray(0);
}

void SP_Mesh::Init()
{
	SetupAllTangent();
	BindAllGLVertices();
}

void SP_Mesh::BindAllGLVertices()
{
	glGenVertexArrays(1, &(_VAO->glID));
	glGenBuffers(1, &(_VBO->glID));
	glGenBuffers(1, &(_EBO->glID));

	glBindVertexArray(_VAO->glID);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO->glID);
	glBufferData(GL_ARRAY_BUFFER,Vertices.size()*sizeof(SP_Vertex), &(Vertices.front()),GL_STATIC_DRAW);

	if (Elements.size()!=0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO->glID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Elements.size()*sizeof(GLuint), &Elements[0], GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::Position));
	//n=1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::TexCoords));
	//t=2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::Normal));
	//c=3
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::Tangent));
	//4
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::BiTangent));

	glBindVertexArray(0);
}

glm::vec3 SP_Transform::WorldPos()
{
	glm::vec3 r=position;
	for (SP_Transform *_ti= _ParentTrns; _ti; _ti = _ti->_ParentTrns)
	{
		r += _ti->position;
	}
	return r;
}

glm::vec4 SP_Transform::WorldQuat()
{
	glm::mat4 rMat=glm::mat4(1.0f);
	glm::vec3 LastQuat= glm::normalize(glm::vec3(quaternion.x, quaternion.y, quaternion.z));
	glm::vec3 ThisQuat = glm::vec3();
	float QuatW=quaternion.w;
	for (SP_Transform *_ti = _ParentTrns; _ti; _ti = _ti->_ParentTrns)
	{
		ThisQuat=glm::normalize(glm::vec3(_ti->quaternion.x, _ti->quaternion.y, _ti->quaternion.z));
		rMat = glm::rotate(
			rMat,
			glm::radians(glm::acos(glm::dot(ThisQuat, glm::vec3(0.0f, 0.0f, 1.0f)))) ,
			glm::cross(ThisQuat, glm::vec3(0.0f,0.0f,1.0f)));
		//LastQuat = ThisQuat;
		QuatW += _ti->quaternion.w;
	}
	
	return glm::vec4(0.0f, 0.0f, 1.0f, QuatW) * rMat;
}
glm::mat4 SP_Transform::GetRotateMat()
{
	if (_ParentTrns)
	{
		quaternion = WorldQuat();
		RealateToEularAngle();
		return glm::eulerAngleYZX(eulerAngle.y, eulerAngle.z, eulerAngle.x);
	}
	else
	{
		return glm::eulerAngleYZX(eulerAngle.y, eulerAngle.z, eulerAngle.x);
	} 
}
glm::mat4 SP_Transform::GetTranslateMat()
{
	return glm::translate(glm::mat4(1.0f), WorldPos());
}
glm::mat4 SP_Transform::GetModleMat()
{
	glm::mat4 rm = glm::mat4(1.0f);
	rm = glm::translate(rm, position);
	rm = glm::eulerAngleYZX(eulerAngle.y, eulerAngle.z, eulerAngle.x)*rm;
	return rm;
}
glm::mat4 SP_Transform::GetScaleMat()
{
	return glm::scale(glm::mat4(1.0f), scale);
}

void SP_Transform::RealateToEularAngle()
{
	float *x=new float(0.0f);
	float *y= new float(0.0f);
	float *z= new float(0.0f);
	extractEulerAngleXYZ(
		glm::rotate
			(glm::mat4(1.0f),
			glm::radians(glm::acos(glm::dot(glm::vec3(0.0f,0.0f,1.0f), glm::vec3(quaternion.x, quaternion.y, quaternion.z)))),
			glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(quaternion.x, quaternion.y, quaternion.z))),  
			*x,*y,*z);
	eulerAngle = glm::vec3(*x, *y, *z);
}
void SP_Transform::EulerRotate(float pitch, float yaw, float roll)
{
	eulerAngle.x += pitch;
	eulerAngle.y += yaw;
	eulerAngle.z += roll;
	RealateToQuaternion();
}
SP_Transform* SP_Transform::GetRootTrans()
{
	for (SP_Transform *_r = this; _r->_ParentTrns!=NULL; _r=_r->_ParentTrns)
	{
		if(_r->_ParentTrns == NULL)
			return _r;
	}
	return NULL;
}
void SP_Transform::DebugLog()
{
	//mat4
	//for (int i=0;i<4;i++)
	//{
	//	for (int j=0;j<4;j++)
	//	{

	//	}
	//}

	cout << "qua";
	for (int i=0;i<4;i++)
	{
		cout << quaternion[i]<<"  ";
	}
	cout << endl;

	cout << "eua";
	for (int i = 0; i < 3; i++)
	{
		cout << eulerAngle[i] << "  ";
	}
	cout << endl;

	cout << "pos";
	for (int i = 0; i < 3; i++)
	{
		cout << position[i] << "  ";
	}
	cout << endl;
}
void SP_Transform::RealateToQuaternion()
{
	quaternion.z= glm::cos(glm::radians(eulerAngle.x)) * glm::cos(glm::radians(eulerAngle.y));
	quaternion.y = glm::sin(glm::radians(eulerAngle.x));
	quaternion.x= glm::sin(glm::radians(eulerAngle.y)) * glm::cos(glm::radians(eulerAngle.x));

	quaternion.w = eulerAngle.z;
	RealateToDirect();
}
glm::vec3 SP_Transform::GetDirect()
{
	return glm::vec3(quaternion.x,quaternion.y,quaternion.z);
}
void SP_Transform::RealateToScale()
{
	glm::scale(ScaleMat, scale);
}
void SP_Transform::RealateToDirect()
{
	Direct_forward.x = quaternion.x;
	Direct_forward.y= quaternion.z;
	Direct_forward=glm::normalize(Direct_forward);
	Direct_back = -Direct_forward;
	
	Direct_left = glm::vec2(Direct_forward.y, -Direct_forward.x);
	Direct_right = -Direct_left;
}

void SP_Transform::QuatRotate(float x, float y, float z, float w)
{
	RealateToEularAngle();
}

SP_Material::SP_Material(MainShaderPractice *IDList)
{
	Root = IDList;
	ID = IDList->MaterialID;
	Root->MaterialID++;

}

SP_Material::~SP_Material()
{
	if (_Program)
	{
		//_Program->~SP_Program();
		delete(_Program);
	}
}

void SP_Material::BindTextureToShader(int i)
{
	glGenTextures(1, &Textures[i].gl_ID);
	glBindTexture(GL_TEXTURE_2D, Textures[i].gl_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (Textures[i].u_data )
	{
		switch (Textures[i].ChannelType)
		{
		case(SP_TEXTURE_CHANNEL_TYPE_RGB):
			BindTexture_RGB(i);
			break;
		case(SP_TEXTURE_CHANNEL_TYPE_RGB8):
			BindTexture_RGB8(i);
			break;
		case(SP_TEXTURE_CHANNEL_TYPE_RGBA):
			BindTexture_RGBA(i);
			break;
		case(SP_TEXTURE_CHANNEL_TYPE_sRGB):
			BindTexture_sRGB(i);
			break;
		case(SP_TEXTURE_CHANNEL_TYPE_sRGBA):
			BindTexture_sRGBA(i);
			break;
		case(SP_TEXTURE_CHANNEL_TYPE_sRGBA64):
			BindTexture_sRGBA64(i);
			break;

		default:
			break;
		}
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	cout << "index:" << i<<"   " << "texture_" + Textures[i].TexType << endl;
	_Program->setInt(("texture_" + Textures[i].TexType+std::to_string(Textures[i].TypeID)).c_str() , i);
	stbi_image_free(Textures[i].u_data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SP_Material::BindAllTextureToShader()
{
	glUseProgram(_Program->ID);
	for (int vi = 0; vi < Textures.size(); vi++)
	{
		BindTextureToShader(vi);
	}
	glUseProgram(0);
}

void SP_Material::ActiveTextureToDraw(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D,Textures[index].gl_ID);
	//Active then bind
	//reason: GL_TEXTURE0 is default Texture 
	//invoke :DrawQueue
}

void SP_Material::ActiveAllTextureToDraw()
{
	for (int ti = 0; ti < Textures.size(); ti++)
	{
		ActiveTextureToDraw(ti);
	}
	/*
	注意pushBack对应纹理的规范
	与shader读取顺序保持一致
	(已过时)
	use this function after glUseProgram(...);
	when you want to draw a obj with TexData
	*/
}

void SP_Material::NullProgramException()
{

}

void SP_Material::BindTexture_RGB(int index)
{
	glTexImage2D
	(GL_TEXTURE_2D,
		0,
		GL_RGB,
		Textures[index].Width,
		Textures[index].High,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		Textures[index].u_data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void SP_Material::BindTexture_RGB8(int index)
{
	glTexImage2D
	(GL_TEXTURE_2D,
		0,
		GL_RGB,
		Textures[index].Width,
		Textures[index].High,
		0,
		GL_RGB8,
		GL_UNSIGNED_BYTE,
		Textures[index].u_data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void SP_Material::BindTexture_RGBA(int index)
{
	glTexImage2D
	(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		Textures[index].Width,
		Textures[index].High,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Textures[index].u_data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void SP_Material::BindTexture_RGBA64(int index)
{
	glTexImage2D
	(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		Textures[index].Width,
		Textures[index].High,
		0,
		GL_RGBA16,
		GL_UNSIGNED_BYTE,
		Textures[index].u_data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void SP_Material::BindTexture_sRGB(int index)
{
	glTexImage2D
	(GL_TEXTURE_2D,
		0,
		GL_SRGB,
		Textures[index].Width,
		Textures[index].High,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Textures[index].u_data);
	glGenerateMipmap(GL_TEXTURE_2D);
}
void SP_Material::BindTexture_sRGBA(int index)
{
	glTexImage2D
	(GL_TEXTURE_2D,
		0,
		GL_SRGB_ALPHA,
		Textures[index].Width,
		Textures[index].High,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Textures[index].u_data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void SP_Material::BindTexture_sRGBA64(int index)
{
	glTexImage2D
	(GL_TEXTURE_2D,
		0,
		GL_SRGB_ALPHA,
		Textures[index].Width,
		Textures[index].High,
		0,
		GL_RGBA16,
		GL_UNSIGNED_BYTE,
		Textures[index].u_data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void SP_Material::DefaultInit()
{
	_Program =new SP_Program(Root);
	_Program->DefaulInit();

	DefaultSetupTexture();
	BindAllTextureToShader();


}
void SP_Material::Init(SP_Program* pi)
{
	if (pi)
	{
		_Program = pi;
		_Program->Init();
	}
	else
	{
		NullProgramException();
		return;
	}

	cout << "Material Init _Program Expection:"<< endl;
	_Program->consoleExpection();

	BindAllTextureToShader();
}

void SP_Material::Init()
{
	
	if (_Program)
	{
		_Program->Init();
	}
	else
	{
		NullProgramException();
		return;
	}
	cout << "Material Init _Program Expection:" << endl;
	_Program->consoleExpection();

	BindAllTextureToShader();
}

void SP_Material::SetupTexture(string fName,unsigned int TypeId,string TexType,int pixleBit)
{
	TexTureStream* _TexTureStream = new TexTureStream();
	_TexTureStream->LoadImg(fName);

	SP_TextureInfo tpi = SP_TextureInfo();
	tpi.u_data = _TexTureStream->data;
	tpi.FileName = _TexTureStream->filePath;
	tpi.High = _TexTureStream->high;
	tpi.Width = _TexTureStream->width;
	tpi.RGB = _TexTureStream->nrChannel;
	tpi.TexType = TexType;
	tpi.TypeID = TypeId;
	tpi.ChannelType = pixleBit;

	Textures.push_back(tpi);

	//_TexTureStream->~TexTureStream();
}
void SP_Material::DefaultSetupTexture()
{
	TexTureStream* _TexTureStream1 = new TexTureStream();
	TexTureStream* _TexTureStream2 = new TexTureStream();
	TexTureStream* _TexTureStream3 = new TexTureStream();
	SP_TextureInfo tpi1= SP_TextureInfo();
	SP_TextureInfo tpi2 = SP_TextureInfo();
	SP_TextureInfo tpi3 = SP_TextureInfo();

	_TexTureStream1->DefaultLoadImg(0);

	tpi1.u_data = _TexTureStream1->data;
	tpi1.FileName = _TexTureStream1->filePath;
	tpi1.High = _TexTureStream1->high;
	tpi1.Width = _TexTureStream1->width;
	tpi1.RGB = _TexTureStream1->nrChannel;
	tpi1.TexType = "main";
	tpi1.TypeID = 0;
	tpi1.ChannelType = SP_TEXTURE_CHANNEL_TYPE_RGB;

	Textures.push_back(tpi1);

	_TexTureStream2->DefaultLoadImg(1);

	tpi2.u_data = _TexTureStream2->data;
	tpi2.FileName = _TexTureStream2->filePath;
	tpi2.High = _TexTureStream2->high;
	tpi2.Width = _TexTureStream2->width;
	tpi2.RGB = _TexTureStream2->nrChannel;
	tpi2.TexType = "main";
	tpi2.TypeID = 1;
	tpi2.ChannelType = SP_TEXTURE_CHANNEL_TYPE_RGBA;

	Textures.push_back(tpi2);

	_TexTureStream3->DefaultLoadImg(3);

	tpi3.u_data = _TexTureStream3->data;
	tpi3.FileName = _TexTureStream3->filePath;
	tpi3.High = _TexTureStream3->high;
	tpi3.Width = _TexTureStream3->width;
	tpi3.RGB = _TexTureStream3->nrChannel;
	tpi3.TexType = "normal";
	tpi3.TypeID = 0;
	tpi3.ChannelType = SP_TEXTURE_CHANNEL_TYPE_RGB;

	Textures.push_back(tpi3);

}

SP_UserInput::SP_UserInput()
{
}
SP_UserInput::~SP_UserInput()
{
}
void SP_UserInput::shutDown()
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, true);
}
void SP_UserInput::cameraContoller(SP_Camera *_cami)
{
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		_cami->_Transform->RealateToDirect();
		_cami->TranslateCam(_cami->_Transform->Direct_left.x/10.0f, 0.0f, _cami->_Transform->Direct_left.y / 10.0f);
	}
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		_cami->_Transform->RealateToDirect();
		_cami->TranslateCam(_cami->_Transform->Direct_back.x / (float)10, 0, _cami->_Transform->Direct_back.y / (float)10);
	}
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		_cami->_Transform->RealateToDirect();
		_cami->TranslateCam(_cami->_Transform->Direct_right.x / (float)10, 0, _cami->_Transform->Direct_right.y / (float)10);
	}
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		_cami->_Transform->RealateToDirect();
		_cami->TranslateCam(_cami->_Transform->Direct_forward.x / (float)10, 0, _cami->_Transform->Direct_forward.y / (float)10);
	}
	glfwGetCursorPos(_window,&CCPX,&CCPY);
	if (glfwGetMouseButton(_window,GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS)
	{
		_cami->RotateCam(LCPY-CCPY,LCPX-CCPX);
	}
	if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cout << "up" << endl;
		_cami->TranslateCam(0, 0.1, 0);
	}
	if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		_cami->TranslateCam(0, -0.1, 0);
	}
	if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		if (_cami->viewAngle < 175)
			_cami->viewAngle++;
		_cami->SetUpProjectionMat();
	}
	if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if(_cami->viewAngle>5)
			_cami->viewAngle--;
		_cami->SetUpProjectionMat();
	}
	LCPX = CCPX;
	LCPY = CCPY;
}

void SP_UserInput::ShaderParameterContoller(SP_RendererFactory * _factory)
{
	if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		ShaderParameter-=0.01f;
		cout << "ShaderParameter" << std::to_string(ShaderParameter) << endl;
		for (vector<list<SP_Object*>*>::iterator o_Li = _factory->OpaqueVL.begin(); o_Li != _factory->OpaqueVL.end(); o_Li++)
		{
			for (list<SP_Object*>::iterator oi = (**o_Li).begin(); oi != (**o_Li).end(); oi++)
			{
				(**oi)._Material->_Program->setFloat("KeyConUni", ShaderParameter);
			}
		}
	}
	if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		ShaderParameter+=0.01f;
		cout << "ShaderParameter" << std::to_string(ShaderParameter) << endl;
		for (vector<list<SP_Object*>*>::iterator o_Li = _factory->OpaqueVL.begin(); o_Li != _factory->OpaqueVL.end(); o_Li++)
		{
			for (list<SP_Object*>::iterator oi = (**o_Li).begin(); oi != (**o_Li).end(); oi++)
			{
				(**oi)._Material->_Program->setFloat("KeyConUni", ShaderParameter);
			}
		}
	}
}

SP_Object::SP_Object(MainShaderPractice *root)
{
	Root = root;
	ID = root->ObjID;
	root->ObjID++;
}
SP_Object::~SP_Object()
{
	if (_Material)
	{
		//_Material->~SP_Material();
		delete(_Material);
	}
	if (_Mesh)
	{
		//_Mesh->~SP_Mesh();
		delete(_Mesh);
	}
	if (_Transform)
	{
		//_Transform->~SP_Transform();
		delete(_Transform);
	}
	if (_ObjStream)
	{
		//_ObjStream->~AssimpStream();
		delete(_ObjStream);
	}

}

void SP_Object::Draw( SP_Camera *DCam)
{
	if (IsActive == true)
	{
		glStencilOp(StencilOp_DSFail, StencilOp_DFail, StencilOp_Pass);
		glStencilFunc(StencilFunc, StencilRef, StencilReadMask);
		glStencilMask(StencilWriteMask);

		switch (dMode)
		{
		case OBJ_DRAW_DEFAULT:
			DefaultDraw(DCam);
			break;
		case OBJ_DRAW_ELEMENT:
			ElementDraw(DCam);
			break;
		case OBJ_DRAW_LINE:
			LineDraw(DCam);
			break;
		default:
			break;
		}
	}
	else
	{

	}
}

void SP_Object::DefaultDraw(SP_Camera *DCam)
{
	_Material->_Program->setMat4("ViewMat", DCam->ViewMat);
	_Material->_Program->setMat4("ProjectionMat", DCam->ProjectionMat);
	_Material->_Program->setMat4("RotateMat", _Transform->GetRotateMat());
	_Material->_Program->setMat4("TranslateMat", _Transform->GetTranslateMat());
	_Material->_Program->setMat4("ScaleMat", _Transform->GetScaleMat());
	_Material->_Program->setVec3("Light0_Color", DCam->LightSourse[0]->LightColor);
	_Material->_Program->setVec3("Light0_Direct",DCam->LightSourse[0]->_Transform->GetDirect());
	_Material->_Program->setVec3("Light1_Pos", DCam->LightSourse[1]->_Transform->position);
	_Material->_Program->setVec3("Light1_Color", DCam->LightSourse[1]->LightColor);
	_Material->_Program->setVec3("CamPos",DCam->_Transform->WorldPos());
	_Material->_Program->setFloat("Alpha", _Material->Alpha);

	
	glUseProgram(_Material->_Program->ID);
	glBindVertexArray(_Mesh->_VAO->glID);
	_Material->ActiveAllTextureToDraw();
	glDrawArrays(GL_TRIANGLES, 0, _Mesh->Vertices.size());
	glBindVertexArray(0);
}

void SP_Object::LineDraw(SP_Camera *DCam)
{
	_Material->_Program->setMat4("ViewMat", DCam->ViewMat);
	_Material->_Program->setMat4("ProjectionMat", DCam->ProjectionMat);
	_Material->_Program->setMat4("RotateMat", _Transform->GetRotateMat());
	_Material->_Program->setMat4("TranslateMat", _Transform->GetTranslateMat());
	_Material->_Program->setMat4("ScaleMat", _Transform->GetScaleMat());
	_Material->_Program->setVec3("Light0_Color", DCam->LightSourse[0]->LightColor);
	_Material->_Program->setVec3("Light0_Direct", DCam->LightSourse[0]->_Transform->GetDirect());
	_Material->_Program->setVec3("Light1_Pos", DCam->LightSourse[1]->_Transform->position);
	_Material->_Program->setVec3("Light1_Color", DCam->LightSourse[1]->LightColor);
	_Material->_Program->setVec3("CamPos", DCam->_Transform->WorldPos());

	glUseProgram(_Material->_Program->ID);
	glBindVertexArray(_Mesh->_VAO->glID);
	_Material->ActiveAllTextureToDraw();
	glDrawArrays(GL_LINES, 0, _Mesh->Vertices.size());
	glBindVertexArray(0);
}
void SP_Object::ElementDraw(SP_Camera *DCam)
{
	glBindVertexArray(_Mesh->_VAO->glID);
	glDrawElements(GL_TRIANGLES, _Mesh->Elements.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SP_Object::DepthDraw(SP_LightBase * LB)
{

	LB->_Program_Shadow->setMat4("TranslateMat", LB->_Transform->GetTranslateMat());
	LB->_Program_Shadow->setMat4("ProjectionMat", LB->ProjectionMat);
	LB->_Program_Shadow->setMat4("RotateMat", LB->_Transform->GetRotateMat());
	LB->_Program_Shadow->setMat4("ViewMat", LB->ViewMat);

	glUseProgram(LB->_Program_Shadow->ID);
	glBindVertexArray(_Mesh->_VAO->glID);
	//LB->ActiveTexToGetDepth();
	glDrawArrays(GL_TRIANGLES, 0, _Mesh->Vertices.size());
	glBindVertexArray(0);
}

void SP_Object::DefaultInit(int MeshType)
{
	IsActive = true;
	HasMesh = true;
	HasMaterial = true;

	_Material = new SP_Material(Root);
	_Material->DefaultInit();

	_Mesh = new SP_Mesh(Root);
	_Mesh->DefaultMeshInit(MeshType);

	_Transform = new SP_Transform(Root, NULL);
}
void SP_Object::Init()
{
	if(_Transform)
		IsActive = true;
	if (_Mesh)
	{
		HasMesh = true;
		_Mesh->Init();
	}		
	if (_Material)
	{
		HasMaterial = true;
		_Material->Init();
	}
		

}
void  SP_Object::LoadMesh(string fName)
{
	_ObjStream = new AssimpStream(fName);
	aiNode *npi = _ObjStream->_Scene->mRootNode;
	ProcesseLoad(npi);
}

void  SP_Object::ProcesseLoad(aiNode *npi)
{
	aiMesh *mpi;
	// process all mesh in this node
	for (unsigned int mi = 0; mi < npi->mNumMeshes; mi++)
	{
		mpi = _ObjStream->_Scene->mMeshes[npi->mMeshes[mi]];

		for (unsigned int vi = 0; vi < mpi->mNumVertices; vi++)
		{
			SP_Vertex vpi;

			//load vertex
			vpi.Position.x = mpi->mVertices[vi].x;
			vpi.Position.y = mpi->mVertices[vi].y;
			vpi.Position.z = mpi->mVertices[vi].z;
			//load normal
			if (mpi->HasNormals())
			{
				vpi.Normal.x = mpi->mNormals[vi].x;
				vpi.Normal.y = mpi->mNormals[vi].y;
				vpi.Normal.z = mpi->mNormals[vi].z;
			}
			//load UV
			if (mpi->HasTextureCoords(vi))
			{
				vpi.TexCoords.x = mpi->mTextureCoords[0][vi].x;
				vpi.TexCoords.y = mpi->mTextureCoords[0][vi].y;
			}
			//load color
			//if (mpi->HasVertexColors(vi))
			//{
			//	vpi.Color.r = mpi->mColors[vi]->r;
			//	vpi.Color.g = mpi->mColors[vi]->g;
			//	vpi.Color.b = mpi->mColors[vi]->b;
			//	vpi.Color.a = mpi->mColors[vi]->a;

			//}
			_Mesh->Vertices.push_back(vpi);
		}
		//load element
		for (unsigned int fi = 0; fi < mpi->mNumFaces; fi++)
		{
			for (unsigned int ei = 0; ei < mpi->mFaces[fi].mNumIndices; ei++)
			{
				_Mesh->Elements.push_back(mpi->mFaces[fi].mIndices[ei]);
			}
		}

	}
	//processe children nodes &load next meshes
	for (unsigned int i = 0; i < npi->mNumChildren; i++)
	{
		ProcesseLoad(npi->mChildren[i]);
	}
}
void  SP_Object::LoadOriginTexture()
{
}

//void SP_Object::ActiveTexture(int Index)
//{
//	glBindTexture(GL_TEXTURE_2D,_Material->Textures[Index].ID);
//	glActiveTexture(Index);
//	glBindTexture(GL_TEXTURE_2D,0);
//}
//
//void SP_Object::ActiveAllTexture()
//{
//	for (vector<SP_TextureInfo>::iterator vi = _Material->Textures.begin(); vi != _Material->Textures.end(); vi++)
//	{
//		ActiveTexture(vi->ID);
//	}
//}

SP_Camera::SP_Camera(MainShaderPractice *root, ConfigStream *CStream)
{
	ID = root->CameraID;
	root->CameraID++;
	_ConfigData = CStream;
	_Transform = new SP_Transform(root, NULL);
}
SP_Camera::~SP_Camera()
{
	//_Transform->~SP_Transform();
	delete(_Transform);
}
void SP_Camera::SetUpProjectionMat()
{
	if (IsRelatedToResolution)
	{
		ProjectionWidth = _ConfigData->SCR_WIDTH;
		ProjectionHight = _ConfigData->SCR_HEIGHT;
	}
	if (!IsOrthogonalViewMat)
	{
		ProjectionMat = glm::perspective<float>
			(viewAngle*glm::pi<float>() / 180.0f,
			(float)ProjectionWidth / (float)ProjectionHight,
				0.1f, ProjectionDistance);
	}
	else
	{
		ProjectionMat = glm::ortho<float>
		(
			-viewAngle, viewAngle, -viewAngle, viewAngle,
			0.0f,
			ProjectionDistance
			);
	}

}
void SP_Camera::TranslateCam(float x, float y, float z)
{
	_Transform->position += glm::vec3(x, y, z);
	SetUpViewMat();
}
void SP_Camera::RotateCam(float pitch, float yoll)
{
	if ((_Transform->eulerAngle.x + pitch) > 89)
	{
		pitch = 0;
		_Transform->eulerAngle.x = 89;
	}
	if ((_Transform->eulerAngle.x + pitch) <- 89)
	{
		pitch = 0;
		_Transform->eulerAngle.x = -89;
	}

	_Transform->EulerRotate(pitch, yoll, 0);
	_Transform->DebugLog();
	SetUpViewMat();
}
void SP_Camera::DefaultInit()
{
	_Transform->position = glm::vec3(0, 0, -5);
	SetUpProjectionMat();
	SetUpViewMat();
}
void SP_Camera::SetUpViewMat()
{
	ViewMat = glm::lookAt
		(_Transform->position, 
		_Transform->position + glm::vec3(_Transform->quaternion.x,
			_Transform->quaternion.y, _Transform->quaternion.z), 
		viewUP);
}

SP_LightBase::SP_LightBase(MainShaderPractice *root)
{
	ID = root->LightID;
	root->LightID++;
	Root = root;
	_Transform = new SP_Transform(root,NULL);
	_ShadowDepthMap = new SP_TextureInfo();
	_DepthFBO = new SP_FBObj(Root);
	_Program_Shadow = new SP_Program(Root);
	InitDepthTex_FBO();
	BindFBOToDepthTex();
}
SP_LightBase::~SP_LightBase()
{
	//_Program_Shadow->~SP_Program();
	delete(_Program_Shadow);

	//_DepthFBO->~SP_FBObj();
	delete(_DepthFBO);

	//_Transform->~SP_Transform();
	delete(_Program_Shadow);

	delete(_Program_Shadow);
}
void SP_LightBase::SetUpProjectionMat()
{
	ProjectionMat = glm::ortho<float>
			(-DepthTexWide,
			DepthTexWide,
			-DepthTexHeight,
			DepthTexHeight, 
			0.0f,
			10.0f);
}
void SP_LightBase::SetUpViewMat()
{
	ViewMat = glm::lookAt
		(_Transform->position,
		_Transform->position + glm::vec3(_Transform->quaternion.x,_Transform->quaternion.y, _Transform->quaternion.z), 
		glm::vec3(0.0f,1.0f,0.0f));
}

void SP_LightBase::InitDepthTex_FBO()
{
	InitDepthMapData();

	glGenTextures(1, &_ShadowDepthMap->gl_ID);
	glBindTexture(GL_TEXTURE_2D, _ShadowDepthMap->gl_ID);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_DEPTH_COMPONENT,
			DepthPixle,
			DepthPixle,
			0,
			GL_DEPTH_COMPONENT,
			GL_FLOAT,
			//NULL);
			_ShadowDepthMap->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_LEQUAL);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenFramebuffers(1, &_DepthFBO->glID);
		glBindFramebuffer(GL_FRAMEBUFFER, _DepthFBO->glID);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _ShadowDepthMap->gl_ID, 0);
			glDrawBuffer(GL_NONE);
			//glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void SP_LightBase::BindFBOToDepthTex()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _DepthFBO->glID);
		glFramebufferTexture2D
			(GL_FRAMEBUFFER, 
			GL_DEPTH_ATTACHMENT, 
			GL_TEXTURE_2D, 
			_ShadowDepthMap->gl_ID, 
			0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void SP_LightBase::ActiveTexToGetDepth()
{
	glActiveTexture(GL_TEXTURE0);
	glBindFramebuffer(GL_FRAMEBUFFER, _DepthFBO->glID);
	glBindTexture(GL_TEXTURE_2D, _ShadowDepthMap->gl_ID);
}
void SP_LightBase::InitDepthProgram()
{
	_Program_Shadow->ShaderSources.push_back(ShaderDataStream("ShadowMapVs.vert",GL_VERTEX_SHADER));
	_Program_Shadow->ShaderSources.push_back(ShaderDataStream("ShadowMapFs.frag", GL_FRAGMENT_SHADER));
	_Program_Shadow->Init();
}

void SP_LightBase::InitDepthMapData()
{
	_ShadowDepthMap->data = (void*)malloc(DepthPixle*DepthPixle *sizeof(float));
}

SP_LightAmbient::SP_LightAmbient(MainShaderPractice *root): SP_LightBase(root)
{
	_Transform->EulerRotate(0,0,0);
}
SP_LightAmbient::~SP_LightAmbient()
{

}
SP_LightPoint::SP_LightPoint(MainShaderPractice *root) : SP_LightBase(root)
{
	_Transform->position = glm::vec3(0, 0, 0);
}
SP_LightPoint::~SP_LightPoint()
{

}

SP_RendererFactory::SP_RendererFactory(MainShaderPractice *root)
{
	Root = root;
	ID = Root->RendererBoxID;
	Root->RendererBoxID++;
}

SP_RendererFactory::~SP_RendererFactory()
{

}

void SP_RendererFactory::LoadAllSceneObj()
{
	for (list<SP_SceneBase*>::iterator si = Scenes.begin(); si != Scenes.end(); si++)
	{
		AlphaVL.push_back(&(**si).Alpha);
		OpaqueVL.push_back(&(**si).Opaque);
		LightVL.push_back(&(**si).Lights);
	}
}

void SP_RendererFactory::LoadAllSceneObjToFLoop()
{
	ThreadFixedLoop_->AlphaVL_AMB_ = &AlphaVL;
	ThreadFixedLoop_->AlphaVL_SDM_ = &AlphaVL;
	ThreadFixedLoop_->AlphaVL_SDM_ = &OpaqueVL;
	ThreadFixedLoop_->LightL_SDM_ = &LightVL;
}

void SP_RendererFactory::DrawAll()
{
	if (IsCullBack)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	switch (R_Mode)
	{
	case(SP_RendererMODE_DEPTH):
		Draw_DEPTH();
		break;
	case(SP_RendererMODE_BLEND):
		Draw_BLEND();
		break;
	case(SP_RendererMODE_DEPTH_BLEND):
		Draw_DEPTH_BLEND();
		break;
	case(SP_RendererMODE_QUEUE_BLEND):
		break;
	default:
		break;
	}
}

void SP_RendererFactory::DrawAllOpaque()
{
	for (vector<list<SP_Object*>*>::iterator o_Li = OpaqueVL.begin(); o_Li != OpaqueVL.end(); o_Li++)
	{
		for (list<SP_Object*>::iterator oi = (**o_Li).begin(); oi != (**o_Li).end(); oi++)
		{
			if ((**oi).IsOpaque)
				(**oi).Draw(_Cam);
		}
	}
}

void SP_RendererFactory::DrawAllAlpha()
{
	for (vector<list<SP_Object*>*>::iterator a_Li = AlphaVL.begin(); a_Li != AlphaVL.end(); a_Li++)
	{
		for (list<SP_Object*>::iterator oi = (**a_Li).begin(); oi != (**a_Li).end(); oi++)
		{
			if (!(**oi).IsOpaque)
				(**oi).Draw(_Cam);
		}
	}
}

void SP_RendererFactory::DrawAllAlphaByOrder()
{
	if(_AlphaM)
	for (std::map<float, SP_Object*>::reverse_iterator  oi = _AlphaM->rbegin(); oi != _AlphaM->rend();oi++)
	{
		if (!(*oi).second->IsOpaque)
			(*oi).second->Draw(_Cam);

		//cout << "Has DRAW " << endl;
	}
}

void SP_RendererFactory::DrawToFrameBuffer()
{
}

void SP_RendererFactory::Draw_DEPTH_BLEND()
{
	glStencilMask(0xff);
	glClear(GL_STENCIL_BUFFER_BIT);
	Draw_STENCIL();


	glEnable(GL_DEPTH_TEST);
	DrawAllOpaque();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);


	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);
	if (ThreadFixedLoop_&&ThreadFixedLoop_->IsActive&&!ThreadFixedLoop_->LockAlphaMapBuild&&ThreadFixedLoop_->out_AlphaM_)
	{
		ThreadFixedLoop_->LockAlphaMapBuild = true;
		ThreadFixedLoop_->AlphaVL_AMB_ = &AlphaVL;
		_AlphaM = ThreadFixedLoop_->out_AlphaM_;
		DrawAllAlphaByOrder();
		ThreadFixedLoop_->LockAlphaMapBuild = false;
	}
	else
		DrawAllAlpha();


	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawAllAlpha();
	glDisable(GL_BLEND);

	DrawAllOpaque();
	glDisable(GL_DEPTH_TEST);
}

void SP_RendererFactory::Draw_DEPTH()
{
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	for (vector<list<SP_Object*>*>::iterator o_Li = OpaqueVL.begin(); o_Li != OpaqueVL.end(); o_Li++)
	{
		for (list<SP_Object*>::iterator oi = (**o_Li).begin(); oi != (**o_Li).end(); oi++)
		{
			if ((**oi).IsOpaque)
				(**oi).Draw(_Cam);
		}
	}
	
	for (vector<list<SP_Object*>*>::iterator a_Li = AlphaVL.begin(); a_Li != AlphaVL.end(); a_Li++)
	{
		for (list<SP_Object*>::iterator oi = (**a_Li).begin(); oi != (**a_Li).end(); oi++)
		{
			if (!(**oi).IsOpaque)
				(**oi).Draw(_Cam);
		}
	}
	glDisable(GL_DEPTH_TEST);
}

void SP_RendererFactory::Draw_BLEND()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	for (vector<list<SP_Object*>*>::iterator o_Li = OpaqueVL.begin(); o_Li != OpaqueVL.end(); o_Li++)
	{
		for (list<SP_Object*>::iterator oi = (**o_Li).begin(); oi != (**o_Li).end(); oi++)
		{
			if ((**oi).IsOpaque)
				(**oi).Draw(_Cam);
		}
	}
	for (vector<list<SP_Object*>*>::iterator a_Li = AlphaVL.begin(); a_Li != AlphaVL.end(); a_Li++)
	{
		for (list<SP_Object*>::iterator oi = (**a_Li).begin(); oi != (**a_Li).end(); oi++)
		{
			if (!(**oi).IsOpaque)
				(**oi).Draw(_Cam);
		}
	}
	glDisable(GL_BLEND);
}

void SP_RendererFactory::Draw_STENCIL()
{
	for (vector<list<SP_Object*>*>::iterator o_Li = OpaqueVL.begin(); o_Li != OpaqueVL.end(); o_Li++)
	{
		for (list<SP_Object*>::iterator oi = (**o_Li).begin(); oi != (**o_Li).end(); oi++)
		{
			if ((**oi).IsOpaque&& (**oi).ActiveStencilWrite)
				(**oi).Draw(_Cam);
		}
	}
	for (vector<list<SP_Object*>*>::iterator a_Li = AlphaVL.begin(); a_Li != AlphaVL.end(); a_Li++)
	{
		for (list<SP_Object*>::iterator oi = (**a_Li).begin(); oi != (**a_Li).end(); oi++)
		{
			if (!(**oi).IsOpaque&&(**oi).ActiveStencilWrite)
				(**oi).Draw(_Cam);
		}
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_ALPHA_BITS);
}

#pragma region Draw_STENCIL

//
//void RendererFactory::Draw_STENCIL()
//{
//
//	for (list<SP_Object*>::iterator oi = Opaque.begin(); oi != Opaque.end(); oi++)
//	{
//		if((**oi).IsStencil)
//			(**oi).Draw(_Cam);
//	}
//	for (list<SP_Object*>::iterator oi = Alpha.begin(); oi != Alpha.end(); oi++)
//	{
//		if ((**oi).IsStencil)
//			(**oi).Draw(_Cam);
//	}
//
//}

#pragma endregion
SP_SceneBase::SP_SceneBase(MainShaderPractice *root)
{
	Root = root;
	ID = Root->RendererBoxID+1;
	Root->RendererBoxID++;
}

SP_SceneBase::~SP_SceneBase()
{
	DestroyAllObj();
}

void SP_SceneBase::DestroyAllObj()
{
	for (list<SP_Object*>::iterator oi=Alpha.begin();oi!=Alpha.end();oi++ )
	{
		if (*oi)
		{
			delete(*oi);
		}
	}
	for (list<SP_Object*>::iterator oi = Opaque.begin(); oi != Opaque.end(); oi++)
	{
		if (*oi)
		{
			delete(*oi);
		}
	}
	for (list<SP_LightBase*>::iterator oi = Lights.begin(); oi != Lights.end(); oi++)
	{
		if (*oi)
		{
			delete(*oi);
		}
	}
	for (list<SP_CubeTex*>::iterator oi = SkyBoxs.begin(); oi != SkyBoxs.end(); oi++)
	{
		if (*oi)
		{
			delete(*oi);
		}
	}
}

SP_Scene1::SP_Scene1(MainShaderPractice *root):SP_SceneBase(root)
{
	 _Sun = new SP_LightBase(root);
	 Lights.push_back(_Sun);

	 _Plight =new SP_LightBase(root);
	 //Lights.push_back(_Plight);
#pragma region Skybox1
	 _SkyBox1 = new SP_CubeTex(Root);
	 SkyBoxs.push_back(_SkyBox1);
#pragma endregion


#pragma region Cloth
	 _ObjCloth = new SP_Object(Root);
	 //Obj0.DefaultInit(SP_DefaultMesh_CUBE);

	 _ObjCloth->_Material = new SP_Material(Root);
	 _ObjCloth->_Transform = new SP_Transform(Root, NULL);
	 _ObjCloth->_Material->_Program = new SP_Program(Root);
	 _ObjCloth->_Mesh = new SP_Mesh(Root);

	 _ObjCloth->_Material->_Program->ShaderSources.push_back(ShaderDataStream("FakeCloth.vert", GL_VERTEX_SHADER));
	 _ObjCloth->_Material->_Program->ShaderSources.push_back(ShaderDataStream("FakeCloth.frag", GL_FRAGMENT_SHADER));
	 _ObjCloth->_Mesh->MeshInit_Plane2();
	 _ObjCloth->_Material->SetupTexture("hfut.png", 1, "main", SP_TEXTURE_CHANNEL_TYPE_RGB);


	 _ObjCloth->_Transform->position = glm::vec3(20.0f, -5.0f, 10.0f);
	 _ObjCloth->_Material->Alpha = 0.5f;
	 _ObjCloth->IsOpaque = true;

	 _ObjCloth->Init();

	 Opaque.push_back(_ObjCloth);
#pragma endregion


#pragma region obj0
	 _Obj0 =new SP_Object(Root);
	//Obj0.DefaultInit(SP_DefaultMesh_CUBE);

	_Obj0->_Material = new SP_Material(Root);
	_Obj0->_Transform = new SP_Transform(Root, NULL);
	_Obj0->_Material->_Program = new SP_Program(Root);
	_Obj0->_Mesh = new SP_Mesh(Root);

	_Obj0->_Material->_Program->ShaderSources.push_back(ShaderDataStream("testShaderVs.vert", GL_VERTEX_SHADER));
	_Obj0->_Material->_Program->ShaderSources.push_back(ShaderDataStream("testShaderFs.frag", GL_FRAGMENT_SHADER));
	_Obj0->_Mesh->MeshInit_Cube();

	_Obj0->_Transform->position = glm::vec3(-4.5f, -0.5f, -0.5f);
	_Obj0->_Material->Alpha = 0.5f;
	_Obj0->IsOpaque = false;

	_Obj0->Init();

	Alpha.push_back(_Obj0);
#pragma endregion

#pragma region obj1
	 _Obj1 =new SP_Object(Root);

	_Obj1->_Material = new SP_Material(Root);
	_Obj1->_Transform = new SP_Transform(Root, NULL);
	_Obj1->_Material->_Program = new SP_Program(Root);
	_Obj1->_Mesh = new SP_Mesh(Root);

	_Obj1->_Material->_Program->ShaderSources.push_back(ShaderDataStream("testShaderVs.vert", GL_VERTEX_SHADER));
	_Obj1->_Material->_Program->ShaderSources.push_back(ShaderDataStream("testShaderFs.frag", GL_FRAGMENT_SHADER));
	_Obj1->_Mesh->MeshInit_Cube();
	_Obj1->IsOpaque = true;

	_Obj1->Init();

	Opaque.push_back(_Obj1);

	//
	_Obj1_c1 = new SP_Object(Root);

	_Obj1_c1->_Material = new SP_Material(Root);
	//_Obj1_c1->_Transform = new Transform(Root, _Obj1->_Transform);
	_Obj1_c1->_Transform = new SP_Transform(Root,NULL);
	_Obj1_c1->_Material->_Program = new SP_Program(Root);
	_Obj1_c1->_Mesh = new SP_Mesh(Root);

	_Obj1_c1->_Material->_Program->ShaderSources.push_back(ShaderDataStream("testShaderVs.vert", GL_VERTEX_SHADER));
	_Obj1_c1->_Material->_Program->ShaderSources.push_back(ShaderDataStream("testShaderFs.frag", GL_FRAGMENT_SHADER));
	_Obj1_c1->_Mesh->MeshInit_Cube();
	_Obj1_c1->IsOpaque = false;

	_Obj1_c1->Init();

	Alpha.push_back(_Obj1_c1);
#pragma endregion

#pragma region Obj2_Door
	_Obj2_Door = new SP_Object(Root);

	_Obj2_Door->_Material = new SP_Material(Root);
	_Obj2_Door->_Transform = new SP_Transform(Root, NULL);
	_Obj2_Door->_Material->_Program = new SP_Program(Root);
	_Obj2_Door->_Mesh = new SP_Mesh(Root);

	_Obj2_Door->_Material->_Program->ShaderSources.push_back(ShaderDataStream("stencilVs.vert", GL_VERTEX_SHADER));
	_Obj2_Door->_Material->_Program->ShaderSources.push_back(ShaderDataStream("stencilFs.frag", GL_FRAGMENT_SHADER));
	_Obj2_Door->_Mesh->MeshInit_Cube();
	_Obj2_Door->_Material->SetupTexture("Door.png", 0, "main", SP_TEXTURE_CHANNEL_TYPE_RGBA);
	_Obj2_Door->IsOpaque = false;

	_Obj2_Door->Init();

	Alpha.push_back(_Obj2_Door);
#pragma endregion


	_testObj0 =new SP_Object(Root);
	_testObj0->DefaultInit(SP_DefaultMesh_CUBE);

	_testObj1 = new SP_Object(Root);
	_testObj1->DefaultInit(SP_DefaultMesh_CUBE);

	_testObj2 = new SP_Object(Root);
	_testObj2->DefaultInit(SP_DefaultMesh_CUBE);

	_testObj3 = new SP_Object(Root);
	_testObj3->DefaultInit(SP_DefaultMesh_PLANE);

	_testObj4 = new SP_Object(Root);
	_testObj4->DefaultInit(SP_DefaultMesh_SHPERE);

	_testObj5 = new SP_Object(Root);
	_testObj5->DefaultInit(SP_DefaultMesh_SHPERE);

	Opaque.push_back(_testObj3);
	Opaque.push_back(_testObj4);
	Opaque.push_back(_testObj5);
	Opaque.push_back(_testObj2);
	Alpha.push_back(_testObj1);
	Opaque.push_back(_testObj0);
}

void SP_Scene1::Start()
{

	_Plight->_Transform->position = glm::vec3(-5, 0, 5);
	_Plight->InitDepthProgram();

	_ObjCloth->_Transform->scale = glm::vec3(0.1f, 0.1f, 0.1f);
	_ObjCloth->dMode = OBJ_DRAW_LINE;

	_Sun->_Transform->position = glm::vec3(0.0f, 0.0f, -5.0f);
	_Sun->_Transform->EulerRotate(10.0f, 0.0f, 0.0f);
	_Sun->InitDepthProgram();

	_Obj1->_Transform->position = glm::vec3(-4.0f, -1.0f, -1.0f);
	_Obj1->_Transform->EulerRotate(30.0f,0.0f,0.0f);
	_Obj1->_Material->Alpha = 0.5f;

	_Obj1_c1->_Transform->position = glm::vec3(0.0f, 0.2f, 0.0f);
	_Obj1_c1->_Material->Alpha = 0.3f;


	_testObj1->_Transform->position = glm::vec3(-5.0f, 0.0f, 0.0f);
	_testObj1->_Material->Alpha = 0.5f;
	_testObj1->IsOpaque = false;

	_testObj2->_Transform->position = glm::vec3(5.0f,5.0f,5.0f);
	_testObj2->_Transform->scale = glm::vec3(1.0f,2.0f,3.0f);

	_testObj3->_Transform->position = glm::vec3(-2.5f, -2.0f, 0.0f);

	_testObj4->_Transform->position = glm::vec3(0.0f, -2.0f, -3.0f);
	_testObj4->_Transform->scale = glm::vec3(1.0f,1.5f,0.75f);

	_testObj5->_Transform->position = glm::vec3(-5.0f,-1.0f,-5.0f);

	_Obj2_Door->_Transform->position = _Obj1->_Transform->position + glm::vec3(0.0f,3.0f,-0.1f);
	//_Obj2_Door->_Transform->EulerRotate(90.0f, 0.0f, 0.0f);
	_Obj2_Door->_Transform->scale = glm::vec3(1.0f)+glm::vec3(0.01f);

	_Obj2_Door->ActiveStencilWrite = true;
	_Obj0->StencilFunc = GL_ALWAYS;
	_Obj2_Door->StencilRef =1;
	_Obj2_Door->StencilReadMask = 0x00;
	_Obj2_Door->StencilWriteMask = 0xff;
	_Obj2_Door->StencilOp_DSFail = GL_REPLACE;
	_Obj2_Door->StencilOp_DFail = GL_REPLACE;
	_Obj2_Door->StencilOp_Pass = GL_REPLACE;
}

void SP_Scene1::OnUpdate()
{

	_testObj2->_Transform->position += glm::vec3(glm::sin(glfwGetTime()) / 40.0f, 0.0f, 0.0f);
	_testObj0->_Transform->EulerRotate(0.05f, 0.01f, 0.015f);
	_ObjCloth->_Material->_Program->setFloat("num", glfwGetTime());
	int i = glfwGetTime();
	//if (i%3==0)
	//{
	//	//_ObjCloth->_Mesh->MeshInit_Plane2();
	//	//_ObjCloth->_Mesh->Init();
	//	_ObjCloth->_Mesh->Vertices[1].Position = glm::vec3(1, 10, 1);
	//	_ObjCloth->_Mesh->Init();
	//}
	//else if(i % 3 == 1)
	//{
	//	_ObjCloth->_Mesh->Vertices[1].Position = glm::vec3(1, 10, 10);
	//	_ObjCloth->_Mesh->Init();
	//}
	//else if (i % 3 == 2)
	//{
	//	_ObjCloth->_Mesh->Vertices[1].Position = glm::vec3(1, 1, 1);
	//	_ObjCloth->_Mesh->Init();
	//}

}

void SP_Scene1::End()
{
}

SP_Scene1::~SP_Scene1()
{
}

SP_Scene2::SP_Scene2(MainShaderPractice * root):SP_SceneBase(root)
{
#pragma region _Obj0
	_Obj0 = new SP_Object(Root);

	_Obj0->_Material = new SP_Material(Root);
	_Obj0->_Transform = new SP_Transform(Root, NULL);
	_Obj0->_Material->_Program = new SP_Program(Root);
	_Obj0->_Mesh = new SP_Mesh(Root);

	_Obj0->_Material->_Program->ShaderSources.push_back(ShaderDataStream("stencilVs.vert", GL_VERTEX_SHADER));
	_Obj0->_Material->_Program->ShaderSources.push_back(ShaderDataStream("stencilFs.frag", GL_FRAGMENT_SHADER));
	_Obj0->_Mesh->MeshInit_Cube();
	_Obj0->_Material->SetupTexture("SceneTestTex.png", 0, "main", SP_TEXTURE_CHANNEL_TYPE_RGBA);
	_Obj0->IsOpaque = true;

	_Obj0->Init();

	Opaque.push_back(_Obj0);
#pragma endregion

#pragma region _Obj1
	_Obj1 = new SP_Object(Root);

	_Obj1->_Material = new SP_Material(Root);
	_Obj1->_Transform = new SP_Transform(Root, NULL);
	_Obj1->_Material->_Program = new SP_Program(Root);
	_Obj1->_Mesh = new SP_Mesh(Root);

	_Obj1->_Material->_Program->ShaderSources.push_back(ShaderDataStream("stencilVs.vert", GL_VERTEX_SHADER));
	_Obj1->_Material->_Program->ShaderSources.push_back(ShaderDataStream("stencilFs.frag", GL_FRAGMENT_SHADER));
	_Obj1->_Mesh->MeshInit_Cube();
	_Obj1->_Material->SetupTexture("SceneTestTex.png", 0, "main", SP_TEXTURE_CHANNEL_TYPE_RGBA);
	_Obj1->IsOpaque = true;

	_Obj1->Init();

	Opaque.push_back(_Obj1);
#pragma endregion

#pragma region _Obj2
	_Obj2 = new SP_Object(Root);

	_Obj2->_Material = new SP_Material(Root);
	_Obj2->_Transform = new SP_Transform(Root, NULL);
	_Obj2->_Material->_Program = new SP_Program(Root);
	_Obj2->_Mesh = new SP_Mesh(Root);

	_Obj2->_Material->_Program->ShaderSources.push_back(ShaderDataStream("stencilVs.vert", GL_VERTEX_SHADER));
	_Obj2->_Material->_Program->ShaderSources.push_back(ShaderDataStream("stencilFs.frag", GL_FRAGMENT_SHADER));
	_Obj2->_Mesh->MeshInit_Cube();
	_Obj2->_Material->SetupTexture("SceneTestTex.png", 0, "main", SP_TEXTURE_CHANNEL_TYPE_RGBA);
	_Obj2->IsOpaque = true;

	_Obj2->Init();

	Opaque.push_back(_Obj2);
#pragma endregion

}
SP_Scene2::~SP_Scene2()
{
}

void SP_Scene2::Start()
{
	_Obj0->_Transform->position = glm::vec3(-10.0f,0.0f,0.0f);

	_Obj0->StencilFunc = GL_EQUAL;
	_Obj0->StencilRef = 1;

	_Obj0->StencilRef = 1;
	_Obj0->StencilReadMask = 0xff;
	_Obj0->StencilWriteMask = 0xff;
	_Obj0->StencilOp_DSFail = GL_KEEP;
	_Obj0->StencilOp_DFail = GL_KEEP;
	_Obj0->StencilOp_Pass = GL_KEEP;

	_Obj1->_Transform->position = glm::vec3(-10.0f, 0.0f, 0.0f);

	_Obj1->StencilFunc = GL_EQUAL;
	_Obj1->StencilRef = 1;

	_Obj1->StencilRef = 1;
	_Obj1->StencilReadMask = 0xff;
	_Obj1->StencilWriteMask = 0xff;
	_Obj1->StencilOp_DSFail = GL_KEEP;
	_Obj1->StencilOp_DFail = GL_KEEP;
	_Obj1->StencilOp_Pass = GL_KEEP;


	_Obj2->StencilFunc = GL_EQUAL;
	_Obj2->StencilRef = 1;

	_Obj2->StencilRef = 1;
	_Obj2->StencilReadMask = 0xff;
	_Obj2->StencilWriteMask = 0xff;
	_Obj2->StencilOp_DSFail = GL_KEEP;
	_Obj2->StencilOp_DFail = GL_KEEP;
	_Obj2->StencilOp_Pass = GL_KEEP;


}

void SP_Scene2::OnUpdate()
{
	_Obj0->_Transform->EulerRotate(0.01f,0.05f,0.03f);
	_Obj1->_Transform->position += glm::vec3(0.0f,glm::sin(glfwGetTime()) / 40.0f,  0.0f);
}

void SP_Scene2::End()
{
}

SP_ScreenMasker::SP_ScreenMasker()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);

	SP_Vertex vi = SP_Vertex();
	vi.Position = glm::vec3(-1.0f, -1.0f, 0.0f); vi.TexCoords = glm::vec2(0.0f, 0.0f);
	Vertices.push_back(vi);
	vi.Position = glm::vec3(1.0f, -1.0f, 0.0f); vi.TexCoords = glm::vec2(1.0f, 0.0f);
	Vertices.push_back(vi);
	vi.Position = glm::vec3(-1.0f, 1.0f, 0.0f); vi.TexCoords = glm::vec2(0.0f, 1.0f);
	Vertices.push_back(vi);
	vi.Position = glm::vec3(1.0f, 1.0f, 0.0f); vi.TexCoords = glm::vec2(1.0f, 1.0f);
	Vertices.push_back(vi);
	vi.Position = glm::vec3(-1.0f, 1.0f, 0.0f); vi.TexCoords = glm::vec2(0.0f, 1.0f);
	Vertices.push_back(vi);
	vi.Position = glm::vec3(1.0f, -1.0f, 0.0f); vi.TexCoords = glm::vec2(1.0f, 0.0f);
	Vertices.push_back(vi);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(SP_Vertex), &(Vertices.front()), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::Position));
	//n=1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::TexCoords));
	//t=2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::Normal));
	//c=3
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::Tangent));
	//4
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(SP_Vertex), (void*)offsetof(SP_Vertex, SP_Vertex::BiTangent));

	glBindVertexArray(0);

	_Program = new SP_Program();
	_Program->ShaderSources.push_back(ShaderDataStream("SCleanvs.vert",GL_VERTEX_SHADER));
	_Program->ShaderSources.push_back(ShaderDataStream("SCleanfs.frag", GL_FRAGMENT_SHADER));
	_Program->Init();

}

SP_ScreenMasker::~SP_ScreenMasker()
{
	if (_Program)
	{
		delete(_Program);
	}
}

void SP_ScreenMasker::CleanStencil()
{
	glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
	glStencilFunc(GL_ALWAYS, 0, 0xff);
	glStencilMask(0xff);

	glUseProgram(_Program->ID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0,Vertices.size());
	glBindVertexArray(0);

	glStencilMask(0x00);
}

void SP_ScreenMasker::DrawIndicateTex(GLuint Texi)
{


	glDisable(GL_DEPTH_TEST);


	glViewport(0, 0, 1600, 900);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texi);
		glUseProgram(_Program->ID);
		_Program->setInt("targetTex", 0);
		glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

SP_Thread::SP_Thread()
{
}

SP_Thread::~SP_Thread()
{

}

void SP_Thread::run()
{
	if (!(*IsActive))
	{
		//cout << " ok??????????  1" << endl << endl << endl;
		*IsActive = true;
		th->detach();
	}
}

SP_Thread_FixedLoop::SP_Thread_FixedLoop()
{
	LightL_SDM_ = new vector<list<SP_LightBase*>*>();
	AlphaVL_SDM_=new vector<list<SP_Object*>*>();
	OpaqueVL_SDM_=new vector<list<SP_Object*>*>();
	out_AlphaM_ = new map<float, SP_Object*>();
	AlphaVL_AMB_=new vector<list<SP_Object*>*>();
}

SP_Thread_FixedLoop::~SP_Thread_FixedLoop()
{
}

void SP_Thread_FixedLoop::InitAndRun()
{
	_this = this;
	th =new thread(&SP_Thread_FixedLoop::FixedLoop,this);
	//Funcs.push_back(&Thread_FixedLoop::test);

	Funcs.push_back(&SP_Thread_FixedLoop::AlphaMapBuild);
	//Funcs.push_back(&SP_Thread_FixedLoop::SetUpDepthMap);

	run();
}

void SP_Thread_FixedLoop::FixedLoop()
{
	while (true)
	{
		//cout << " is it ok??????????  2" << endl << endl << endl;
		for (list<void(SP_Thread_FixedLoop::*)()>::iterator fi = Funcs.begin(); fi != Funcs.end(); fi++)
		{
			//cout << " is it really ok??????????  3" << endl << endl << endl;
			(*_this.**fi)();
		}
		Sleep(100);
	}
}

void SP_Thread_FixedLoop::test()
{
	//cout << " is ok!!!!!!" << endl << endl << endl << endl << endl << endl << endl;
}

void SP_Thread_FixedLoop::AlphaMapBuild()
{
	if (!LockAlphaMapBuild)
	{
		LockAlphaMapBuild = true;
		out_AlphaM_= new map<float, SP_Object*>();
		if(AlphaVL_AMB_)
		for (vector<list<SP_Object*>*>::iterator a_Li = AlphaVL_AMB_->begin(); a_Li != AlphaVL_AMB_->end(); a_Li++)
		{
			for (list<SP_Object*>::iterator oi = (**a_Li).begin(); oi != (**a_Li).end(); oi++)
			{
					float distance = glm::length(Cam_AMB_->_Transform->WorldPos() - (**oi)._Transform->WorldPos());
					(*out_AlphaM_)[distance] = (*oi);
			}
		}
		LockAlphaMapBuild = false;
	}

}

void SP_Thread_FixedLoop::SetUpDepthMap()
{
	if (!LockSetUpDepthMap)
	{
		LockSetUpDepthMap = true;
		if(LightL_SDM_)
		for (vector<list<SP_LightBase*>*>::iterator VLi = LightL_SDM_->begin(); VLi != LightL_SDM_->end(); VLi++)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glClear(GL_DEPTH_BUFFER_BIT);

			for (list<SP_LightBase*>::iterator Li = (**VLi).begin(); Li != (**VLi).end(); Li++)
			{
				glClear(GL_DEPTH_BUFFER_BIT);

				glViewport(0, 0, (*Li)->DepthPixle, (*Li)->DepthPixle);
				//glViewport(0, 0, 1600, 900);
				(*Li)->SetUpProjectionMat();
				(*Li)->SetUpViewMat();

				//glBindFramebuffer(GL_FRAMEBUFFER, (*Li)->_DepthFBO->glID);
				//glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, (*Li)->_ShadowDepthMap->gl_ID);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, (*Li)->_ShadowDepthMap->gl_ID, 0);
				if (AlphaVL_SDM_)
					for (vector<list<SP_Object*>*>::iterator a_Li = AlphaVL_SDM_->begin(); a_Li != AlphaVL_SDM_->end(); a_Li++)
					{
						for (list<SP_Object*>::iterator oi = (**a_Li).begin(); oi != (**a_Li).end(); oi++)
						{
							if ((**oi).HasShadow)
								(**oi).DepthDraw(*Li);
						}
					}
				if (OpaqueVL_SDM_)
					for (vector<list<SP_Object*>*>::iterator o_Li = OpaqueVL_SDM_->begin(); o_Li != OpaqueVL_SDM_->end(); o_Li++)
					{
						for (list<SP_Object*>::iterator oi = (**o_Li).begin(); oi != (**o_Li).end(); oi++)
						{
							if ((**oi).HasShadow)
								(**oi).DepthDraw(*Li);
						}
					}
				//glBindFramebuffer(GL_FRAMEBUFFER, 0);
				//glBindTexture(GL_TEXTURE_2D, 0);
				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
			glDisable(GL_DEPTH_TEST);

		}
		glDepthMask(GL_FALSE);
		LockSetUpDepthMap = false;
	}
	
}

SP_FBObj::SP_FBObj(MainShaderPractice * IDList)
{
	spID = IDList->FBOID;
	IDList->FBOID++;
}

SP_FBObj::~SP_FBObj()
{
}


SP_MotionBlur::SP_MotionBlur(MainShaderPractice *root)
{
	Root = root;
	ID = Root->RendererBoxID;
	Root->RendererBoxID++;
}

SP_MotionBlur::~SP_MotionBlur()
{
}

void SP_MotionBlur::Blur()
{
}

void SP_MotionBlur::ReadScrPixle()
{
}

void SP_MotionBlur::Init()
{
	_TexInfos = new vector<SP_TextureInfo>();




}

SP_PBObj::SP_PBObj(MainShaderPractice * IDList)
{
	spID = IDList->PBOID;
	IDList->PBOID ++ ;
}

SP_PBObj::~SP_PBObj()
{
}


SP_CubeTex::SP_CubeTex(MainShaderPractice *root):SP_Object(root)
{
	ID = root->CubeTexID;
	root->CubeTexID++;
}

SP_CubeTex::~SP_CubeTex()
{

}

void SP_CubeTex::DefaultInit()
{
}

void SP_CubeTex::Init()
{
}

void SP_CubeTex::LoadCubeTex()
{
}

void SP_CubeTex::BuildCubeByAllDirTex()
{
}


SP_ScrBufferPass::SP_ScrBufferPass(MainShaderPractice *root)
{

}

SP_ScrBufferPass::~SP_ScrBufferPass()
{
	delete(Texturei);
	glDeleteBuffers(1,&gl_BufferID);
}

void SP_ScrBufferPass::Init()
{

}

void SP_ScrBufferPass::GetScrBuffer(int GB_type)
{
	glGenFramebuffers(1, &gl_BufferID);
	glBindFramebuffer(GL_FRAMEBUFFER,gl_BufferID);
	switch (GB_type)
	{
	case(GEOMETRY_FRAME_BUFFER_DEPTH):
		break;
		default:
			break;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteFramebuffers(1, &gl_BufferID);


}

void SP_ScrBufferPass::PassBuffer()
{

}

void SP_ScrBufferPass::GetDefaultBuffer_test()
{
	glGenBuffers(1, &gl_BufferID);
	glBindBuffer(GL_FRAMEBUFFER, gl_BufferID);
		//glBufferData(GL_FRAMEBUFFER, 1024 * 1024 * 4*sizeof(float), data, GL_FRAGMENT_DEPTH);
		
	glBindBuffer(GL_FRAMEBUFFER, 0);


}

void SP_ScrBufferPass::GetDefaultBuffer()
{

}

void SP_ScrBufferPass::BindCaptureTex()
{
	//if (data)
	//{
	//	glGenTextures(1, &gl_TexID);
	//	//glBindTexture(,0);

	//}
	//else if(u_data)
	//{
	//	 
	//}
}

void SP_ScrBufferPass::ScrBufferExpection()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		cout << "Framebuffer is complete"<<endl;
}


SP_TextureCompress::SP_TextureCompress()
{
}

SP_TextureCompress::~SP_TextureCompress()
{
}

SP_ScenePapar::SP_ScenePapar(MainShaderPractice* root):SP_SceneBase(root)
{
}

SP_ScenePapar::~SP_ScenePapar()
{
}

void SP_ScenePapar::Start()
{
	_Plight->_Transform->position = glm::vec3(-5, 0, 5);
	_Plight->InitDepthProgram();
}

void SP_ScenePapar::OnUpdate()
{
}

void SP_ScenePapar::End()
{
}

SP_Cloth::SP_Cloth(MainShaderPractice* root):SP_Object(root)
{


}

SP_Cloth::~SP_Cloth()
{
}

SP_Spring::SP_Spring(MainShaderPractice* root) :SP_Object(root)
{

}

SP_Spring::~SP_Spring()
{
	 
}

void SP_Spring::InitSpringMoudle()
{
}

void SP_Spring::SetUpAll()
{

}

void SP_Spring::InitAddVert()
{
	for (vector<SP_Vertex>::iterator vi = _Mesh->Vertices.begin(); vi != _Mesh->Vertices.end();vi++)
	{
		addVert.push_back(SP_AddVertex{});
	}
}

void SP_Spring::ReSetVertices()
{

}

void SP_Spring::ReSetVAO()
{
	_Mesh->BindAllGLVertices();
}

void SP_Spring::ReSetNormal()
{

}
