file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.h")

add_executable(GlRenderer ${SOURCE_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GlRenderer PROPERTY CXX_STANDARD 20)
endif()

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(GlRenderer PRIVATE fmt::fmt)
target_link_libraries(GlRenderer PRIVATE glfw)
target_link_libraries(GlRenderer PRIVATE glad::glad)
target_link_libraries(GlRenderer PRIVATE spdlog::spdlog)
target_link_libraries(GlRenderer PRIVATE OpenGL::GL)