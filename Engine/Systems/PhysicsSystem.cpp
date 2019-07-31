#include "PhysicsSystem.hpp"
#define NODEBUG
namespace Straw{
int PhysicsSystem::PPM = 1;
RCback PhysicsSystem::rcb;

physx::PxDefaultAllocator PhysicsSystem::defaultAlloc;
physx::PxDefaultErrorCallback PhysicsSystem::defaultError;
physx::PxFoundation* PhysicsSystem::mFoundation;
physx::PxPhysics* PhysicsSystem::mPhysics;
physx::PxScene* PhysicsSystem::mScene;
int PhysicsSystem::currentBounces=0,PhysicsSystem::bounceLimit=4;
void PhysicsSystem::PhysicsSystemFixedUpdate(entt::registry &registry){
  auto view = registry.view<Components::Physics>();
  for(auto& ent : view){
    Components::Physics &physics = registry.get<Components::Physics>(ent);
    physics.lastpos = XVector::fromVec(physics.body->getGlobalPose().p) * PPM;
  }
  mScene->simulate(1.f/60.f);
  mScene->fetchResults(true);
}

void PhysicsSystem::PhysicsSystemUpdate(entt::registry & registry,float alpha) {
  auto view = registry.view<Components::Transform,Components::Physics>();
  for(auto& ent : view){
    Components::Physics &physics = registry.get<Components::Physics>(ent);
    Components::Transform &transform = registry.get<Components::Transform>(ent);
    transform.position = XVector::Interpolate(physics.lastpos == XVector(0,0) ? XVector::fromVec(physics.body->getGlobalPose().p) * PhysicsSystem::PPM : physics.lastpos, XVector::fromVec(physics.body->getGlobalPose().p) * PhysicsSystem::PPM, alpha);
  }
}

physx::PxRigidDynamic* PhysicsSystem::CreateBody(XVector pos,XVector scale,float rotation,entt::entity id,bool ignoreCasts){
    using namespace physx;
    PxReal cubeDensity = 1000.0f;
    scale.z = 2;
    PxBoxGeometry cubeGeomtry(XVector::ToVec<PxVec3>(scale/2.0f / PhysicsSystem::PPM,true));
    PxTransform cubeTransform(XVector::ToVec<PxVec3>(pos/ PhysicsSystem::PPM,true));
    cubeTransform.q = PxQuat(rotation,PxVec3(0,0,1));
    cubeTransform.p = XVector::ToVec<PxVec3>(pos / PhysicsSystem::PPM ,true);
    PxMaterial* material = mPhysics->createMaterial(0.5f,0.1f,1.0f);
    PxRigidDynamic* cubeActor = mPhysics->createRigidDynamic(cubeTransform);
    cubeActor->userData = (void*)(long)id;
    PxShape* shape = PxRigidActorExt::createExclusiveShape(*cubeActor,cubeGeomtry,*material);
    PxFilterData filterData;
    if(ignoreCasts){
        filterData.word0 = (1<<3);
     }else {
           filterData.word0 = (1<<0);
     }

    shape->setQueryFilterData(filterData);
    cubeActor->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z | PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
    cubeActor->setMass(100);
    mScene->addActor(*cubeActor);
    return cubeActor;
}
physx::PxRigidDynamic* PhysicsSystem::CreateBody(entt::registry& reg, entt::entity id,bool ignoreCasts) {
	Components::Transform& tran = reg.get<Components::Transform>(id);
    return CreateBody(tran.position, tran.scale, tran.rotation, id,ignoreCasts);

}

void PhysicsSystem::RayCast(
    XVector pointa, XVector direction,float distance,
    std::function<void(const XVector &point, const XVector &normal,  unsigned int EntityID)> callback) {
    physx::PxRaycastBuffer buffer;
    physx::PxQueryFilterData filterData = physx::PxQueryFilterData();
    filterData.data.word0 = (1 << 0);
    physx::PxHitFlags hitFlags = physx::PxHitFlag::ePOSITION|physx::PxHitFlag::eNORMAL|physx::PxHitFlag::eMTD;

    mScene->raycast(XVector::ToVec<physx::PxVec3>(pointa,true),XVector::ToVec<physx::PxVec3>(direction,true),distance,buffer,hitFlags,filterData);
   // std::cout << buffer.hasAnyHits() << std::endl;
    if(buffer.hasAnyHits()){

        unsigned int entity = (unsigned int)(long)buffer.block.actor->userData;
        XVector point = XVector::fromVec(buffer.block.position);
        XVector normal = XVector::fromVec(buffer.block.normal);
        callback(point,normal,entity);
    }
}
} // namespace Straw
