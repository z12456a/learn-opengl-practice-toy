#pragma once
#include "glm.hpp"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "vector"
#include "list"
#include "map"
#include "InputStream.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "thread"


using namespace std;

class MainShaderPractice
{
public:
	MainShaderPractice();
	~MainShaderPractice();

	GLuint VAOID=0;
	GLuint VBOID=0;
	GLuint EBOID=0;
	GLuint FBOID = 0;
	GLuint PBOID = 0;
	GLuint CameraID = 0;
	GLuint SamplerID = 0;
	GLuint MeshID = 0;
	GLuint ProgramID = 0;
	GLuint ShaderID = 0;
	GLuint MaterialID = 0;
	GLuint TransformID = 0;
	GLuint TextureID = 0;
	GLuint LightID = 0;
	GLuint ObjID = 0;
	GLuint RendererBoxID = 0;
	GLuint SceneID = 0;
	GLuint ThreadID = 0;
	GLuint MotionBlurID = 0;
	GLuint CubeTexID = 0;
};

class SP_VAObj
{
public:
	SP_VAObj(MainShaderPractice *IDList);
	~SP_VAObj();
	GLuint spID;
	GLuint glID;


	void Init();
private:

};

class SP_PBObj
{
public:
	SP_PBObj(MainShaderPractice *IDList);
	~SP_PBObj();
	GLuint spID;
	GLuint glID;

private:

};

class SP_VBObj
{
public:
	SP_VBObj(MainShaderPractice *IDList);
	~SP_VBObj();
	GLuint spID;
	GLuint glID;
private:

};
class SP_FBObj
{
public:
	SP_FBObj(MainShaderPractice *IDList);
	~SP_FBObj();
	GLuint spID;
	GLuint glID;
private:

};

class SP_EBObj
{
public:
	SP_EBObj(MainShaderPractice *IDList);
	~SP_EBObj();
	GLuint spID;
	GLuint glID;
private:

};

struct SP_Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2  TexCoords;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
};

enum SP_TEXTURE_CHANNEL_TYPE
{
	SP_TEXTURE_CHANNEL_TYPE_RGB,
	SP_TEXTURE_CHANNEL_TYPE_RGB8,
	SP_TEXTURE_CHANNEL_TYPE_sRGB,
	SP_TEXTURE_CHANNEL_TYPE_RGBA,
	SP_TEXTURE_CHANNEL_TYPE_sRGBA,
	SP_TEXTURE_CHANNEL_TYPE_sRGBA64,
};

class SP_Thread
{
public:
	SP_Thread();
	~SP_Thread();

	thread *th;
	bool *IsActive = new bool(false);
	void run();
private:

};

struct SP_TextureInfo
{
	/*
	example in shader:
	uniform sampler2D texture_<TexType><TypeID>
	or
	uniform samplerCube texture_<TexType(cubeTex)><TypeID>
	*/

	//example in gl:
	/*
	glGenTextures(...);
	glBindTexture(...);
		*data input*
		glTexImage...(...);
		...
		*parameter setup*
		glTexParameteri(...);
		...
	glBindTexture(...);
	*/

	GLuint gl_ID;//gl ID

	//shader name setup info
	unsigned int TypeID;//shader ID
	string TexType;
	string ShaderName;
	string FileName;

	//tex info
	int Width;
	int High;
	int RGB;
	int ChannelType= SP_TEXTURE_CHANNEL_TYPE_RGB;

	//tex memory
	int MemoryType=0;
	unsigned char *u_data;//0
	void* data;//1
};

class SP_TextureCompress
{
public:
	SP_TextureCompress();
	~SP_TextureCompress();






private:

};

enum SP_DefautMesh
{
	SP_DefaultMesh_CUBE,
	SP_DefaultMesh_PLANE,
	SP_DefaultMesh_SHPERE,
};
class SP_Mesh
{
public:
	SP_Mesh(MainShaderPractice *IDList);
	~SP_Mesh();

	GLuint ID;
	SP_VAObj* _VAO;
	SP_VBObj* _VBO;
	SP_EBObj* _EBO;

	vector<SP_Vertex> Vertices;
	vector<unsigned int> Elements;

	void DefaultMeshInit(int DefautMeshType);
	void Init();
	void BindAllGLVertices();
	void SetupAllTangent();
	void MeshInit_Cube();
	void MeshInit_Plane();
	void MeshInit_Shpere();
	void MeshInit_Plane2();

private:

};

