
//Every thing here just begin with a simple triangle
#include "windows.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "iostream"
#include "InputStream.h"
#include "MainShaderPractice.h"

 
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
int main()
{
#pragma region StreamInit
	ConfigStream config = ConfigStream("config.txt");
	config.GetObjAttribute();
#pragma endregion

#pragma region glfwInit
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(config.SCR_WIDTH, config.SCR_HEIGHT, "testProgram", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//is it any use??????
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
#pragma endregion

#pragma region oglConfig
	//glEnable(GL_FRAMEBUFFER_SRGB);
#pragma endregion

#pragma region ObjInit
	MainShaderPractice ProjectRoot = MainShaderPractice();
	SP_RendererFactory DrawBox_1 = SP_RendererFactory(&ProjectRoot);

	SP_Scene1 Scene1 = SP_Scene1(&ProjectRoot);
	SP_Scene2 Scene2 = SP_Scene2(&ProjectRoot);

	SP_UserInput userInput = SP_UserInput();

	SP_Camera MainCamera = SP_Camera(&ProjectRoot, &config);
	MainCamera.DefaultInit();

	SP_ScreenMasker ScrFrameDebuger = SP_ScreenMasker();

	SP_Thread_FixedLoop FixedLoop = SP_Thread_FixedLoop();

	//SP_Program  SP_Program = new SP_Program(&ProjectRoot)

	SP_MotionBlur MotionBlur = SP_MotionBlur(&ProjectRoot);
	MotionBlur.Init();
#pragma endregion

#pragma region ParameterInit
	userInput._window = window;

	float Frame;
	float LastTime = 0;

	DrawBox_1._Cam = &MainCamera;
	DrawBox_1._Rwin = window;
	DrawBox_1.R_Mode = SP_RendererMODE_DEPTH_BLEND;
	DrawBox_1.Scenes.push_back(&Scene1);
	DrawBox_1.Scenes.push_back(&Scene2);
	DrawBox_1.LoadAllSceneObj();
	DrawBox_1.ThreadFixedLoop_ = &FixedLoop;
	DrawBox_1.LoadAllSceneObjToFLoop();

	MainCamera.TranslateCam(0, 0, 2);
	MainCamera.viewAngle = 60.0f;
	//MainCamera.IsOrthogonalViewMat = true;
	MainCamera.SetUpProjectionMat();
	MainCamera.LightSourse.push_back(Scene1._Sun);
	MainCamera.LightSourse.push_back(Scene1._Plight);

	Scene1.Start();
	Scene2.Start();

	//Sun._Program = ;

	FixedLoop.Cam_AMB_ = &MainCamera;
	FixedLoop.InitAndRun();
#pragma endregion	

#pragma region RenderLoop
	while (!glfwWindowShouldClose(window))//rendering loop
	{
#pragma region UserInput
		userInput.shutDown();
		userInput.cameraContoller(&MainCamera);
		userInput.ShaderParameterContoller(&DrawBox_1);
		glfwPollEvents();
#pragma endregion

#pragma region ScreenPre
		glEnable(GL_STENCIL_TEST);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//glStencilMask(0xff);
		//glClear(GL_STENCIL_BUFFER_BIT);		
		//Cleaner.CleanStencil();
		//Cleaner.DrawIndicateTex(Scene1._Sun->_ShadowDepthMap->ID);

		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);
#pragma endregion

#pragma region runtime
		Scene1.OnUpdate();
		Scene2.OnUpdate();
		//Sun._Transform->EulerRotate(1,0,0);
#pragma endregion

#pragma region draw
		//FixedLoop.SetUpDepthMap();
		glViewport(0, 0, config.SCR_WIDTH, config.SCR_HEIGHT);
		//testObj1.Draw( &MainCamera);
		//testObj0.Draw( &MainCamera);
		//testObj2.Draw( &MainCamera);
		//testObj3.Draw( &MainCamera);
		//Scene1._testObj0->Draw( &MainCamera);
		//testObj5.Draw( &MainCamera);

		DrawBox_1.DrawAll();

		//ScrFrameDebuger.DrawIndicateTex(Scene1._testObj1->_Material->Textures[0].gl_ID);
		//ScrFrameDebuger.DrawIndicateTex(Scene1._Sun->_ShadowDepthMap->gl_ID);
		//ScrFrameDebuger.CleanStencil();
#pragma endregion

#pragma region ImageReandering

#pragma endregion

#pragma region debuge
		Frame = 1 / (glfwGetTime() - LastTime);
		LastTime = glfwGetTime();
		cout << Frame << endl;

#pragma endregion

#pragma region ScreenFree

		glfwSwapBuffers(window);

#pragma endregion
	}
#pragma endregion

#pragma region Terminate
	glfwTerminate();
	cin >> *(new int());
	return 0;
#pragma endregion
}


