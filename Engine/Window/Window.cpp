#include "Window.hpp"
namespace Straw {

double Window::S_HEIGHT, Window::S_WIDTH;
Window::Window(const std::string &name, float width, float height)
    : m_name(name), m_width(width), m_height(height), m_window(nullptr){
	S_HEIGHT = height;  
	S_WIDTH =	width;
}
void Window::Init() {
  glfwInit();
  // Giving the necessary Hints to create a porper modern opengl context
  glfwWindowHint(GLFW_SAMPLES, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Create a new window
  m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
  glfwMakeContextCurrent(m_window);
  GLenum err = glewInit();
  if (err != GLEW_OK) {
  }
  // Print the vendor
  const GLubyte *vendor = glGetString(GL_VENDOR); // Returns the vendor
  std::cout << "GL Vendor : " << vendor << std::endl;
  // Enable OpenGL State preperation
  glEnable(GL_DEPTH_TEST);

  glViewport(0, 0, m_width, m_height);
  // Setup a function for blending

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
  glfwSwapInterval(1);
}
Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}
void Window::Loop(std::function<void(float,float)> Update,
                  std::function<void(float)> FixedUpdate) {
  double t = 0.0;
  const double  dt = .0166666666667;

  double currentTime = glfwGetTime();
  double accumulator = 0;
  while (!glfwWindowShouldClose(m_window)) {
    bool RANFIXED = false;
     glfwPollEvents();
    double newTime = glfwGetTime();
    double frameTime = newTime - currentTime;
    currentTime = newTime;
	glfwGetCursorPos(m_window, &mousexpos, &mouseypos);
    accumulator += frameTime;
    while (accumulator >= dt) {
		FixedUpdate(dt);
        std::cout << "FIXED" << std::endl;
		accumulator -= dt;
		t += dt;
        RANFIXED = true;

	}
    if(!RANFIXED) {std::cout << " JITTER " << std::endl;}
	const double alpha = accumulator / dt;
    std::cout << " NEEDS A CARPENTER" << std::endl;
    glClearColor(1, 1, 0, 1);
	glEnable(GL_STENCIL_TEST | GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    Update(alpha,dt);
    glfwSwapBuffers(m_window);
  }
 
}
} // namespace Straw
