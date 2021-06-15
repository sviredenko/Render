#include "common.h"
#include "Image.h"
#include "Canvas.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>



struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}


void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{http://ab.lordfilms-s.me/52836-serial-oni-2021.html
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
    
    int WINDOW_WIDTH = 512, WINDOW_HEIGHT = 512;
    if(argc>1){
        WINDOW_WIDTH = 1024;
        WINDOW_HEIGHT = 1024;
    }
        
	if(!glfwInit())
    return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rendering task2", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
  glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	Point starting_pos{.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};


	Image img("./resources/load.png");
	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;
  /*built scene*/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, img.Data()); GL_CHECK_ERRORS;
        glfwSwapBuffers(window);
        
        
        
    //constexpr GLsizei 
    
    Vect3f light1={13,-10,-13};
    Vect3f light2={-22,-10,-5};
    
    Vect3f point_wall{0,2,20};
    Vect3f normal_wall{0,0,-1};
    Pixel color_wall{245,245,220,0};
    float reflection_wall=0;
    float refraction_wall=0;
    Floor wall(point_wall,normal_wall,color_wall,reflection_wall,refraction_wall);
    
    
    
    Vect3f point_floor{4,3,6.5};
    Vect3f normal_floor{0,-1,0};
    Pixel color_floor{140,80,28,0};
    float reflection_floor=0;//0.5
    float refraction_floor=0;
    Floor floor(point_floor,normal_floor,color_floor,reflection_floor,refraction_floor);
  
    Pixel color_sphere{0,0,0,0};
    Vect3f centr_sphere{0,-0.1,3.5};
    float reflection_sphere=1;//1.33;
    float refraction_sphere=1.33;
    Sphere sphere(2,centr_sphere,color_sphere,reflection_sphere,refraction_sphere);
    
    Vect3f boxmin{-4,3,2.5};
    Vect3f boxmax{4,2,4.5};
    Pixel color_box{166,198,206,0};
    float reflection_box=0;
    float refraction_box=0;
    Box box(boxmax,boxmin,color_box,reflection_box,refraction_box);
    
    
    Vect3f n_eye{0,-1,-3};
    Vect3f n_canvas_locate{0,0,1};
    Vect3f n_canvas_normal{0,0,0};
        
    Canvas canvas(n_eye,n_canvas_locate,n_canvas_normal,4,4,&sphere,&box,&floor,&wall,light1,light2);
    canvas.Render(screenBuffer,WINDOW_WIDTH,WINDOW_HEIGHT);
    
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        glfwPollEvents();


 

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        glfwSwapBuffers(window);
	}
    screenBuffer.reverse();
    screenBuffer.Save("./resources/315_Sviredenko_v0v2.png");
    
	glfwTerminate();
	return 0;
}
