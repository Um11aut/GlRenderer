#include "FrameBuffer.h"

FrameBuffer FrameBuffer::create(const uint32_t& width, const uint32_t& height)
{
    uint32_t fbo, texture, rbo;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height); // 4x multisampling
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return FrameBuffer(M{
        .fbo = fbo,
        .texture = texture,
        .rbo = rbo,
        });
}

void FrameBuffer::invoke() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m.fbo);
}

void FrameBuffer::destroy() const
{
	glDeleteFramebuffers(1, &m.fbo);
	glDeleteTextures(1, &m.texture);
	glDeleteRenderbuffers(1, &m.rbo);
}

void FrameBuffer::rescale(const uint32_t& width, const uint32_t& height)
{
	glBindTexture(GL_TEXTURE_2D, m.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m.texture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m.rbo);
}

FrameBuffer::FrameBuffer(WithResultOf&& res)
{
	auto other = res.func();

	m = std::move(other.m);
}
