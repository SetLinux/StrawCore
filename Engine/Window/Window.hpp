#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include <functional>
class Window{
public:
  Window(const std::string& name,float width,float height);
  ~Window();
  void Init();
  void Loop(std::function<void(float,float)> Update,std::function<void(float)> FixedUpdate);
  GLFWwindow* m_window;
  double mousexpos, mouseypos;
  static double S_WIDTH, S_HEIGHT;
  
private:
  std::string m_name;
  float m_width,m_height;
};
