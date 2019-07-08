#pragma once
#include "../Math/Math.hpp"
#include <GL/glew.h>
#include <map>
#include <vector>

#define SPRITESIZE sizeof(Vertex) * 4
#define MAXSPRITES 90000
#define BUFFERSIZE SPRITESIZE *MAXSPRITES
#define INDICESBUFFERSIZE MAXSPRITES * 6
namespace Straw {
namespace Rendering {
class Renderer {
public:
  Renderer();
  ~Renderer();
  void Begin();
  void Submit(Vertex *vertices, unsigned int TexID, int verticesnum = 4);
  void End();
  void Init();

private:
bool Binded = false;
  std::map<unsigned int, std::vector<Vertex>> m_queuedvertices;
  unsigned int m_VAO, m_VBO, m_IBO;
  Vertex *m_buffer;
};
} // namespace Rendering
} // namespace Straw
