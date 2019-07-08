#include "RenderingSystem.hpp"

namespace Straw{
#pragma region Static Declarations
	std::vector <ShaderBatch*> RenderingSystem::ShaderBatches;
	Vertex* RenderingSystem::vertices;
	Rendering::Renderer RenderingSystem::renderer;
	std::map<unsigned int, std::function<void()>> RenderingSystem::ShaderUniforms;
#pragma endregion

void RenderingSystem::RenderSystem(entt::registry &registery) {
  auto view = registery.view<Components::Transform, Components::Sprite>();
  glm::mat4 MVP = Camera::main.Projection * Camera::main.getViewMatrix();
  renderer.Begin();
  Straw::Threading::MakeViewParrarel<decltype (view),unsigned int>(view,[&](unsigned int ts){

       Components::Transform& transform = view.get<Components::Transform>(ts);
       Components::Sprite& sprite = view.get<Components::Sprite>(ts);

       glm::mat4 model = glm::mat4(1.0f);
       model = glm::translate(model, XVector::ToVec<glm::vec3>(transform.position, true));
       model = glm::rotate(model, transform.rotation, glm::vec3(0.0, 0.0, 1.0f));
       model = glm::scale(model, XVector::ToVec<glm::vec3>(transform.scale, true));

       glm::mat4 FMVP = MVP * model;
       // making the model matrix
       for (int i = 0; i < 4; i++) {
           glm::vec4 intern = glm::vec4(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z, 1.0f);
           glm::vec4 res = FMVP * intern;
           sprite.vertices[i].position = XVector::fromVec<glm::vec4>(res, true);
           if (sprite.vertices[i].TexCoord == XVector(0, 0))
               sprite.vertices[i].TexCoord = vertices[i].TexCoord;
       }
  },3);
  for (auto& ts : view) {

      Components::Sprite& sprite = view.get<Components::Sprite>(ts);
//		  model = glm::translate(model, XVector::ToVec<glm::vec3>(transform.position, true));
//		  model = glm::scale(model, XVector::ToVec<glm::vec3>(transform.scale, true));
//
//		  glm::mat4 FMVP = MVP * model;
//		  // making the model matrix
//		  for (int i = 0; i < 4; i++) {
//			  intern = glm::vec4(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z, 1.0f);
//			  res = FMVP * intern;
//			  sprite.vertices[i].position = XVector::fromVec<glm::vec4>(res, true);
//			  if (sprite.vertices[i].TexCoord == XVector(0, 0))
//				  sprite.vertices[i].TexCoord = vertices[i].TexCoord;
//		  }
//	      model = glm::mat4(1.0f);
//		  model = glm::rotate(model, transform.rotation, glm::vec3(0.0, 0.0, 1.0f));
	  if (sprite.ShdrID != 0) {
		  bool found = false;
		  for (auto& it : ShaderBatches) {
			  if (it->shdrID == sprite.ShdrID) {
				  it->sprites.push_back(&sprite);
				  found = true;
			  }
		  }
		  if (!found) {
			  ShaderBatch* local = new ShaderBatch();
			  local->shdrID = sprite.ShdrID;
			  local->sprites.push_back(&sprite);
			  ShaderBatches.push_back(local);
			  
			}
	  }
	  else {
		  renderer.Submit(sprite.vertices, sprite.texID);
	  }
  }

  renderer.End();
  for (auto& it : ShaderBatches) {
	  glUseProgram(it->shdrID);
	  if (ShaderUniforms[it->shdrID]) ShaderUniforms[it->shdrID]();
	  renderer.Begin();
	  for (auto& spr : it->sprites) {
			renderer.Submit(spr->vertices, spr->texID);
	  }
	  renderer.End();
	  it->sprites.clear();
  }
}
void RenderingSystem::ClipSprite(XVector pos, XVector coords, XVector sheet,Straw::Components::Sprite& sprite) {
	float xTex = pos.x / sheet.x;
	float yTex = 1 - (pos.y / sheet.y);
	float wTex = coords.x / sheet.x;
	float hTex = coords.y / sheet.y;

	sprite.vertices[0].TexCoord = XVector(xTex + wTex,yTex);
	sprite.vertices[1].TexCoord = XVector(xTex + wTex , yTex - hTex);
	sprite.vertices[2].TexCoord = XVector(xTex, yTex - hTex);
	sprite.vertices[3].TexCoord = XVector(xTex, yTex);
}

} // namespace Straw
