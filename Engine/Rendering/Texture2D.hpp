#include <GL/glew.h>
#include <string>
namespace Straw {
namespace Rendering {

class Texture2D {
public:
  Texture2D(const std::string &path);
  ~Texture2D();
  void Init();
  unsigned int id;
  void Reload();

private:
  std::string m_path;
};
} // namespace Rendering
} // namespace Straw
