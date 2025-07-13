#pragma once

namespace skel
{


class Surface
{
public:
    Surface(int width, int height, bool needGPUTexture);
    Surface(const std::string& file, bool needGPUTexture);
    ~Surface();

    Surface(const Surface& other) = delete;
    Surface& operator=(const Surface& other) = delete;



    // rendering

    void Clear(uint color);

    void Plot(int x, int y, uint color);
    void Plot(const int2& p, uint color);

    void Line(int x1, int y1, int x2, int y2, uint color);
    void Line(const int2& p1, const int2& p2, uint color);

    void CopyTo(int x, int y, Surface* d) const;
    void CopyTo(const int2& p, Surface* d) const;

    void Box(int x1, int y1, int x2, int y2, uint color);   // only draws outline of box
    void Box(const int2& p1, const int2& p2, uint color);   // only draws outline of box

    void Bar(int x1, int y1, int x2, int y2, uint color);
    void Bar(const int2& p1, const int2& p2, uint color);

    void UpdateGPUTexture();
    uint32_t GetTextureID();


    // when editing values with it will not be marked as dirty so you need to manually mark it.
    uint32_t* GetBuffer() { return m_pixels; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    void MarkAsDirty() { m_dirty = true; }
private:
    void InitTexture();

private:
    uint32_t* m_pixels = nullptr;
    int m_width = 0, m_height = 0;

    uint32_t m_textureID = 0;
    bool m_dirty = true;
};


}
