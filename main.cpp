#include <iostream>
#include <tuple>
#include "entt/entt.hpp"
#include "Engine/Systems/RenderingSystem.hpp"
#include "Engine/Systems/PhysicsSystem.hpp"
#include "Engine/Systems/ScriptingSystem.hpp"
#include "Engine/Rendering/Texture2D.hpp"
#include "Engine/Rendering/Shader.hpp"
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
static int bounceLimit = 2;
static int currentBounces= 0;
struct  UserCallback : physx::PxSweepCallback{


};
enum ActiveGroup
{
    GROUP1    = (1<<0),
    GROUP2    = (1<<1),
    GROUP3    = (1<<2),
    GROUP4    = (1<<3),
};
physx::PxTransform MovePlayer(entt::registry& reg,physx::PxVec3 pos,physx::PxVec3  vel,const float xOffset,const float yOffset) {
    using namespace physx;
    if(currentBounces <= bounceLimit){
        currentBounces++;
    physx::PxTransform expectedtransform;
    expectedtransform.p = physx::PxVec3(pos.x + vel.x ,pos.y + vel.y,0);
    expectedtransform.q = physx::PxQuat(1.0f);
    physx::PxSweepHit hitInfo;
    physx::PxHitFlags hitFlags = physx::PxHitFlag::ePOSITION|physx::PxHitFlag::eNORMAL | physx::PxHitFlag::eMTD;
    XVector scale;
    scale = reg.get<Straw::Components::Transform>(1).scale;
    scale.z=2;
    PxBoxGeometry cubeGeomtry(XVector::ToVec<PxVec3>(XVector(50,50,5,2)/2.0f,true));
    PxTransform playertransform;
    playertransform.p = PxVec3(pos.x ,pos.y ,0);
    playertransform.q = PxQuat(1.0);
    PxSweepBuffer hit;

    PxQueryFilterData filterData = PxQueryFilterData();
    filterData.data.word0 =  GROUP1;
    bool hitCount =  Straw::PhysicsSystem::mScene->sweep(cubeGeomtry,playertransform,vel.getNormalized(),vel.magnitude(),hit,hitFlags,filterData);

    if(hit.hasAnyHits() ){

     expectedtransform.p = pos + vel.getNormalized() * (hit.block.distance - .4f);

     if((expectedtransform.p - pos) != PxVec3(0,0,0)){
     PxTransform Step1 = MovePlayer(reg,pos + vel.getNormalized() * 0.01f,(expectedtransform.p - pos) - vel.getNormalized() * 0.01f,0,0);
     return Step1;

     }
     return expectedtransform;
    }else{
        PxTransform tran;
        tran.p = expectedtransform.p;
        tran.q = PxQuat(1.0f);
        return tran;
    }
    }else{

        PxTransform tran;
        tran.p = pos+ vel;
        tran.q = PxQuat(1.0f);
        std::cout << " HAD TO GIVE UP" << std::endl;
        return tran;
    }

}

int main(){
Straw::Window win("Fast Test",1000,1000);
win.Init();
std::string TESTQTDEBUG ="OAH NO";
 auto reg = entt::registry{};
Straw::RenderingSystem::InitSystem(reg);
Straw::PhysicsSystem::Init(reg);
Straw::Rendering::Shader shdr("Assets/base.vert","Assets/base.frag");
shdr.Init();
shdr.Use();
Straw::Rendering::Texture2D tex("Assets/sheet.png");
tex.Init();
ScriptingSystem::Init(reg,win);
ScriptingSystem::ExecuteScript("Assets/KeyBindings.lua");
auto ScriptGod = reg.create();
ScriptingSystem::ExecuteScript("Assets/easing.lua");
reg.assign<Straw::Components::Script> (ScriptGod,ScriptingSystem::luastate).LoadScript("Assets/God.lua");
std::string thisisateststring = "FUCK QT CREATOR";
auto ent = reg.create();
std::cout  << ent << std::endl;
reg.assign<Straw::Components::Transform>(ent,XVector(500,900,-10),XVector(50,50),0);
reg.assign<Straw::Components::Sprite>(ent,tex.id,0);
reg.assign<Straw::Components::Physics>(ent,Straw::PhysicsSystem::CreateBody(reg,ent,true)).bodyjoint.body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,true);

auto floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(500,000),XVector(1024,128),Radians(0));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).bodyjoint.body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,true);
floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(900,000),XVector(800,100),Radians(45));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).bodyjoint.body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,true);
reg.get<Straw::Components::Physics>(floor).Slope = true;
  floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(100,000),XVector(800,100),Radians(-45));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).bodyjoint.body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,true);
reg.get<Straw::Components::Physics>(floor).Slope = true;
floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(756,446.9f),XVector(300,60),Radians(0));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).bodyjoint.body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,true);
reg.get<Straw::Components::Physics>(floor).Slope = false;
reg.get<Straw::Components::Physics>(floor).oneWay = true;

floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(856,346.9f),XVector(150,100),Radians(0));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).bodyjoint.body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,true);
reg.get<Straw::Components::Physics>(floor).Slope = false;
reg.get<Straw::Components::Physics>(floor).oneWay = true;

floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(990,146.9f),XVector(150,100),Radians(0));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).bodyjoint.body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,true);
reg.get<Straw::Components::Physics>(floor).Slope = false;
reg.get<Straw::Components::Physics>(floor).onSlope = true;

floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(-50,196.9f),XVector(128,128),Radians(0));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).bodyjoint.body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,true);
reg.get<Straw::Components::Physics>(floor).Slope = false;
reg.get<Straw::Components::Physics>(floor).onSlope = true;

floor = reg.create();
reg.assign<Straw::Components::Transform>(floor,XVector(500,300),XVector(400,150),Radians(0));
reg.assign<Straw::Components::Sprite>(floor,tex.id,0);
reg.assign<Straw::Components::Physics>(floor,Straw::PhysicsSystem::CreateBody(reg,floor)).bodyjoint.body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,true);
reg.get<Straw::Components::Physics>(floor).Slope = false;
reg.get<Straw::Components::Physics>(floor).oneWay = true;
win.Loop([&](float alpha,float dt){
    ScriptingSystem::ScriptingSystemUpdate(reg,dt);
    Straw::PhysicsSystem::PhysicsSystemUpdate(reg,1);

    //Camera::main.position = XVector(std::round(Camera::main.position.x),std::round(Camera::main.position.y));
    ScriptingSystem::ScriptingSystemLateFixedUpdate(reg,dt);
    Straw::RenderingSystem::RenderSystem(reg);
},[&](float dt){
    ScriptingSystem::ScriptingSystemFixedUpdate(reg,dt);
    Straw::PhysicsSystem::PhysicsSystemFixedUpdate(reg);
   //Camera::main.position = XVector::Interpolate(Camera::main.position,XVector(reg.get<Straw::Components::Physics>(ent).bodyjoint.body->getGlobalPose().p.x * -1 + 500,Camera::main.position.y),0.2f);

    /*
    if(glfwGetKey(win.m_window,GLFW_KEY_SPACE) == GLFW_PRESS){
        reg.get<Straw::Components::Physics>(1).body->setGlobalPose( MovePlayer(reg,(playerLocation),XVector::ToVec<physx::PxVec3>(XVector(0,30)),0,-.3f));
       // reg.get<Straw::Components::Physics>(1).body->setGlobalPose(MovePlayer(reg,(playerLocation),XVector::ToVec<physx::PxVec3>(XVector(0,-10)),-1.0,1.0));
        playerLocation = (reg.get<Straw::Components::Physics>(1).body->getGlobalPose().p);

        currentBounces = 0;
    }else{
       reg.get<Straw::Components::Physics>(1).body->setGlobalPose(MovePlayer(reg,(playerLocation),XVector::ToVec<physx::PxVec3>(XVector(0,-15)),0,1.0));
     //  reg.get<Straw::Components::Physics>(1).body->setGlobalPose(MovePlayer(reg,(playerLocation),XVector::ToVec<physx::PxVec3>(XVector(0,-10)),-1.0,1.0));
       playerLocation = (reg.get<Straw::Components::Physics>(1).body->getGlobalPose().p);

       currentBounces = 0;
}
    if(glfwGetKey(win.m_window,GLFW_KEY_D) == GLFW_PRESS){
        physx::PxQueryFilterData filterData = physx::PxQueryFilterData();
        filterData.data.word0 =  GROUP1;
        physx::PxHitFlags hitFlags = physx::PxHitFlag::ePOSITION|physx::PxHitFlag::eNORMAL | physx::PxHitFlag::eMTD;
        physx::PxRaycastCallback* callback;
        physx::PxRaycastBuffer buffer;
        Straw::PhysicsSystem::mScene->raycast(physx::PxVec3(playerLocation.x + 25,playerLocation.y,0),physx::PxVec3(0,1,0),20,buffer,hitFlags,filterData);
        reg.get<Straw::Components::Physics>(1).body->setGlobalPose(MovePlayer(reg,(playerLocation),XVector::ToVec<physx::PxVec3>(XVector(20,0)),-1.0,1.0));
        playerLocation = (reg.get<Straw::Components::Physics>(1).body->getGlobalPose().p);


        //reg.get<Straw::Components::Physics>(1).body->setGlobalPose(MovePlayer(reg,(playerLocation),XVector::ToVec<physx::PxVec3>(XVector(0,-20)),-1.0,1.0));

        currentBounces = 0;
    }if(glfwGetKey(win.m_window,GLFW_KEY_A) == GLFW_PRESS){
       reg.get<Straw::Components::Physics>(1).body->setGlobalPose( MovePlayer(reg,(playerLocation),XVector::ToVec<physx::PxVec3>(XVector(-20,0)),1.0f,1.0f));
       playerLocation = (reg.get<Straw::Components::Physics>(1).body->getGlobalPose().p);
      // reg.get<Straw::Components::Physics>(1).body->setGlobalPose(MovePlayer(reg,(playerLocation),XVector::ToVec<physx::PxVec3>(XVector(0,-20)),-1.0,1.0));
       //reg.get<Straw::Components::Physics>(1).body->setGlobalPose(MovePlayer(reg,(playerLocation),XVector::ToVec<physx::PxVec3>(XVector(0,-10)),-1.0,1.0));
        std::cout << "LEFT" << std::endl;
       currentBounces = 0;
    }
*/
}
);
}
