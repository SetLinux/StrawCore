#include "PhysicsSystem.hpp"
namespace Straw{
int PhysicsSystem::PPM = 50;
b2World* PhysicsSystem::m_world;
RCback PhysicsSystem::rcb;
void PhysicsSystem::PhysicsSystemFixedUpdate(entt::registry &registry){
  auto view = registry.view<Components::Physics>();
  for(auto& ent : view){
    Components::Physics &physics = registry.get<Components::Physics>(ent);
    physics.lastpos = XVector::fromVec(physics.body->GetPosition()) * PPM;
  }
  m_world->Step(1.f/60.f, 8,4);
}

void PhysicsSystem::PhysicsSystemUpdate(entt::registry & registry,float alpha) {
  auto view = registry.view<Components::Transform,Components::Physics>();
  for(auto& ent : view){
    Components::Physics &physics = registry.get<Components::Physics>(ent);
    Components::Transform &transform = registry.get<Components::Transform>(ent);
    transform.position = XVector::Interpolate(physics.lastpos == XVector(0,0) ? XVector::fromVec<b2Vec2>(physics.body->GetPosition()) * PhysicsSystem::PPM : physics.lastpos, XVector::fromVec<b2Vec2>(physics.body->GetPosition()) * PhysicsSystem::PPM, alpha);
  }
}

b2Body* PhysicsSystem::CreateBody(XVector pos,XVector scale,float rotation,entt::entity id){
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(
		       pos.x / PhysicsSystem::PPM,
		       pos.y / PhysicsSystem::PPM);
  bodyDef.angle = rotation;
  //bodyDef.fixedRotation = true;
  //dddddddbodyDef.bullet = true;
   bodyDef.bullet = true;
  bodyDef.userData = (void*)(long(id));
  b2Body* m_body  = m_world->CreateBody(&bodyDef);
  b2PolygonShape shape;
  shape.SetAsBox(
		 scale.x / 2 / PhysicsSystem::PPM,
		 scale.y / 2 / PhysicsSystem::PPM);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtureDef.density = 0.5f;
  fixtureDef.friction = 0.0f;
  fixtureDef.restitution = 0.0f;

  m_body->CreateFixture(&fixtureDef);
  return m_body;
}
b2Body* PhysicsSystem::CreateBody(entt::registry& reg, entt::entity id) {
	Components::Transform& tran = reg.get<Components::Transform>(id);
    return CreateBody(tran.position, tran.scale, tran.rotation, id);

}

void PhysicsSystem::RayCast(
    XVector pointa, XVector pointb,
    std::function<float(const b2Vec2 &point, const b2Vec2 &normal,  unsigned int EntityID,float fraction)> callback) {
  rcb.callback = callback;
  m_world->RayCast(&rcb, XVector::ToVec<b2Vec2>(pointa / PPM),
                   XVector::ToVec<b2Vec2>(pointb / PPM));
}
} // namespace Straw
