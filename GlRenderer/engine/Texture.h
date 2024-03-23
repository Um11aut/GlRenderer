#pragma once
#include <type_traits>
#include <fstream>
#include <sstream>
#include <optional>
#include <stb_image.h>
#include <functional>
#include <glad/glad.h>

class Texture {
private:
	struct M {
		uint32_t texture;
	} m;

	explicit Texture(M m) : m(std::move(m)) {}
public:
	static Texture create(const std::string& texture);

    inline virtual void invoke() const { glBindTexture(GL_TEXTURE_2D, m.texture); }

	inline const uint32_t get_texture() const& { return m.texture; }

	struct WithResultOf {
		std::function<Texture()> func;
	};

	Texture(WithResultOf&& res);
};