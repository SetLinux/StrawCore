#include "../Math/Math.hpp"

class Camera{
public:
  Camera();
  ~Camera();
  XVector position = XVector(0,0,0);
  static Camera main;
  glm::mat4 Projection = glm::ortho(0.5f,(1000 - 1) + -0.5f , -0.5f, ( 1000 - 1.0f) + 0.5f , -1000.f,1000.f);
  inline glm::mat4 getViewMatrix(){
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view,XVector::ToVec<glm::vec3>(position,true));
    return view;
  }

  
  inline XVector ScreenToWorld(const XVector& inpoint) {
	  float xpos = (inpoint.x / (1000.f )) * 2 - 1; // WIDTH 
	  float ypos = 1.0f - (inpoint.y / (1000.f )) * 2;
	  glm::mat4 PV = Projection * getViewMatrix();
	  glm::mat4 end = glm::inverse(PV);
	  glm::vec4 p4 = glm::vec4(xpos, ypos, 0.0f, 1.0f);
	  glm::vec4 res =  end * p4;
	  return XVector::fromVec<glm::vec4>(res);
  };
  
private:
  
  
};
