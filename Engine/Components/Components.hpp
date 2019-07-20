#pragma once
#include "../Math/Math.hpp"
#include <Box2D/Box2D.h>
#include <sol.hpp>
namespace Straw {
namespace Components {
	
struct Transform {
  XVector position{500, 500, 0};
  XVector scale{100, 100, 1};
  float rotation{0};
  Transform(const XVector& pos, const XVector& scale, float rot) : position(pos), scale(scale), rotation(rot) {}
  Transform(sol::table& tbl) {
	  tbl.for_each([&](std::pair<sol::object, sol::object> pair) {
		  if (pair.first.as<std::string>() == "position") {
			  this->position = pair.second.as<XVector>();
		  }
		  if (pair.first.as<std::string>() == "scale") {
			  this->scale = pair.second.as<XVector>();
		  }
		  if (pair.first.as<std::string>() == "rotation") {
			  this->rotation = pair.second.as<float>();
		  }
		  });

  }
};
struct Sprite {
	Sprite(unsigned texid, unsigned int shdid) { texID = texid; ShdrID = shdid; }
	Sprite(sol::table& tbl) {
	  
		tbl.for_each([&](std::pair<sol::object, sol::object> pair) {
			if (pair.first.as<std::string>() == "texID") {
				this->texID = pair.second.as<unsigned int>();
                std::cout << "CreatedSprite " << std::endl;
			}
			});

	}
  ~Sprite() {}
  Vertex vertices[4];	
  unsigned int texID;
  unsigned int ShdrID{ 0 };
};
struct Physics {
  b2Body *body;
  XVector lastpos;
  bool Slope{false};
};

//The Reason this Component is big because it has the Error handling logic inside itself
//and it's the Script Component what were you expecting ?
struct Script {
    template<typename... arg>
    void ExecuteLuaFunction(const sol::protected_function& function,arg... args) {
        sol::protected_function_result res = function(args...);
		if (!res.valid())
		{
			sol::error err = res;
			std::cerr << "failed to Execute FixedUpdate in script::" << err.what() << std::endl;
		}
	}
	Script(sol::state& luastate) {
        this->luastate = &luastate;
    }
	void LoadScript(const std::string& fileName) {
		sol::load_result loadFileResult = luastate->load_file(fileName);
			if (!loadFileResult.valid()) {
				sol::error err = loadFileResult;
				std::cerr << "failed to load file-based script::" << fileName << "::" << err.what() << std::endl;
			}
			sol::protected_function_result res = loadFileResult();
			if (!res.valid()) {
				sol::error err = res;
				std::cerr << "failed to Execture file-based script::" << fileName << "::" << err.what() << std::endl;
			}
			sol::table FunctionsTable = res;
			 
			res = FunctionsTable["Init"]();
			if (!res.valid())
			{
				sol::error err = res;
				std::cerr << "failed to Execute Init in script::" << fileName << "::" << err.what() << std::endl;
			}
			LuaTables.push_back(FunctionsTable);
	}
	void RunUpdate(float dt) {
        for (size_t i  = 0; i < LuaTables.size(); i++) {
            ExecuteLuaFunction(LuaTables[i]["Update"],dt);
		}
    }
    void RunFixedUpdate(float dt) {
        for (size_t i = 0; i < LuaTables.size(); i++) {
            ExecuteLuaFunction(LuaTables[i]["FixedUpdate"],dt);
        }

    }

    void RunLateFixedUpdate(float dt) {
        for (size_t i = 0; i < LuaTables.size(); i++) {
            ExecuteLuaFunction(LuaTables[i]["LateFixedUpdate"],dt);
        }

    }
	std::string fileName;
	sol::state* luastate;
	std::vector<sol::table> LuaTables;
};
} // namespace Components
} // namespace Straw
