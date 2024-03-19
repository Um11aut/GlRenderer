#include "FrameBuffer.h"

FrameBuffer FrameBuffer::create(std::shared_ptr<ImVec2> size)
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
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGBA, size->x, size->y, GL_TRUE);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size->x, size->y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, regularTexture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size->x, size->y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

    ImVec2 last_size = *size;

    return FrameBuffer(M{
        .fbo_multisampled = multisampledFBO,
        .fbo = regularFBO,
        .rbo = rbo,
        .texture_multisampled = multisampledTexture,
        .texture = regularTexture,
        .size = size,
        .last_size = last_size
    });
}


void FrameBuffer::invoke()
{
    if(m.last_size.x != m.size->x || m.last_size.y != m.size->y) {
        resize();

        m.last_size.x = m.size->x;
        m.last_size.y = m.size->y;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m.fbo_multisampled);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m.fbo);

    glBlitNamedFramebuffer(m.fbo_multisampled, m.fbo, 0, 0, m.size->x, m.size->y, 0, 0, m.size->x, m.size->y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m.fbo_multisampled);
}

void FrameBuffer::revoke() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize() const
{
    glViewport(0, 0, m.size->x, m.size->y);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m.texture_multisampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGBA, m.size->x, m.size->y, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    // Update regular texture
    glBindTexture(GL_TEXTURE_2D, m.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m.size->x, m.size->y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Update renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, m.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m.size->x, m.size->y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
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