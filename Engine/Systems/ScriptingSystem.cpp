#include "ScriptingSystem.hpp"
#include "PhysicsSystem.hpp"
#include "../Rendering/Texture2D.hpp"
#include "../Components/Components.hpp"
#include "../Rendering/Camera.hpp"
sol::state ScriptingSystem::luastate;
void ScriptingSystem::Init(entt::registry& registry, Straw::Window& win) {
	luastate.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);
	//XVector Bindings
	luastate.new_usertype<XVector>("XVector", sol::constructors<XVector(float, float), XVector(float, float, float),XVector(float,float,float,float)>(),
		"x", &XVector::x,
		"y", &XVector::y,
		"z", &XVector::z,
		"w", &XVector::w,
        "Normal",&XVector::Normalize,
        "Mag",&XVector::Magnitude
		, sol::meta_function::addition, sol::overload([](const XVector & self, const XVector & other) {return self + other; }, [](const XVector & self, float other) {return self + other; })
		, sol::meta_function::subtraction, sol::overload([](const XVector & self, const XVector & other) {return self - other; }, [](const XVector & self, float other) {return self - other; })
		, sol::meta_function::division, sol::overload([](const XVector & self, const XVector & other) {return self / other; }, [](const XVector & self, float other) {return self / other; })
        , sol::meta_function::multiplication, sol::overload([](const XVector & self, const XVector & other) {return self * other; }, [](const XVector & self, float other) {return self * other; })
        ,sol::meta_function::equal_to , [](XVector& a,XVector& b){return a == b;});
    luastate.new_usertype<b2Contact>("contact",sol::constructors<>(),"setEnabled",[](b2Contact& cntct,bool val){cntct.SetEnabled(val);});
    luastate["Dot"] = [](XVector a, XVector b) {return XVector::Dot(a, b); };
	luastate["Distance"] = [](XVector a, XVector b) {return XVector::Distance(a, b); };
	luastate["Radians"] = [](float input) {return Radians(input); };
	//Components Existence Checking
	luastate["isSprite"] = [&](entt::entity ent) {return registry.has<Straw::Components::Sprite>(ent); };
	luastate["isBody"] = [&](entt::entity ent) {return registry.has<Straw::Components::Physics>(ent); };
	luastate["isScripted"] = [&](entt::entity ent) {return registry.has<Straw::Components::Script>(ent); };
	//Graphic Work Bindings
	luastate["MakeTexture"] = [&](const std::string & path) {
		Straw::Rendering::Texture2D* tex = new Straw::Rendering::Texture2D(path);
		tex->Init();
		return tex->id;
    };
    luastate["Camera"] = sol::new_table();
    luastate["Camera"]["SetPosition"] = [&](const XVector& pos){Camera::main.position = pos;};
    luastate["Camera"]["GetPosition"] = [&](){return Camera::main.position;};
	luastate["PhysicsSystem"] = sol::new_table();
