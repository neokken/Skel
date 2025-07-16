#include "skelpch.h"
#include "Renderer/Surface.h"



#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_PNM

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glad/glad.h"
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
			m_pixels[i] = data[i * 4 ] << 24 | data[i * 4 + 1] << 16 | data[i * 4 + 2] << 8 | data[i * 4 + 3];  
		}
	}
	else
	{
		for (int i = 0; i < s; i++)
		{
			m_pixels[i] = (255 << 24) | data[i * n ] << 16 | data[i * n + 1] << 8 | data[i * n + 2];
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

void skel::Surface::Plot(const int x, const int y, const uint color)
{
	if (x < 0 || y < 0 || x >= static_cast<int>(m_width) || y >= static_cast<int>(m_height)) return;
	m_pixels[y * m_width + x] = color;
	m_dirty = true;
}

void skel::Surface::Plot(const int2& p, const uint color)
{
	Plot(p.x, p.y, color);
}

void skel::Surface::Line(const int x1, const int y1, const int x2, const int y2, const uint color)
{
	float x1f = static_cast<float>(x1);
	float y1f = static_cast<float>(y1);
	float x2f = static_cast<float>(x2);
	float y2f = static_cast<float>(y2);


	// clip (Cohen-Sutherland, https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm)
	const float xmin = 0, ymin = 0, xmax = static_cast<float>(m_width) - 1, ymax = static_cast<float>(m_height) - 1;
	int c0 = OUTCODE(x1f, y1f), c1 = OUTCODE(x2f, y2f);
	bool accept = false;
	while (1)
	{
		if (!(c0 | c1)) { accept = true; break; }
		else if (c0 & c1) break; else
		{
			float x = 0, y = 0;
			const int co = c0 ? c0 : c1;
			if (co & 8) x = x1f + (x2f - x1f) * (ymax - y1f) / (y2f - y1f), y = ymax;
			else if (co & 4) x = x1f + (x2f - x1f) * (ymin - y1f) / (y2f - y1f), y = ymin;
			else if (co & 2) y = y1f + (y2f - y1f) * (xmax - x1f) / (x2f - x1f), x = xmax;
			else if (co & 1) y = y1f + (y2f - y1f) * (xmin - x1f) / (x2f - x1f), x = xmin;
			if (co == c0) x1f = x, y1f = y, c0 = OUTCODE(x1f, y1f);
			else x2f = x, y2f = y, c1 = OUTCODE(x2f, y2f);
		}
	}
	if (!accept) return;
	const float b = x2f - x1f;
	const float h = y2f - y1f;
	float l = fabsf(b);
	if (fabsf(h) > l) l = fabsf(h);
	const int il = static_cast<int>(l);
	const float dx = b / l;
	const float dy = h / l;
	for (int i = 0; i <= il; i++, x1f += dx, y1f += dy)
		*(m_pixels + static_cast<int>(x1f) + static_cast<int>(y1f) * m_width) = color;

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


void skel::Surface::Rectangle(const int x1, const int y1, const int x2, const int y2, const uint color, int strokeWidth)
{
	if (strokeWidth < 0) strokeWidth = 0;

	const int x1s = std::min(x1, x2);
	const int x2s = std::max(x1, x2);
	const int y1s = std::min(y1, y2);
	const int y2s = std::max(y1, y2);

	if (x1s >= m_width || x2s < 0 || y1s >= m_height || y2s < 0) return;

	if (x2s - x1s < strokeWidth || y2s - y1s < strokeWidth) strokeWidth = 0;

	const int x1c = std::max(x1s, 0);
	const int x2c = std::min(x2s, m_width - 1);
	const int y1c = std::max(y1s, 0);
	const int y2c = std::min(y2s, m_height - 1);


	if (strokeWidth == 0)
	{
		for (int y = y1c; y <= y2c; y++)
		{
			for (int x = x1c; x <= x2c;x++)
			{
				m_pixels[y * m_width + x] = color;
			}
		}

		m_dirty = true;
		return;
	}

	// width based

	Rectangle(x1s, y1s, x2s, y1s + strokeWidth - 1, color, 0);

	Rectangle(x1s, y2s - strokeWidth + 1, x2s, y2s, color, 0);


	Rectangle(x1s, y1s + strokeWidth - 1 , x1s + strokeWidth - 1, y2s - strokeWidth + 1, color, 0);
	Rectangle(x2s - strokeWidth + 1, y1s + strokeWidth - 1 , x2s, y2s - strokeWidth + 1, color, 0);

	m_dirty = true;
}

void skel::Surface::Rectangle(const int2& p1, const int2& p2, const uint color, const int strokeWidth)
{
	Rectangle(p1.x, p1.y, p2.x, p2.y, color, strokeWidth);
}

void skel::Surface::Circle(const int cx, const int cy, const int radius, const uint color, int strokeWidth)
{
	// ripped out from an old project. but there has to be a better/cleaner way

	if (radius == 0) { Plot(cx, cy, color); return; }
	if (radius < 0) return;
	

	if (strokeWidth > radius)
		strokeWidth = 0;

	auto drawOctants = [&](int x, int y, uint color, int centerX, int centerY)
		{
			Plot(x + centerX, y + centerY, color);
			Plot(-x + centerX, y + centerY, color);
			Plot(x + centerX, -y + centerY, color);
			Plot(-x + centerX, -y + centerY, color);

			// If the generated point is on the line x = y then 
			// the perimeter points have already been printed
			if (x != y)
			{
				Plot(y + centerX, x + centerY, color);
				Plot(-y + centerX, x + centerY, color);
				Plot(y + centerX, -x + centerY, color);
				Plot(-y + centerX, -x + centerY, color);
			}
		};

	strokeWidth = strokeWidth == 0 ? radius + 1 : strokeWidth - 1;

	int x = radius;
	int y = 0;

	// we add the thickness to the radius because we want the thickness to go outward

	// Initialising the value of P
	int p = 1 - radius;


	for (int i = 0; i <= strokeWidth; i++)
	{
		drawOctants(radius - i, 0, color, cx, cy);
	}

	while (x > y)
	{
		y++;

		// Mid-point is inside or on the perimeter
		if (p <= 0)
		{
			p = p + 2 * y + 1;
		}
		else
		{
			// Mid-point is outside the perimeter
			x--;
			p = p + 2 * y - 2 * x + 1;
		}

		// All the perimeter points have already been printed
		if (x < y)
			break;



		for (int i = 0; i <= strokeWidth; i++)
		{
			drawOctants(x - i, y, color, cx, cy);
		}


	}


	if (strokeWidth == 0 || strokeWidth > radius)
		return;

	x = radius - strokeWidth;
	y = 0;

	p = 1 - (radius - strokeWidth);

	for (int i = 0; i <= strokeWidth; i++)
	{
		drawOctants(radius - strokeWidth + i, 0, color, cx, cy);
	}

	while (x > y)
	{
		y++;

		// Mid-point is inside or on the perimeter
		if (p <= 0)
		{
			p = p + 2 * y + 1;
		}
		else
		{
			// Mid-point is outside the perimeter
			x--;
			p = p + 2 * y - 2 * x + 1;
		}

		// All the perimeter points have already been printed
		if (x < y)
			break;



		for (int i = 0; i <= strokeWidth; i++)
		{
			drawOctants(x + i, y, color, cx, cy);
			drawOctants(x, y + i, color, cx, cy);
		}
	}

	m_dirty = true;
}

void skel::Surface::Circle(const int2& center, const int radius, const uint color, const int strokeWidth)
{
	Circle(center.x, center.y, radius, color, strokeWidth);
}


void skel::Surface::UpdateGPUTexture()
{
	if (!m_dirty || m_textureID==0) return;
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, m_pixels);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
