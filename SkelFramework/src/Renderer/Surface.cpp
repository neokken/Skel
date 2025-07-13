#include "skelpch.h"
#include "Renderer/Surface.h"

#include <GL/gl.h>

#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_PNM

#include "stbimage/stb_image.h"


// helper macro for line clipping
#define OUTCODE(x,y) (((x)<xmin)?1:(((x)>xmax)?2:0))+(((y)<ymin)?4:(((y)>ymax)?8:0))

skel::Surface::Surface(const int width, const int height, const bool needGPUTexture)
	: m_width(width)
	, m_height(height)
{
	m_pixels = new uint32_t[height * width];
	Clear(0);
	if (needGPUTexture) InitTexture();
}

skel::Surface::Surface(const std::string& file, bool needGPUTexture)
{

	// use stb_image to load the image file
	int n;
	unsigned char* data = stbi_load(file.c_str(), &m_width, &m_height, &n, 0);
	if (!data)
	{
		SKEL_CORE_ERROR("Could not create surface, file \"{}\" not found", file);
		m_width = 0;
		m_height = 0;
		m_pixels = nullptr;
		return;
	}
	const int s = m_width * m_height;
	m_pixels = new uint32_t[s];

	if (n == 1) /* greyscale */ for (int i = 0; i < s; i++)
	{
		const unsigned char p = data[i];
		m_pixels[i] = (255 << 24) | p << 16 | (p << 8) | p;
	}
	if (n == 4)  // RGBA file
	{
		for (int i = 0; i < s; i++)
		{
			m_pixels[i] = data[i * 4 + 3] << 24 | data[i * 4 + 2] << 16 | data[i * 4 + 1] << 8 | data[i * 4 + 0];  
		}
	}
	else
	{
		for (int i = 0; i < s; i++)
		{
			m_pixels[i] = (255 << 24) | data[i * n + 2] << 16 | data[i * n + 1] << 8 | data[i * n + 0];
		}
	}
	// free stb_image data
	stbi_image_free(data);
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

void skel::Surface::Plot(const int2& p, const uint color)
{
	Plot(p.x, p.y, color);
}

void skel::Surface::Line(int x1i, int y1i, int x2i, int y2i, uint color)
{
	float x1 = static_cast<float>(x1i);
	float y1 = static_cast<float>(y1i);
	float x2 = static_cast<float>(x2i);
	float y2 = static_cast<float>(y2i);


	// clip (Cohen-Sutherland, https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm)
	const float xmin = 0, ymin = 0, xmax = (float)m_width - 1, ymax = (float)m_height - 1;
	int c0 = OUTCODE(x1, y1), c1 = OUTCODE(x2, y2);
	bool accept = false;
	while (1)
	{
		if (!(c0 | c1)) { accept = true; break; }
		else if (c0 & c1) break; else
		{
			float x = 0, y = 0;
			const int co = c0 ? c0 : c1;
			if (co & 8) x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1), y = ymax;
			else if (co & 4) x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1), y = ymin;
			else if (co & 2) y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1), x = xmax;
			else if (co & 1) y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1), x = xmin;
			if (co == c0) x1 = x, y1 = y, c0 = OUTCODE(x1, y1);
			else x2 = x, y2 = y, c1 = OUTCODE(x2, y2);
		}
	}
	if (!accept) return;
	float b = x2 - x1, h = y2 - y1, l = fabsf(b);
	if (fabsf(h) > l) l = fabsf(h);
	int il = (int)l;
	float dx = b / (float)l, dy = h / (float)l;
	for (int i = 0; i <= il; i++, x1 += dx, y1 += dy)
		*(m_pixels + (int)x1 + (int)y1 * m_width) = color;

	m_dirty = true;
}

void skel::Surface::Line(const int2& p1, const int2& p2, const uint color)
{
	Line(p1.x, p1.y, p2.x, p2.y, color);
}

void skel::Surface::CopyTo(int x, int y, Surface* d) const
{
	uint* dst = d->m_pixels;
	uint* src = m_pixels;
	if ((src) && (dst))
	{
		int srcwidth = m_width;
		int srcheight = m_height;
		int dstwidth = d->m_width;
		int dstheight = d->m_height;
		if ((srcwidth + x) > dstwidth) srcwidth = dstwidth - x;
		if ((srcheight + y) > dstheight) srcheight = dstheight - y;
		if (x < 0) src -= x, srcwidth += x, x = 0;
		if (y < 0) src -= y * srcwidth, srcheight += y, y = 0;
		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += x + dstwidth * y;
			for (int i = 0; i < srcheight; i++)
			{
				memcpy(dst, src, srcwidth * 4);
				dst += dstwidth, src += m_width;
			}
		}
	}
	d->m_dirty = true;
}

void skel::Surface::CopyTo(const int2& p, Surface* d) const
{
	CopyTo(p.x, p.y, d);
}

void skel::Surface::Box(const int x1, const int y1, const int x2, const int y2, const uint color)
{
	Line(x1, y1, x2, y1, color);
	Line(x2, y1, x2, y2, color);
	Line(x1, y2, x2, y2, color);
	Line(x1, y1, x1, y2, color);
	m_dirty = true;
}

void skel::Surface::Box(const int2& p1, const int2& p2, const uint color)
{
	Box(p1.x, p1.y, p2.x, p2.y, color);
}

void skel::Surface::Bar(int x1, int y1, int x2, int y2, const uint color)
{
	// clipping
	x1 = std::max(x1, 0);
	x2 = std::min(x2, m_width - 1);
	y1 = std::max(y1, 0);
	y2 = std::min(y2, m_height - 1);

	// draw clipped bar
	uint* a = x1 + y1 * m_width + m_pixels;
	for (int y = y1; y <= y2; y++)
	{
		for (int x = 0; x <= (x2 - x1); x++) a[x] = color;
		a += m_width;
	}
	m_dirty = true;
}

void skel::Surface::Bar(const int2& p1, const int2& p2, const uint color)
{
	Bar(p1.x, p1.y, p2.x, p2.y, color);
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
