file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.h")

set(IMGUI_SOURCE_DIR "${CMAKE_SOURCE_DIR}/deps/imgui")
include_directories(${IMGUI_SOURCE_DIR} ${IMGUI_SOURCE_DIR}/backends)

set(STB_SOURCE_DIR "${CMAKE_SOURCE_DIR}/deps/stb")
include_directories(${STB_SOURCE_DIR})
link_directories(${STB_SOURCE_DIR})

set(GLAD_SOURCE_DIR "${CMAKE_SOURCE_DIR}/deps/glad/include")
include_directories(${GLAD_SOURCE_DIR})
link_directories(${GLAD_SOURCE_DIR})

add_executable(GlRenderer ${SOURCE_FILES} 
${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp 
${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp 
${IMGUI_SOURCE_DIR}/imgui.cpp 
${IMGUI_SOURCE_DIR}/imgui_draw.cpp 
${IMGUI_SOURCE_DIR}/imgui_demo.cpp 
${IMGUI_SOURCE_DIR}/imgui_tables.cpp 
${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
${GLAD_SOURCE_DIR}/../src/glad.c)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GlRenderer PROPERTY CXX_STANDARD 23)
endif()

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(GlRenderer PRIVATE fmt::fmt)
target_link_libraries(GlRenderer PRIVATE glfw)
target_link_libraries(GlRenderer PRIVATE spdlog::spdlog)
target_link_libraries(GlRenderer PRIVATE OpenGL::GL)
target_link_libraries(GlRenderer PRIVATE glm::glm)

set(source "${CMAKE_SOURCE_DIR}/resources")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(
    TARGET GlRenderer
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
    DEPENDS ${destination}
    COMMENT "symbolic link common folder from ${source} => ${destination}")