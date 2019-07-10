#include "../../entt/entt.hpp"
#include "../Components/Components.hpp"
#include "../Rendering/Camera.hpp"
#include "../Rendering/Renderer.hpp"
#include <mutex>
#include <thread>
#include "../Utils/Threading.hpp"
namespace Straw {
struct ShaderBatch{
	unsigned int shdrID;
	std::vector<Straw::Components::Sprite*> sprites;
};
class RenderingSystem {
public:
  static Vertex *vertices;
  
  static void RenderSystem(entt::registry &registery);
  static void InitSystem(entt::registry& registery) {
    reg = &registery;
    renderer.Init();
    vertices = new Vertex[4];
    vertices[0].position = XVector(-0.5f, 0.5f);
    vertices[0].TexCoord = XVector(0.0f, 1.0f);
    vertices[1].position = XVector(0.5f, 0.5f);
    vertices[1].TexCoord = XVector(1.0f, 1.0f);
    vertices[2].position = XVector(0.5f, -0.5f);
    vertices[2].TexCoord = XVector(1.0f, 0.0f);
    vertices[3].position = XVector(-0.5f, -0.5f);
    vertices[3].TexCoord = XVector(0.0f, 0.0f);
  }
  //Used to get part of a texture (SpriteSheet) :)
  static void ClipSprite(XVector pos, XVector coords, XVector sheet, Straw::Components::Sprite& sprite);
  //the assigned function will execute when a shader is bind (should be used to assign uniforms)
  static void AddUniformBinding(unsigned int shaderID, std::function<void()> function) {
	  ShaderUniforms[shaderID] = function;
  }
private:
  static Rendering::Renderer renderer;
  static std::vector<ShaderBatch*>  ShaderBatches;
  static entt::registry* reg;
  static std::map<unsigned int, std::function<void()>> ShaderUniforms;
};
} // namespace Straw
