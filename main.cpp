#include <iostream>
#include "entt/entt.hpp"
#include "Engine/Systems/RenderingSystem.hpp"
#include "Engine/Systems/PhysicsSystem.hpp"
#include "Engine/Systems/ScriptingSystem.hpp"
#include "Engine/Rendering/Texture2D.hpp"
#include "Engine/Rendering/Shader.hpp"
int main(){
Straw::Window win("Fast Test",1000,1000);
win.Init();
 auto reg = entt::registry{};
Straw::RenderingSystem::InitSystem(reg);
Straw::PhysicsSystem::Init();
Straw::Rendering::Shader shdr("Assets/base.vert","Assets/base.frag");
shdr.Init();
shdr.Use();
Straw::Rendering::Texture2D tex("Assets/sheet.png");
tex.Init();
Camera::main.position.x += 90;
ScriptingSystem::Init(reg,win);
std::cout << "TEXTURE: " << tex.id << std::endl;
ScriptingSystem::ExecuteScript("Assets/KeyBindings.lua");
auto ScriptGod = reg.create();
reg.assign<Straw::Components::Script> (ScriptGod,ScriptingSystem::luastate).LoadScript("Assets/God.lua");
auto ent = reg.create();
std::cout  << ent << std::endl;
reg.assign<Straw::Components::Transform>(ent,XVector(500,500),XVector(100,100),0);
reg.assign<Straw::Components::Sprite>(ent,tex.id,0);
reg.assign<Straw::Components::Physics>(ent,Straw::PhysicsSystem::CreateBody(reg,ent)).body->SetLinearDamping(0);
reg.get<Straw::Components::Physics>(ent).body->SetGravityScale(0);
reg.get<Straw::Components::Physics>(ent).body->SetFixedRotation(true);
auto floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(500,000),XVector(800,100),Radians(0));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).body->SetType(b2BodyType::b2_staticBody);

std::cout << floor << std::endl;
 floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(900,000),XVector(800,100),Radians(45));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).body->SetType(b2BodyType::b2_staticBody);
reg.get<Straw::Components::Physics>(floor).Slope = true;
floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(100,000),XVector(800,100),Radians(-45));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).body->SetType(b2BodyType::b2_staticBody);
reg.get<Straw::Components::Physics>(floor).Slope = true;
floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(756,446.9f),XVector(300,60),Radians(45));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).body->SetType(b2BodyType::b2_staticBody);
reg.get<Straw::Components::Physics>(floor).Slope = true;

floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(500,300),XVector(400,100),Radians(45));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).body->SetType(b2BodyType::b2_staticBody);
reg.get<Straw::Components::Physics>(floor).Slope = true;

win.Loop([&](float alpha,float dt){
    ScriptingSystem::ScriptingSystemUpdate(reg,dt);
    Straw::PhysicsSystem::PhysicsSystemUpdate(reg,alpha);
    Straw::RenderingSystem::RenderSystem(reg);
},[&](float dt){
    ScriptingSystem::ScriptingSystemFixedUpdate(reg,dt);
    Straw::PhysicsSystem::PhysicsSystemFixedUpdate(reg);
});
}
