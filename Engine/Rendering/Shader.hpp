#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "../Math/Math.hpp"
namespace Straw {
namespace Rendering {

class Shader {
public:
  Shader(const std::string &vShaderFile, const std::string &fShaderFile);
  ~Shader();
  void Init();
  void Use();
  unsigned int getUniformLocation(std::string name);
  void setFloatUniform(unsigned int location, float value) {
      glUniform1f(location,value);
  }
  
  unsigned int getID() { return id; }
private:
  unsigned int id;
  std::string _vertpath, _fragpath;
  char *_fragsource, *_vertsource;
  unsigned int vertshader, fragshader;
  void CompileShader(unsigned int *shdrID, const char *Source, int SXType);

};
} // namespace Rendering
} // namespace Straw
