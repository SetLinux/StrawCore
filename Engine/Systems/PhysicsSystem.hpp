#pragma  once
#include <Box2D/Box2D.h>
#include "../../entt/entt.hpp"
#include "../Components/Components.hpp"
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
  std::function<void(b2Contact*)> callback;
};

class PhysicsSystem{
public:
  static int PPM;
  static b2World* m_world;
  
  static void Init() {
    m_world = new b2World(b2Vec2(0, -40.f));
    m_world->SetContinuousPhysics(true);
  }

  static void PhysicsSystemUpdate(entt::registry &regisry, float alpha);
  static void PhysicsSystemFixedUpdate(entt::registry &regisry);
  static b2Body *CreateBody(XVector pos, XVector scale,float rotation, entt::entity id = 0);
  static b2Body *CreateBody(entt::registry& reg,entt::entity id);
  static void
  RayCast(XVector pointa, XVector point,std::function<float(const b2Vec2 &point, const b2Vec2 &normal,unsigned int EntityID,float fraction)> callback);
   
private:
  static RCback rcb;
};
} // namespace Straw
