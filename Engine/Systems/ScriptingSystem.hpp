#include <sol.hpp>
#include "../Math/Math.hpp"
#include "../Components/Components.hpp"
#include "../../entt/entt.hpp"
#include "../Window/Window.hpp"
class GLFWwindow;
class ScriptingSystem {
public:
    static void Init(entt::registry& registry,Straw::Window& win);
	static sol::state luastate;
	static void ScriptingSystemUpdate( entt::registry& registry,float dt);
	static void ScriptingSystemFixedUpdate( entt::registry& registry,float dt);
    static void ScriptingSystemLateFixedUpdate(entt::registry& registry,float dt);
	static void ExecuteScript(const std::string& fileName);
	template<class T>
	static void MakeComponentFromLua(entt::registry& registry,entt::entity id ,sol::table& tbl) {
		registry.assign<T>(id, tbl);
	}
};
 