//Just plain ol' Physx RayCast
    luastate["PhysicsSystem"]["rayCast"] = [](XVector pointa,XVector direction,float distance , sol::function func){

        Straw::PhysicsSystem::RayCast(pointa,direction,distance,[func](const XVector& point,const XVector normal,float distancer,unsigned int entity){

            func(XVector::fromVec(normal),point,distancer,entity);

    });};
    luastate["CreateEntity"] = [&](sol::table tbl) {
		auto ent = registry.create();
		tbl.for_each([&](std::pair<sol::object, sol::object> pair) {
			std::string key = pair.first.as<std::string>();
			sol::table tbl = pair.second.as<sol::table>();
			if (key == "Transform") {
		         MakeComponentFromLua<Straw::Components::Transform>(registry, ent, tbl);
			}if (key == "Sprite") {
			  MakeComponentFromLua<Straw::Components::Sprite>(registry, ent,tbl);
			}
			});
		return ent;
	};
    //Straw::PhysicsSystem::m_world->SetContactListener(collisionBack);
	//Input Binding 
	luastate["Input"] = sol::new_table();
	luastate["Input"]["isKeyDown"] = [&](int keynum) {return glfwGetKey(win.m_window,keynum) == GLFW_PRESS; };
	
	//Components Types Bindings
	luastate.new_usertype <Straw::Components::Sprite>("Sprite", sol::constructors<>(),
		"new", sol::no_constructor,
		"texID", &Straw::Components::Sprite::texID);

    luastate.new_usertype <Straw::Components::Physics>("Body", sol::constructors<>(),
		"new", sol::no_constructor,
        "mass",[](Straw::Components::Physics& self){ return self.bodyjoint.body->getMass();},

        "slope",&Straw::Components::Physics::Slope,
        "onSlope",&Straw::Components::Physics::onSlope,
        "position" , sol::property([](Straw::Components::Physics& self){return XVector::fromVec(self.bodyjoint.body->getGlobalPose().p) * Straw::PhysicsSystem::PPM;},[](Straw::Components::Physics& physics,XVector& other){
        physx::PxTransform temptransform;
        temptransform.p = XVector::ToVec<physx::PxVec3>(other);
        temptransform.q = physx::PxQuat(1.0f);
        physics.bodyjoint.body->setGlobalPose(temptransform);}
    ));
    luastate["PhysicsSystem"]["MovePlayer"] = [&](const XVector& pos,const XVector& vel,float xOffset,float yOffset,bool ignore){
        Straw::PhysicsSystem::currentBounces=0;
        std::tuple<physx::PxTransform,physx::PxVec3,bool> matuple = (Straw::PhysicsSystem::MovePlayer(registry,XVector::ToVec<physx::PxVec3>(pos,true),XVector::ToVec<physx::PxVec3>(vel,true),xOffset,yOffset,ignore));
        physx::PxTransform final = std::get<0>(matuple);
      /*
        if(std::get<1>(matuple).x != 0){
         final =  std::get<0>(Straw::PhysicsSystem::MovePlayer(registry,std::get<0>(matuple).p,physx::PxVec3(std::get<1>(matuple).x,0,0),0,0));
        }
        if(std::get<1>(matuple).y != 0) {
       final = std::get<0>(Straw::PhysicsSystem::MovePlayer(registry,final.p,physx::PxVec3(0,std::get<1>(matuple).y,0),0,0));
        }
*/
        registry.get<Straw::Components::Physics>(1).bodyjoint.body->setGlobalPose(final);
        return (std::get<2>(matuple));

    };

	luastate.new_usertype<Straw::Components::Transform>("Transform", sol::constructors<>(), "new", sol::no_constructor, "position", &Straw::Components::Transform::position, "scale", &Straw::Components::Transform::scale, "rotation", &Straw::Components::Transform::rotation);

	luastate["GetTransformComponent"] = [&](entt::entity ent) {
		return &(registry.get<Straw::Components::Transform>(ent));
	};

	luastate["GetSpriteComponent"] = [&](entt::entity ent) {
		return &(registry.get<Straw::Components::Sprite>(ent));
	};
	luastate["GetPhysicsComponent"] = [&](entt::entity ent) {
		return &(registry.get<Straw::Components::Physics>(ent));
	};
}
void ScriptingSystem::ScriptingSystemUpdate(entt::registry& registry,float dt) {
	auto view = registry.view<Straw::Components::Script>();
	for (auto& ent : view) {
		luastate["owner"] = ent;
		Straw::Components::Script& script = registry.get<Straw::Components::Script>(ent);
		script.RunUpdate(dt);
	}
}
void ScriptingSystem::ScriptingSystemFixedUpdate(entt::registry& registry, float dt) {
    auto view = registry.view<Straw::Components::Script>();
    for (auto& ent : view) {
        luastate["owner"] = ent;
        Straw::Components::Script& script = registry.get<Straw::Components::Script>(ent);
        script.RunFixedUpdate(dt);
    }
}


void ScriptingSystem::ScriptingSystemLateFixedUpdate(entt::registry& registry, float dt) {
    auto view = registry.view<Straw::Components::Script>();
    for (auto& ent : view) {
        luastate["owner"] = ent;
        Straw::Components::Script& script = registry.get<Straw::Components::Script>(ent);
        script.RunLateFixedUpdate(dt);
    }
}

void ScriptingSystem::ExecuteScript(const std::string& fileName) {
	sol::load_result loadFileResult = luastate.load_file(fileName);
	if (!loadFileResult.valid()) {
		sol::error err = loadFileResult;
		std::cerr << "failed to load file-based script::" << fileName << "::" << err.what() << std::endl;
	}
    sol::protected_function_result res = loadFileResult();
	if (!res.valid()) {
		sol::error err = res;
		std::cerr << "failed to Execture file-based script::" << fileName << "::" << err.what() << std::endl;
	}
}
