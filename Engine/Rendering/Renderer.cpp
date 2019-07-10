#include "Renderer.hpp"
namespace Straw {
namespace Rendering {
Renderer::Renderer() : m_VBO(0), m_VAO(0) {}
Renderer::~Renderer() {}
void Renderer::Init() {
  unsigned int indices[INDICESBUFFERSIZE];
  int m_offset = 0;
  for (int i = 0; i < INDICESBUFFERSIZE; i += 6) {
    indices[i + 0] = (m_offset + 0);
    indices[i + 1] = (m_offset + 1);
    indices[i + 2] = (m_offset + 3);
    indices[i + 3] = (m_offset + 1);
    indices[i + 4] = (m_offset + 2);
    indices[i + 5] = (m_offset + 3);
    m_offset += 4;
  }
  glGenBuffers(1, &m_IBO);
  glGenBuffers(1, &m_VBO);
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, BUFFERSIZE, NULL, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               INDICESBUFFERSIZE * sizeof(unsigned int), indices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)sizeof(XVector));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
void Renderer::Begin() {
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
 // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
  if(Binded){std::cout << "You already begain without ending "; }
  Binded = true;
  
  //  m_buffer = (Vertex
  //  *)glMapBufferRange(GL_ARRAY_BUFFER,0,BUFFERSIZE,GL_MAP_WRITE_BIT|
  //  GL_MAP_UNSYNCHRONIZED_BIT|GL_MAP_INVALIDATE_RANGE_BIT);
}
void Renderer::Submit(Vertex vertices[4], unsigned int texid, int verticesnum) {
  for (int i = 0; i < verticesnum; i++) {
	    m_queuedvertices[texid].emplace_back(vertices[i]);
  }
  if (m_queuedvertices[texid].capacity() <= 4) {
    m_queuedvertices[texid].reserve(6 * 110000);
  }
}
void Renderer::End() {
	if (!Binded) {
        std::cout << "[*] Renderer : You don't bind before calling End " << std::endl;
	}
  int offset = 0;
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

  for (auto &rq : m_queuedvertices) {
    glBindTexture(GL_TEXTURE_2D, rq.first);
	glBufferSubData(GL_ARRAY_BUFFER, offset, rq.second.size() * sizeof(Vertex),rq.second.data());
    glDrawElements(GL_TRIANGLES, rq.second.size() / 4 * 6, GL_UNSIGNED_INT,(void *)offset);
	
    rq.second.clear();
    offset += rq.second.size() * sizeof(Vertex);
  }
  Binded = false;
  glBindVertexArray(0);
  
}
} // namespace Rendering
} // namespace Straw
