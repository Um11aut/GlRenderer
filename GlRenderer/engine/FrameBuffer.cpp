#include "FrameBuffer.h"

FrameBuffer FrameBuffer::create(const uint32_t& width, const uint32_t& height)
{
    // Enable multisampling
    glEnable(GL_MULTISAMPLE);

    // Generate multisampled framebuffer
    GLuint multisampledFBO;
    glGenFramebuffers(1, &multisampledFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, multisampledFBO);

    // Generate multisampled texture
    GLuint multisampledTexture;
    glGenTextures(1, &multisampledTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampledTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGBA, width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisampledTexture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Generate regular framebuffer
    GLuint regularFBO;
    glGenFramebuffers(1, &regularFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, regularFBO);

    // Generate regular texture
    GLuint regularTexture;
    glGenTextures(1, &regularTexture);
    glBindTexture(GL_TEXTURE_2D, regularTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, regularTexture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

    return FrameBuffer(M{
        .fbo_multisampled = multisampledFBO,
        .fbo = regularFBO,
        .texture_multisampled = multisampledTexture,
        .texture = regularTexture,
        .width = width,
        .height = height,
    });
}


void FrameBuffer::invoke() const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m.fbo_multisampled);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m.fbo);

    glBlitNamedFramebuffer(m.fbo_multisampled, m.fbo, 0, 0, m.width, m.height, 0, 0, m.width, m.height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m.fbo_multisampled);
}

void FrameBuffer::revoke() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::destroy() const
{
	glDeleteFramebuffers(1, &m.fbo);
	glDeleteTextures(1, &m.texture);
}

const uint32_t FrameBuffer::get_texture() const &
{
    return m.texture;
}

FrameBuffer::FrameBuffer(WithResultOf&& res)
{
	auto other = res.func();

	m = std::move(other.m);
}