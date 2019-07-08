#include "Shader.hpp"
namespace Straw {
namespace Rendering {
Shader::Shader(const std::string &vertPath, const std::string &fragPath)
  : _fragpath(fragPath) , _vertpath(vertPath),vertshader(0),fragshader(0) {
}
Shader::~Shader() {
  //TODO ::  ADD A FUCKING DESTRUCTOR
}
void Shader::Init(){
  
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile; 
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open files
    vShaderFile.open(_vertpath);
    fShaderFile.open(_fragpath);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "Couldn't Read Shader File" << std::endl;
  }
  _vertsource = (char *)vertexCode.c_str();
  _fragsource = (char *)fragmentCode.c_str();
  CompileShader(&vertshader, _vertsource, GL_VERTEX_SHADER);
  CompileShader(&fragshader, _fragsource, GL_FRAGMENT_SHADER);
  id = glCreateProgram();
  glAttachShader(id, vertshader);
  glAttachShader(id, fragshader);
  glLinkProgram(id);
}
void Shader::Use(){
  glUseProgram(id);
}
void Shader::CompileShader(unsigned int *shdrID, const char *Source,int SXType) {
  (*shdrID) = glCreateShader(SXType);
  glShaderSource((*shdrID), 1, &Source, NULL);
  glCompileShader((*shdrID));
  int success;
  char infoLog[512];
  glGetShaderiv((*shdrID), GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog((*shdrID), 512, NULL, infoLog);
    std::cout << " FAILED SHADER : " << infoLog << std::endl;
  }
}
unsigned int Shader::getUniformLocation(std::string name) {
  return glGetUniformLocation(id,name.c_str());
}
  
}
}// namespace Rendering
