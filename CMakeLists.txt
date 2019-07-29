cmake_minimum_required(VERSION 2.8.9)
project(test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -mavx2")
find_package(Lua53 REQUIRED)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package (Threads)
#pkg_search_module(GLFW REQUIRED GLFW3)
#pkg_search_module(GLEW REQUIRED GLEW)

add_executable(test entt/entt.hpp
Engine/Rendering/Texture2D.hpp
Engine/Rendering/Texture2D.cpp
Engine/Rendering/Camera.hpp
Engine/Rendering/Camera.cpp
Engine/Rendering/Shader.hpp
Engine/Rendering/Shader.cpp
Engine/Components/Components.hpp
Engine/Systems/RenderingSystem.hpp
Engine/Utils/Threading.hpp
Engine/Systems/RenderingSystem.cpp
Engine/Systems/ScriptingSystem.cpp
Engine/Systems/PhysicsSystem.hpp
Engine/Systems/PhysicsSystem.cpp
Engine/Rendering/Renderer.hpp
Engine/Rendering/Renderer.cpp
Engine/Window/Window.hpp
Engine/Window/Window.cpp
#NanoVGShit
Engine/Utils/nanovg.c
Engine/Utils/NanoVGImplementation.cpp
Engine/Math/Math.hpp
${CMAKE_CURRENT_SOURCE_DIR}/libs/SOIL2/srcs/SOIL2.c
${CMAKE_CURRENT_SOURCE_DIR}/libs/SOIL2/srcs/etc1_utils.c
${CMAKE_CURRENT_SOURCE_DIR}/libs/SOIL2/srcs/image_DXT.c
${CMAKE_CURRENT_SOURCE_DIR}/libs/SOIL2/srcs/image_helper.c
main.cpp
)
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/SOIL2/incs ${CMAKE_CURRENT_SOURCE_DIR}/libs/ ${CMAKE_CURRENT_SOURCE_DIR}/libs/include/ )

target_link_libraries(test PRIVATE GL glfw tbb GLEW ${LUA53_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}   ${GLFW_LIBRARIES}    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libPhysX_static_64.a  ${CMAKE_CURRENT_SOURCE_DIR}/libs/libPhysXPvdSDK_static_64.a ${CMAKE_CURRENT_SOURCE_DIR}/libs/libPhysXExtensions_static_64.a ${CMAKE_CURRENT_SOURCE_DIR}/libs/libPhysXFoundation_static_64.a  ${CMAKE_CURRENT_SOURCE_DIR}/libs/libPhysXCommon_static_64.a dl   ${CMAKE_CURRENT_SOURCE_DIR}/libs/Box2D/libBox2D.a)