class SP_Program
{
public:
	GLuint ID;

	list<ShaderDataStream>  ShaderSources;

	SP_Program();
	SP_Program(MainShaderPractice *IDList, list<ShaderDataStream> *shaderSources);
	SP_Program(MainShaderPractice *IDList);
	~SP_Program();


	void Init();
	void DefaulInit();
	void programDestroy();
	void consoleExpection();
	void attachAllShader();

	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value); 
	void setFloat(const std::string &name, float value); 
	void setVec2(const std::string &name, const glm::vec2 &value);
	void setVec2(const std::string &name, float x, float y);
	void setVec3(const std::string &name, const glm::vec3 &value);
	void setVec3(const std::string &name, float x, float y, float z);
	void setVec4(const std::string &name, const glm::vec4 &value);
	void setVec4(const std::string &name, float x, float y, float z, float w);
	void setMat2(const std::string &name, const glm::mat2 &mat) ;
	void setMat3(const std::string &name, const glm::mat3 &mat);
	void setMat4(const std::string &name, const glm::mat4 &mat);


private:

};

class SP_Material
{
public:
	SP_Material(MainShaderPractice *IDList);
	~SP_Material();

	MainShaderPractice *Root;
	GLuint ID;

	/*
	tip:
	Textures load order will influence Tex glID
	*/
	vector<SP_TextureInfo> Textures;
	SP_Program* _Program;
	float Alpha=1;

	void Init(SP_Program* pi);
	void Init();
	void DefaultInit();

	void SetupTexture(string fName, unsigned int TypeId, string TexType,int ChanType);
	void DefaultSetupTexture();

	void BindTextureToShader(int Index);
	void BindAllTextureToShader();

	void ActiveTextureToDraw(int index);
	void ActiveAllTextureToDraw();

	void NullProgramException();

	void BindTexture_RGB(int index);
	void BindTexture_RGBA(int index);
	void BindTexture_RGBA64(int index);
	void BindTexture_sRGB(int index);
	void BindTexture_sRGBA(int index);
	void BindTexture_sRGBA64(int index);
	void BindTexture_RGB8(int index);


private:

};

class SP_Transform
{
public:
	SP_Transform(MainShaderPractice *IDList, SP_Transform *parent);
	~SP_Transform();

	GLuint ID;

	SP_Transform* _ParentTrns;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec4 quaternion = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f,1.0f,1.0f);
	glm::vec3 eulerAngle = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec2 Direct_forward = glm::vec2(0.0f, 0.0f);
	glm::vec2 Direct_back = glm::vec2(0.0f,  0.0f);
	glm::vec2 Direct_left = glm::vec2(0.0f,  0.0f);
	glm::vec2 Direct_right = glm::vec2(0.0f, 0.0f);
	glm::vec3 rotateAxis = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 GetDirect();
	

	glm::vec3 WorldPos();
	glm::vec4 WorldQuat();

	SP_Transform* GetRootTrans();
	glm::mat4 GetRotateMat();
	glm::mat4 GetTranslateMat();
	glm::mat4 GetModleMat();
	glm::mat4 GetScaleMat();
	void RealateToEularAngle();
	void RealateToQuaternion();
	void RealateToDirect();
	void RealateToScale();
	void EulerRotate(float x,float y,float z);
	void QuatRotate(float x, float y, float z,float w);
	 
	void DebugLog();
private:
	glm::mat4 RotateMat;
	glm::mat4 TranslateMat;
	glm::mat4 ScaleMat;
};

class SP_Sampler
{
public:
	SP_Sampler(MainShaderPractice *ListID);
	~SP_Sampler();

	GLuint ID;
	

private:

};

enum SP_LIGHT_TYPE
{
	SP_LIGHT_TYPE_AMBIENT,
	SP_LIGHT_TYPE_POINT,
};
class SP_LightBase
{
public:
	SP_LightBase(MainShaderPractice *root);
	~SP_LightBase();

	MainShaderPractice *Root;
	GLuint ID;
	SP_Transform* _Transform;
	int Type;

	glm::vec3 LightColor = glm::vec3(0.95f, 0.95f, 0.90f);
	SP_Program 	*_Program_Shadow;

	SP_FBObj *_DepthFBO;
	SP_TextureInfo *_ShadowDepthMap;

