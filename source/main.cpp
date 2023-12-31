#include <iostream>

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/Version.h>

#include <glbinding-aux/ContextInfo.h>
#include <glbinding-aux/types_to_string.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>

#include "Timer.h"
#include "Scene.h"
#include "Sphere.h"
#include "PhongMaterial.h"
#include "Earth.h"

static int  s_WindowWidth  = 800;
static int  s_WindowHeight = 600;
static bool s_bEnableVSync = true;
static bool s_bWireframe   = false;
static bool s_bFullScreen = false;

static auto TheScene = std::make_shared<Scene>();

using namespace gl;

void Draw()
{
    gl::glClear(gl::ClearBufferMask::GL_COLOR_BUFFER_BIT | gl::ClearBufferMask::GL_DEPTH_BUFFER_BIT);
	TheScene->Draw();
}

void Reshape(GLFWwindow* /*window*/, int width, int height)
{
	if (width > 0 && height > 0)
	{
		TheScene->camera()->SetViewportSize(width, height);
		gl::glViewport(0, 0, (GLint)width, (GLint)height);
	}    
}

static void CreateScene()
{
	gl::glClearColor(0.0, 0.0, 0.0, 1.0);

	auto sphereMesh = std::make_shared<SphereMesh>(200);
    
    glm::vec3 EarthPosition{ 0.0f };
    float EarthRadius = 6.371f;

    auto TheEarth = std::make_shared<Earth>(EarthPosition, EarthRadius, sphereMesh);

	// Order is important here. Earth must be the last
	TheScene->AddDrawable(TheEarth);
}

static void Init()
{
    // OpenGL Initialization
    using namespace std;
	using namespace glbinding;

    cout << "OpenGL Version: " << endl;
    cout << "\tGL_RENDERER: " << glbinding::aux::ContextInfo::renderer() << endl;
    cout << "\tGL_VERSION : " << glbinding::aux::ContextInfo::version().toString() << endl;
    cout << "\tGL_VENDOR  : " << glbinding::aux::ContextInfo::vendor() << endl;	

	Binding::setCallbackMaskExcept(CallbackMask::After, { "glGetError" });		
	Binding::setAfterCallback([](const FunctionCall &)
	{
		const gl::GLenum error = gl::glGetError();
		if (error != GL_NO_ERROR)
		{ 
			cout << "Error: " << error << endl;
		}			
	});
    
    gl::glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);   
	
	CreateScene();
}

int main()
{
    GLFWwindow* Window = nullptr;
    
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
		return EXIT_FAILURE;
    }
    
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_SAMPLES, 16);

    std::string WindowTitleBase = "Physically Based Rendering with OpenGL ";	
    
	if (s_bFullScreen)
	{
		int monitorsCount;
		GLFWmonitor** Monitors = glfwGetMonitors(&monitorsCount);
		const int ActiveMonitorIdx = 1;

		const GLFWvidmode* Mode = glfwGetVideoMode(Monitors[ActiveMonitorIdx]);
		glfwWindowHint(GLFW_RED_BITS, Mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, Mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, Mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, Mode->refreshRate);

		Window = glfwCreateWindow(Mode->width, Mode->height, WindowTitleBase.data(), Monitors[ActiveMonitorIdx], nullptr);
	}
	else
	{
		Window = glfwCreateWindow(s_WindowWidth, s_WindowHeight, WindowTitleBase.data(), nullptr, nullptr);
	}
    
    if (!Window)
    {
        std::cerr << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    // Set callback functions
    glfwSetFramebufferSizeCallback(Window, Reshape);
    glfwSetKeyCallback(Window, 
		[](GLFWwindow* /*window*/, int Key, int /*s*/, int Action, int /*mods*/)
		{
			TheScene->camera()->KeyEvent(Key, Action);

			if (Action == GLFW_RELEASE)
			{
				switch (Key)
				{
				case GLFW_KEY_L:
					//scene->toggleLightAnimation();
					break;
				case GLFW_KEY_K:
					//scene->toggleLightDirection();
					break;
				case GLFW_KEY_O:
					s_bWireframe = !s_bWireframe;
					gl::glPolygonMode(GL_FRONT_AND_BACK, s_bWireframe ? GL_LINE : GL_FILL);
					std::cout << "Wireframe: " << s_bWireframe << std::endl;
					break;
				default:
					break;
				}
			}
		});
	glfwSetMouseButtonCallback(Window, 
        [](GLFWwindow* window, int button, int action, int /*mods*/)
        {
            if (button != GLFW_MOUSE_BUTTON_LEFT)
            {
                return;
            }

            if (action == GLFW_PRESS)
            {
                double x, y;

                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwGetCursorPos(window, &x, &y);

                TheScene->camera()->MouseButtonEvent(static_cast<float>(x), static_cast<float>(y));
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        });
	glfwSetCursorPosCallback(Window, 
        [](GLFWwindow* window, double x, double y)
        {
            if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            {
                TheScene->camera()->MouseMotionEvent(static_cast<float>(x), static_cast<float>(y));
            }
        });
	glfwSetScrollCallback(Window, 
        [](GLFWwindow* /*window*/, double x, double y)
        {
            TheScene->camera()->MouseScrollEvent(static_cast<float>(x), static_cast<float>(y));
        });
    
    glfwMakeContextCurrent(Window);
    glfwSwapInterval(s_bEnableVSync);

	glbinding::Binding::initialize(glfwGetProcAddress);
    
    glfwGetFramebufferSize(Window, &s_WindowWidth, &s_WindowHeight);
    Reshape(Window, s_WindowWidth, s_WindowHeight);

    Init();
    
    const glbinding::Version OpenGLVersion = glbinding::aux::ContextInfo::version();
    WindowTitleBase += OpenGLVersion.toString();
    
    FPSTimer fpsTimer;
    Timer FrameTimer;
    
    // Main loop
    while( !glfwWindowShouldClose(Window) )
    {        
        Draw();
        
        // Update animation
        TheScene->Update(FrameTimer.ElapsedSeconds());
        
        FrameTimer.Start();
        
        // Swap buffers
        glfwSwapBuffers(Window);
        glfwPollEvents();
        
        if(fpsTimer.Update())
        {
            const std::string windowTitle = WindowTitleBase + " - FPS: " + std::to_string(fpsTimer.GetFPS());
            glfwSetWindowTitle(Window, windowTitle.data());
        }
    }
    
    // Terminate GLFW
    glfwTerminate();
    
    // Exit program
    return EXIT_SUCCESS;
}
