#pragma once

namespace skel
{
	class Surface;


	class Renderer
	{
	public:
		Renderer(int width, int height);
		~Renderer();

		void Begin(); // Bind FBO, clear
		void End();   // Unbind FBO


		void Resize(int width, int height);


		void Clear(const float4& color);
		void BlitSurface(Surface* surface, int x, int y);



		uint32_t GetOutputTexture() const { return m_colorTexture; }

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
	private:
		void InitFramebuffer();
		void InitQuad();
		void InitShader();

	private:
		uint32_t m_FBO = 0;
		uint32_t m_colorTexture = 0;
		uint32_t m_quadVAO = 0, m_quadVBO = 0;
		uint32_t m_shader = 0;

		int m_width, m_height;
	};


}

