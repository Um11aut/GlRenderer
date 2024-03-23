#pragma once
#include <type_traits>
#include <fstream>
#include <sstream>
#include <optional>
#include <functional>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "Shader.h"
#include <imgui.h>
#include <chrono>

class FrameBuffer {
private:
	struct M {
		uint32_t fbo_multisampled;
		uint32_t fbo;
		uint32_t rbo_multisampled;
		uint32_t rbo;
		uint32_t texture_multisampled;
		uint32_t texture;
		uint32_t samples;

		std::shared_ptr<ImVec2> size;
		ImVec2 last_size;
	} m;

	explicit FrameBuffer(M m) : m(std::move(m)) {}
public:
	static FrameBuffer create(std::shared_ptr<ImVec2> size, const uint32_t samples);

	void invoke();
	void revoke() const;
	
	void resize() const;
	void destroy() const;

	const uint32_t get_texture() const&;

	struct WithResultOf {
		std::function<FrameBuffer()> func;
	};

	FrameBuffer(WithResultOf&& res);
};