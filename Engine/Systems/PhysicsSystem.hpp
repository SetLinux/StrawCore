#pragma  once
#include <Box2D/Box2D.h>
#include "../../entt/entt.hpp"
#include "../Components/Components.hpp"

#include <include/PxPhysicsAPI.h>

namespace Straw{

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

class  RCback : public physx::PxQueryFilterCallback{
public:
   physx::PxQueryHitType::Enum preFilter(const physx::PxFilterData &filterData, const physx::PxShape *shape, const physx::PxRigidActor *actor, physx::PxHitFlags &queryFlags) override{
    std::cout << "OKEY YOU GOT PRE" << std::endl;
    return physx::PxQueryHitType::eBLOCK;
   }
    physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData &filterData, const physx::PxQueryHit &hit) override{
        std::cout << 31 << std::endl;

        return physx::PxQueryHitType::eBLOCK;
    }
};
class PhysicsSystem{
public:
  static int PPM;
  static physx::PxFoundation* mFoundation;
  static physx::PxPhysics* mPhysics;
  static physx::PxScene* mScene;
  static int currentBounces,bounceLimit;
  static Straw::RCback rcbck;
  static std::tuple<physx::PxTransform,physx::PxVec3,bool> MovePlayer(entt::registry& reg,physx::PxVec3 pos,physx::PxVec3  vel,const float xOffset,const float yOffset,bool ignore= false) {
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
      PxBoxGeometry cubeGeomtry(XVector::ToVec<PxVec3>(XVector(64,64,5,2)/2.0f,true));
      PxTransform playertransform;
      playertransform.p = PxVec3(pos.x ,pos.y ,0);
      playertransform.q = PxQuat(1.0);
      PxSweepHit bufhit[256];
      PxSweepBuffer hit(bufhit,256);

      PxQueryFilterData filterData = PxQueryFilterData();
      filterData.data.word0 =  (1<<0) | (1<<1) | (1 << 6);
      bool hitCount =  Straw::PhysicsSystem::mScene->sweep(cubeGeomtry,playertransform,vel.getNormalized(),vel.magnitude(),hit,hitFlags,filterData,&rcbck);
      bool collision = false;
      int maxHits = hit.getNbAnyHits();
      int currenthit = -1;
        int toUseHit = 0;
        bool HittenShit = false;
      if(hit.hasAnyHits()){
          while(currenthit < maxHits - 1 && !collision){
              collision = true;
              currenthit++;
         if(reg.get<Straw::Components::Physics>((unsigned int)(long)hit.getTouch(currenthit).actor->userData).oneWay){
            collision = false;
        XVector hittenscale = reg.get<Straw::Components::Transform>((unsigned int)(long)hit.getTouch(currenthit).actor->userData).scale;
       PxVec3 actualvel = vel + PxVec3(xOffset,yOffset,0);
       if(((hit.getTouch(currenthit).position.y - (hit.getTouch(currenthit).actor->getGlobalPose().p.y - (hittenscale.y/2))) >1 && !hit.getTouch(currenthit).hadInitialOverlap() && hit.getTouch(currenthit).position.y < playertransform.p.y + 25) ){
           collision = true;


      }
       if((actualvel.x != 0 && actualvel.y >= 0)){collision = false;}


         }
         if(!HittenShit && collision){
            HittenShit = true;
            toUseHit = currenthit;
         }
      }
      }
      if(hit.hasAnyHits()&& !ignore  && collision){
       PxVec3 groundnormal = hit.getTouch(toUseHit).normal;
       expectedtransform.p = pos + vel.getNormalized() * (hit.getTouch(toUseHit).distance - 0.04f);
       float TOI = (XVector::fromVec(pos - expectedtransform.p) / XVector::fromVec(vel)).Magnitude();
       float dotproduct = (vel.x * groundnormal.y+vel.y*groundnormal.x) * (1 - TOI);
       XVector hittenscale = reg.get<Straw::Components::Transform>((unsigned int)(long)hit.getTouch(toUseHit).actor->userData).scale;
       PxBoxGeometry hittenGeo(XVector::ToVec<PxVec3>(XVector(hittenscale.x,hittenscale.y,3) / 2));
       if((expectedtransform.p - pos) != PxVec3(0,0,0)){

       std::tuple<PxTransform,PxVec3,bool> Step1 =(MovePlayer(reg,pos + vel.getNormalized() * 0.01f,(expectedtransform.p - pos) - vel.getNormalized() * 0.01f ,xOffset,yOffset));
       return std::make_tuple(std::get<0>(Step1),PxVec3(dotproduct* groundnormal.y,dotproduct*groundnormal.x,0),true);
       }
       return std::make_tuple(expectedtransform,PxVec3(0.0f),true);
      }else{
          PxTransform tran;
          tran.p = pos+ vel;
          tran.q = PxQuat(1.0f);
          return std::make_tuple(tran,PxVec3(0,0,0),false);
      }
      }else{

          PxTransform tran;
          tran.p = pos+ vel;
          tran.q = PxQuat(1.0f);
          return std::make_tuple(tran,PxVec3(0,0,0),false);
      }

  }

  static void Init(entt::registry& reg) {
        mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION,defaultAlloc,defaultError);
        registry = &reg;
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
  static  PhysicsBodyJoint CreateBody(XVector pos, XVector scale,float rotation, entt::entity id = 0,bool ignoreCasts = false);
  static PhysicsBodyJoint CreateBody(entt::registry& reg,entt::entity id,bool ignoreCasts = false);
  static void
  RayCast(XVector pointa, XVector direction,float distance,std::function<void(const XVector &point, const XVector &normal,float distancer,unsigned int EntityID)> callback,unsigned int AdditionalFilters = (-1));
  static entt::registry* registry;

private:

  static physx::PxDefaultAllocator defaultAlloc;
  static physx::PxDefaultErrorCallback defaultError;
};

} // namespace Straw
