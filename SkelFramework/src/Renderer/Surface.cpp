#include "skelpch.h"
#include "Renderer/Surface.h"

#include <GL/gl.h>

skel::Surface::Surface(const int32_t width, const int32_t height, const bool needGPUTexture)
	: m_width(width)
	, m_height(height)
{
	m_pixels = new uint32_t[height * width];
	Clear(0);
	if (needGPUTexture) InitTexture();
}

skel::Surface::~Surface()
{
	delete[] m_pixels;

	if (m_textureID) glDeleteTextures(1, &m_textureID);
}

void skel::Surface::Clear(const uint color)
{
	std::fill_n(m_pixels, m_width * m_height, color);
	m_dirty = true;
}

void skel::Surface::Plot(const int x, const int y, const uint32_t color)
{
	if (x < 0 || y < 0 || x >= static_cast<int>(m_width) || y >= static_cast<int>(m_height)) return;
	m_pixels[y * m_width + x] = color;
	m_dirty = true;
}

void skel::Surface::UpdateGPUTexture()
{
	if (!m_dirty || m_textureID==0) return;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
	m_dirty = false;
}

uint32_t skel::Surface::GetTextureID()
{
	UpdateGPUTexture();
	return m_textureID;
}

void skel::Surface::InitTexture()
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