	int DepthTexWide =  32;
	int DepthTexHeight =  32;
	unsigned int DepthPixle=1024;
	
	glm::mat4 ViewMat;
	glm::mat4 ProjectionMat;

	void SetUpProjectionMat();
	void SetUpViewMat();
	void BindFBOToDepthTex();
	void InitDepthTex_FBO();
	void InitDepthProgram();
	void InitDepthMapData();
	void ActiveTexToGetDepth();
};

class SP_LightAmbient:public SP_LightBase
{
public:
	SP_LightAmbient(MainShaderPractice *root);
	~SP_LightAmbient();
};

class SP_LightPoint :public SP_LightBase
{
public:
	SP_LightPoint(MainShaderPractice *root);
	~SP_LightPoint();
};

class SP_Camera 
{
public:
	SP_Camera(MainShaderPractice *root, ConfigStream *CStream);
	~SP_Camera();

	bool IsOrthogonalViewMat = false;
	bool IsRelatedToResolution = true;
	vector<SP_LightBase*> LightSourse;

	ConfigStream *_ConfigData;
	GLuint ID;
	int ProjectionWidth;
	int ProjectionHight;
	float ProjectionDistance=1000.0f;
	float viewAngle = 60.0f;
	glm::vec3 viewUP = glm::vec3(0.0f, 1.0f, 0.0f);
	SP_Transform *_Transform;

	glm::mat4 ViewMat;
	glm::mat4 ProjectionMat;
	void SetUpProjectionMat();
	void SetUpViewMat();
	void DefaultInit();
	virtual void TranslateCam(float x, float y, float z);
	virtual void RotateCam(float pitch, float yoll);



private:
};

static enum SP_DrawMode
{
	OBJ_DRAW_DEFAULT,
	OBJ_DRAW_ELEMENT,
	OBJ_DRAW_LINE,
};

class SP_ScreenMasker
{
public:
	SP_ScreenMasker();
	~SP_ScreenMasker();
	void CleanStencil();
	void DrawIndicateTex(GLuint TexID);

	vector<SP_Vertex> Vertices;
	GLuint VAO;
	GLuint VBO;
	SP_Program* _Program;
	
};

class SP_Object
{
public:
	SP_Object(MainShaderPractice *root);
	~SP_Object();

	MainShaderPractice *Root;
	int ID;

	bool IsActive = false;
	bool HasMesh = false;
	bool HasMaterial = false;
	bool IsOpaque = true;
	bool ActiveStencilWrite=false;
	bool HasShadow = true;

	int dMode= OBJ_DRAW_DEFAULT;

	int StencilRef = 0x00;
	int StencilWriteMask =0x00;
	int StencilReadMask = 0x00;
	GLenum StencilFunc = GL_ALWAYS;//
	GLenum StencilOp_DSFail = GL_KEEP;
	GLenum StencilOp_DFail = GL_KEEP;
	GLenum StencilOp_Pass = GL_KEEP;
		

	SP_Material* _Material;
	SP_Mesh *_Mesh;
	SP_Transform *_Transform;

	AssimpStream *_ObjStream;

	//void ActiveTexture(int Index);
	//void ActiveAllTexture();
	//useless code

	void DefaultInit(int MeshType);
	void Init();
	void ProcesseLoad(aiNode *npi);
	void LoadMesh(string fName);
	void LoadOriginTexture();

	void Draw(SP_Camera *DCam);

	void DefaultDraw(SP_Camera *DCam);
	void LineDraw(SP_Camera *DCam);
	void ElementDraw(SP_Camera *DCam);
	void DepthDraw(SP_LightBase *LB);
};
#pragma region SP_Object Init Example
/*
	_Obj2_Door = new SP_Object(Root);

	_Obj2_Door->_Material = new SP_Material(Root);
	_Obj2_Door->_Transform = new SP_Transform(Root, NULL);
	_Obj2_Door->_Material->_Program = new SP_Program(Root);
	_Obj2_Door->_Mesh = new SP_Mesh(Root);

	_Obj2_Door->_Material->_Program->ShaderSources.push_back(SP_ShaderDataStream("stencilVs.vert", GL_VERTEX_SHADER));
	_Obj2_Door->_Material->_Program->ShaderSources.push_back(SP_ShaderDataStream("stencilFs.frag", GL_FRAGMENT_SHADER));
	...
	_Obj2_Door->_Mesh->MeshInit_Cube();
	_Obj2_Door->_Material->SetupTexture("Door.png", 0, "main", SP_TEXTURE_CHANNEL_TYPE_RGBA);
	...
	_Obj2_Door->IsOpaque = false;

	_Obj2_Door->Init();

	Alpha.push_back(_Obj2_Door);
*/

