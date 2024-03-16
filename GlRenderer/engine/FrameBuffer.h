#pragma once
#include <type_traits>
#include <fstream>
#include <sstream>
#include <optional>
#include <functional>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

class FrameBuffer {
private:
	struct M {
		uint32_t fbo;
		uint32_t texture;
		uint32_t rbo;
	} m;

	explicit FrameBuffer(M m) : m(std::move(m)) {}
public:
	static FrameBuffer create(const uint32_t& width, const uint32_t& height);

	void invoke() const;
	static constexpr inline void revoke() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	
	void destroy() const;
	void rescale(const uint32_t& width, const uint32_t& height);

	inline const uint32_t get_texture() const& { return m.texture; }

	struct WithResultOf {
		std::function<FrameBuffer()> func;
	};

	FrameBuffer(WithResultOf&& res);
};