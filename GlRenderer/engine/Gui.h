#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <type_traits>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "external/Camera.h"
#include "Model.h"

class Gui {
private:
	struct M {
		GLFWwindow* window;
		std::shared_ptr<Camera::CameraControls> camera_controls;
		std::shared_ptr<Camera::ProjectionControls> camera_proj_controls;
		std::shared_ptr<Camera::ViewControls> camera_view_controls;
		std::shared_ptr<ImVec2> scene_viewport_size;
	} m;

	explicit Gui(M m) : m(std::move(m)) {}
public:
	static Gui create(GLFWwindow* window, std::unique_ptr<Camera>& camera);

	void invoke_start() const;

	static void enable_dark_theme();
	static void draw_main_dockspace(bool *p_open);
	void draw_camera_controls_window() const;
	void render_scene(const uint32_t framebuffer_texture, std::unique_ptr<Camera>& camera) const;
	void draw_models_control(std::vector<std::unique_ptr<Model>>& models, std::unique_ptr<Camera>& camera) const;

	void invoke_end() const;
	void destroy() const;

	inline std::shared_ptr<ImVec2> get_scene_viewport_size() const { return m.scene_viewport_size; }

	struct WithResultOf {
		std::function<Gui()> func;
	};

	Gui(WithResultOf&& res);
};