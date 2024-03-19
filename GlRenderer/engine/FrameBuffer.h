#pragma once
#include <type_traits>
#include <fstream>
#include <sstream>
#include <optional>
#include <functional>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include "Shader.h"

class FrameBuffer {
private:
	struct M {
		uint32_t fbo_multisampled;
		uint32_t fbo;
		uint32_t texture_multisampled;
		uint32_t texture;
		uint32_t width;
		uint32_t height;
	} m;

	explicit FrameBuffer(M m) : m(std::move(m)) {}
public:
	static FrameBuffer create(const uint32_t& width, const uint32_t& height);

	void invoke() const;
	void revoke() const;
	
	void destroy() const;

	const uint32_t get_texture() const&;

	struct WithResultOf {
		std::function<FrameBuffer()> func;
	};

	FrameBuffer(WithResultOf&& res);
};