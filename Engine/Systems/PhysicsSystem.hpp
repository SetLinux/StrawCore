#pragma  once
#include <Box2D/Box2D.h>
#include "../../entt/entt.hpp"
#include "../Components/Components.hpp"

#include <include/PxPhysicsAPI.h>

namespace Straw{
class  RCback : public b2RayCastCallback{
public:
  entt::registry* rg;
  
  float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point,
                        const b2Vec2 &normal, float32 fraction) {
    // Just a bunch of shitty castings because of the [[~Percisions Errrors~]] Shit 
      if((unsigned int)((long)(fixture->GetBody()->GetUserData())) == 1){
          return -1;
      }
      return callback(point,normal,(unsigned int)((long)(fixture->GetBody()->GetUserData())),fraction);

    return fraction;
  }
  std::function<float(const b2Vec2& point, const b2Vec2 &normal,unsigned int EntityID,float fraction)> callback;
};
class CLBack : public b2ContactListener{
public:
  void BeginContact(b2Contact* contact){
    callback(contact);
  }
  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
    b2WorldManifold manifold;
    contact->GetWorldManifold(&manifold);
    presolve(XVector::fromVec(manifold.normal),(unsigned int)((long)(contact->GetFixtureA()->GetBody()->GetUserData())),(unsigned int)((long)(contact->GetFixtureB()->GetBody()->GetUserData())),contact);
  }

  std::function<void(b2Contact*)> callback;
   std::function<void(const XVector& normal,const unsigned int ent1,const unsigned int ent2,b2Contact* cntc)> presolve;

};

class PhysicsSystem{
public:
  static int PPM;
  static physx::PxFoundation* mFoundation;
  static physx::PxPhysics* mPhysics;
  static physx::PxScene* mScene;
  static int currentBounces,bounceLimit;
  static std::tuple<physx::PxTransform,physx::PxVec3> MovePlayer(entt::registry& reg,physx::PxVec3 pos,physx::PxVec3  vel,const float xOffset,const float yOffset) {
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
      PxBoxGeometry cubeGeomtry(XVector::ToVec<PxVec3>(XVector(49.5,49.5,5,2)/2.0f,true));
      PxTransform playertransform;
      playertransform.p = PxVec3(pos.x+.5f ,pos.y - .5f ,0);
      playertransform.q = PxQuat(1.0);
      PxSweepBuffer hit;

      PxQueryFilterData filterData = PxQueryFilterData();
      filterData.data.word0 =  (1<<0);
      bool hitCount =  Straw::PhysicsSystem::mScene->sweep(cubeGeomtry,playertransform,vel.getNormalized(),vel.magnitude(),hit,hitFlags,filterData);
       if(hit.hasAnyHits() ){
       PxVec3 groundnormal = hit.block.normal;

       expectedtransform.p = pos + vel.getNormalized() * (hit.block.distance - .4f);
       float TOI = (XVector::fromVec(pos - expectedtransform.p) / XVector::fromVec(vel)).Magnitude();
       float dotproduct = (vel.x * groundnormal.y+vel.y*groundnormal.x) * (1 - TOI);
        if((expectedtransform.p - pos) != PxVec3(0,0,0)){
       PxTransform Step1 = std::get<0>(MovePlayer(reg,pos + vel.getNormalized() * 0.01f,(expectedtransform.p - pos) - vel.getNormalized() * 0.01f,0,0));
       return std::make_tuple(Step1,PxVec3(dotproduct* groundnormal.y,dotproduct*groundnormal.x,0));

       }
       return std::make_tuple(expectedtransform,PxVec3(0.0f));
      }else{
          PxTransform tran;
          tran.p = expectedtransform.p;
          tran.q = PxQuat(1.0f);
          return std::make_tuple(tran,PxVec3(0,0,0));
      }
      }else{

          PxTransform tran;
          tran.p = pos+ vel;
          tran.q = PxQuat(1.0f);
        return std::make_tuple(tran,PxVec3(0,0,0));
      }

  }

  static void Init() {
        mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION,defaultAlloc,defaultError);
        mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION,*mFoundation,physx::PxTolerancesScale());
        physx::PxSceneDesc scenedesk(mPhysics->getTolerancesScale());
        scenedesk.gravity = physx::PxVec3(0.0f,-18.6f,0.0);
        if(!scenedesk.cpuDispatcher)
            {
                physx::PxDefaultCpuDispatcher* mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
                scenedesk.cpuDispatcher = mCpuDispatcher;
            }

            if(!scenedesk.filterShader)
            {
                scenedesk.filterShader  = physx::PxDefaultSimulationFilterShader;
            }

        mScene = mPhysics->createScene(scenedesk);

  }

  static void PhysicsSystemUpdate(entt::registry &regisry, float alpha);
  static void PhysicsSystemFixedUpdate(entt::registry &regisry);
  static physx::PxRigidDynamic *CreateBody(XVector pos, XVector scale,float rotation, entt::entity id = 0,bool ignoreCasts = false);
  static physx::PxRigidDynamic *CreateBody(entt::registry& reg,entt::entity id,bool ignoreCasts = false);
  static void
  RayCast(XVector pointa, XVector direction,float distance,std::function<void(const XVector &point, const XVector &normal,unsigned int EntityID)> callback);

private:
  static RCback rcb;
  static physx::PxDefaultAllocator defaultAlloc;
  static physx::PxDefaultErrorCallback defaultError;
};
} // namespace Straw
