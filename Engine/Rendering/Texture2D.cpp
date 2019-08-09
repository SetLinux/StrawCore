#include "Texture2D.hpp"
#include <SOIL2.h>
namespace Straw {
namespace Rendering {

Texture2D::Texture2D(const std::string& path) : m_path(path) , id(0){

}
Texture2D::~Texture2D() {
    glDeleteTextures(1, &id);

}
void Texture2D::Init() {
	id = SOIL_load_OGL_texture
	(
		m_path.c_str(),
		SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
     SOIL_FLAG_NEAREST | SOIL_FLAG_INVERT_Y );
	if (id == 0) {
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
};
//If Image Changed and you want to reload at Run-Time
//TODO : Test this (not tested yet.)
void Texture2D::Reload(){
  glDeleteTextures(1, &id);  
  id  = SOIL_load_OGL_texture
    (
     m_path.c_str(),
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NEAREST|
      SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
     );
  if (id == 0) {
    printf("SOIL loading error: '%s'\n", SOIL_last_result());
  }
};
}
}// namespace Rendering
