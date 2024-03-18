#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include "Shader.h"
#include "VertexObject.h"
#include "UniformObject.h"
#include "Gui.h"
#include "FrameBuffer.h"
#include <imgui.h>
#include <glad/glad.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include "gui/SceneViewport.h"
#include "Model.h"

class Renderer {
private:
	struct M {
		std::unique_ptr<Camera> camera;
		std::unique_ptr<FrameBuffer> frame_buffer;
		std::unique_ptr<Gui> gui;
		std::vector<std::unique_ptr<Model>> models;
	} m;

	explicit Renderer(M m) : m(std::move(m)) {}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	template<typename... T>
	constexpr void invoke(std::unique_ptr<T>&... objects);
public:
	static Renderer create(GLFWwindow* window);

	struct WithResultOf {
		std::function<Renderer()> func;
	};

	Renderer(WithResultOf&& res);

	void draw();
	void destroy() const;

	Renderer(Renderer&& other) noexcept;
	~Renderer();
};