#pragma endregion

class SP_CubeTex:public SP_Object
{
public:
	SP_CubeTex(MainShaderPractice *root);
	~SP_CubeTex();

	vector<SP_TextureInfo*> DirTextures;
	//0~5:PX NX PY NY PZ NZ
	SP_TextureInfo TextureCube;

	void DefaultInit();
	void Init();
	void LoadCubeTex();
	void BuildCubeByAllDirTex();
	

private:

};

class SP_SceneBase
{
public:
	SP_SceneBase(MainShaderPractice *root);
	~SP_SceneBase();
	MainShaderPractice *Root;
	GLuint ID;

	list<SP_Object*> Alpha;
	list<SP_Object*> Opaque;
	list<SP_LightBase*> Lights;
	list<SP_CubeTex*> SkyBoxs;

	virtual void Start() {};
	virtual void OnUpdate() {};
	virtual void End() {};

	virtual void DestroyAllObj();

private:

};

struct SP_AddVertex
{
	float Mass;
	float Velocity;
	SP_Vertex* _Vertex;
};

struct  SP_SpringAttribute
{
	float Length;
	float SpringK;
	glm::vec3 Pos1;
	glm::vec3 Pos2;
};

class SP_Cloth :SP_Object
{
public:
	SP_Cloth(MainShaderPractice* root);
	~SP_Cloth();

	bool IsPlay=false;


};

class SP_Spring :SP_Object
{
public:
	SP_Spring(MainShaderPractice* root);
	~SP_Spring();

	bool IsPlay = false;

	void InitSpringMoudle();


	vector<SP_AddVertex> addVert;

	int NodeNum;

	void SetUpAll();
	void InitAddVert();

	void ReSetVertices();
	void ReSetVAO();
	void ReSetNormal();
};



class SP_ScenePapar :public SP_SceneBase
{
public:
	SP_ScenePapar(MainShaderPractice* root);
	~SP_ScenePapar();

	SP_Cloth* _Cloth1;
	SP_Spring* _Spring;

	SP_LightBase* _Plight;

	void Start();
	void OnUpdate();
	void End();
};

class SP_Scene1 :public SP_SceneBase
{
public:
	SP_Scene1(MainShaderPractice *root);
	~SP_Scene1();

	SP_Object* _Obj0;
	SP_Object* _Obj1;
	SP_Object* _Obj1_c1;
	SP_Object* _testObj0;
	SP_Object* _testObj1;
	SP_Object* _testObj2;
	SP_Object* _testObj3;
	SP_Object* _testObj4;
	SP_Object* _testObj5;
	SP_Object* _Obj2_Door;
	SP_Object* _ObjCloth;

	SP_CubeTex* _SkyBox1;

	SP_LightBase *_Sun;
	SP_LightBase *_Plight;

	void Start() ;
	void OnUpdate();
	void End() ;
private:

};
class SP_Scene2 :public SP_SceneBase
{
public:
	SP_Scene2(MainShaderPractice *root);
	~SP_Scene2();

	SP_Object* _Obj0;
	SP_Object* _Obj1;
	SP_Object* _Obj2;
	SP_Object* _Obj3;
	SP_Object* _Obj4;
	SP_Object* _Obj5;

	void Start();
	void OnUpdate();
	void End();
private:

};

class SP_Thread_FixedLoop :public SP_Thread
{
public:
	SP_Thread_FixedLoop();
	~SP_Thread_FixedLoop();
	bool Lock = false;

	list<void(SP_Thread_FixedLoop::*)()> Funcs;
	SP_Thread_FixedLoop* _this;

	void InitAndRun();
	void FixedLoop();

	void test();

#pragma region func1
	void AlphaMapBuild();
	bool LockAlphaMapBuild = false;
	map<float, SP_Object*> *out_AlphaM_;
	vector<list<SP_Object*>*>  *AlphaVL_AMB_;
	SP_Camera *Cam_AMB_;
#pragma endregion

#pragma region func2
	void SetUpDepthMap();
	bool LockSetUpDepthMap = false;
	vector<list<SP_Object*>*>  *AlphaVL_SDM_;
	vector<list<SP_Object*>*>  *OpaqueVL_SDM_;
	vector<list<SP_LightBase*>*> *LightL_SDM_;
#pragma endregion




private:
};

