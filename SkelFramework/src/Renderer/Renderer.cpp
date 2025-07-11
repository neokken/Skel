#include "skelpch.h"
#include "Renderer/Renderer.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/Surface.h"





static const char* vertexSrc = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aTex;

    out vec2 TexCoord;

    uniform vec2 uPosition;
    uniform vec2 uSize;
    uniform vec2 uScreenSize;

    void main()
    {
        vec2 pos = aPos * uSize + uPosition;
        vec2 ndc = (pos / uScreenSize) * 2.0 - 1.0;
        gl_Position = vec4(ndc * vec2(1, -1), 0.0, 1.0);
        TexCoord = aTex;
    }
)";

static const char* fragmentSrc = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoord;
    uniform sampler2D uTexture;

    void main()
    {
        FragColor = texture(uTexture, TexCoord);
    }
)";


static GLuint CompileShader(GLenum type, const char* src)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}


skel::Renderer::Renderer(const int width, const int height)
	: m_width(width)
	, m_height(height)
{
    InitFramebuffer();
    InitQuad();
    InitShader();
}




skel::Renderer::~Renderer()
{
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteTextures(1, &m_colorTexture);
    glDeleteBuffers(1, &m_quadVBO);
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteProgram(m_shader);
}



void skel::Renderer::InitFramebuffer()
{
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glGenTextures(1, &m_colorTexture);
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        SKEL_CORE_ERROR("Framebuffer incomplete!\n");
        assert(0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void skel::Renderer::InitQuad()
{
	constexpr float quadVertices[] = {
        // pos      // tex
         0.0f, 0.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 1.0f,
         1.0f, 1.0f, 1.0f, 1.0f,
    };

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);

    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void skel::Renderer::InitShader()
{
    GLuint vert = CompileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint frag = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    m_shader = glCreateProgram();
    glAttachShader(m_shader, vert);
    glAttachShader(m_shader, frag);
    glLinkProgram(m_shader);

    glDeleteShader(vert);
    glDeleteShader(frag);
}


void skel::Renderer::Begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glViewport(0, 0, m_width, m_height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void skel::Renderer::End()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void skel::Renderer::Resize(const int width, const int height)
{
    SKEL_CORE_INFO("Resizing renderer to {} x {}", width, height);
    if (width == 0 || height == 0)
        return;

    m_width = width;
    m_height = height;

    // Recreate the texture
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Update framebuffer size
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        SKEL_CORE_ERROR("Framebuffer incomplete after resize!");
        assert(0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void skel::Renderer::Clear(const float4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void skel::Renderer::BlitSurface(Surface* surface, const int x, const int y)
{
    const uint surfaceTexture = surface->GetTextureID();

    if (surfaceTexture == 0)
    {
        SKEL_CORE_WARN("Tried to blit a surface, however the surface does not have a GPUTexture");
        return;
    }

    glUseProgram(m_shader);

    glUniform2f(glGetUniformLocation(m_shader, "uPosition"), static_cast<float>(x), static_cast<float>(y));
    glUniform2f(glGetUniformLocation(m_shader, "uSize"), static_cast<float>(surface->GetWidth()), static_cast<float>(surface->GetHeight()));
    glUniform2f(glGetUniformLocation(m_shader, "uScreenSize"), static_cast<float>(m_width), static_cast<float>(m_height));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, surface->GetTextureID());
    glUniform1i(glGetUniformLocation(m_shader, "uTexture"), 0);

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