enum SP_RendererMODE
{
	SP_RendererMODE_DEPTH,
	SP_RendererMODE_BLEND,
	SP_RendererMODE_DEPTH_BLEND,
	SP_RendererMODE_QUEUE_BLEND,
	SP_RendererMODE_OIT,
};
class SP_RendererFactory
{
public:
	SP_RendererFactory(MainShaderPractice *root);
	~SP_RendererFactory();
	MainShaderPractice *Root;
	GLuint ID;
	SP_Camera *_Cam;
	GLFWwindow *_Rwin;

	int R_Mode= SP_RendererMODE_DEPTH;
	bool IsCullBack=true;

	SP_Thread_FixedLoop *ThreadFixedLoop_;
	list<SP_SceneBase*> Scenes;
	vector<list<SP_Object*>*>  AlphaVL;
	vector<list<SP_Object*>*>  OpaqueVL;
	vector<list<SP_LightBase*>*> LightVL;
	map<float, SP_Object*> *_AlphaM;
	
	void LoadAllSceneObj();
	void LoadAllSceneObjToFLoop();


	void DrawAll();
	void DrawAllOpaque();
	void DrawAllAlpha();
	void DrawAllAlphaByOrder();

	void DrawToFrameBuffer();

	void Draw_DEPTH_BLEND();
	void Draw_DEPTH();
	void Draw_BLEND();
	void Draw_STENCIL();
private:

};

//DrawBox_1._Cam = &MainCamera;
//DrawBox_1._Rwin = window;
//DrawBox_1.R_Mode = SP_RendererMODE_DEPTH_BLEND;
//DrawBox_1.Scenes.push_back(&Scene1);
//DrawBox_1.Scenes.push_back(&Scene2);
//DrawBox_1.LoadAllSceneObj();
//DrawBox_1._ThreadFixedLoop = &FixedLoop;
//DrawBox_1.LoadAllSceneObjToFLoop();

class SP_UserInput
{
public:
	SP_UserInput();
	~SP_UserInput();
	GLFWwindow *_window;

	double LCPX = 800;//LastCursorPosX
	double LCPY = 450;//LastCursorPosY
	double CCPX = 800;//CurrentCursorPosX
	double CCPY = 450;//CurrentCursorPosY
	float ShaderParameter = 0;


	void shutDown();
	void cameraContoller(SP_Camera *_cami);
	void ShaderParameterContoller(SP_RendererFactory *_factory);

private:

};

class SP_MotionBlur
{
public:
	SP_MotionBlur(MainShaderPractice *root);
	~SP_MotionBlur();
	MainShaderPractice *Root;
	unsigned int ID;
	bool IsActive;

	int TexNum=5;

	SP_PBObj* _PBO;
	SP_Program* _Program;
	vector<SP_TextureInfo>  *_TexInfos;

	void Blur();
	void ReadScrPixle();
	void Init();

private:

};//building

enum SP_FrameBufferType
{
//GetPos_RP		Type	DataInfo
	GEOMETRY_PIXLE_BUFFER_ALL,
	GEOMETRY_FRAME_BUFFER_ALL,
	GEOMETRY_FRAME_BUFFER_DEPTH,
	GEOMETRY_FRAME_BUFFER_COLOR_RGBA,
	GEOMETRY_FRAME_BUFFER_COLOR_RGB,
	GEOMETRY_FRAME_BUFFER_COLOR_RGB32,
	GEOMETRY_FRAME_BUFFER_COLOR_RGBA32,
	GEOMETRY_FRAME_BUFFER_NORMAL,
};

class SP_ScrBufferPass
{
public:
	SP_ScrBufferPass(MainShaderPractice *root);
	~SP_ScrBufferPass();

	GLuint gl_BufferID;

	SP_TextureInfo* Texturei = new SP_TextureInfo();


	void Init();
	void GetScrBuffer(int GB_type);

	void PassBuffer();

	void GetDefaultBuffer_test();
	void GetDefaultBuffer();

	void BindCaptureTex();

	void ScrBufferExpection();
private:

};//building
