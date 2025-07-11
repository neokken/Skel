#pragma once

namespace skel
{


class Surface
{
public:
    Surface(int32_t width, int32_t height, bool needGPUTexture=true);
    ~Surface();

    Surface(const Surface& other) = delete;
    Surface& operator=(const Surface& other) = delete;



    // rendering

    void Clear(uint color);

    void Plot(int x, int y, uint32_t color);




    void UpdateGPUTexture();
    uint32_t GetTextureID();


    // when editing values with it will not be marked as dirty so you need to manually mark it.
    uint32_t* GetBuffer() { return m_pixels; }
    uint32_t GetWidth() const { return m_width; }
    uint32_t GetHeight() const { return m_height; }

    void MarkAsDirty() { m_dirty = true; }
private:
    void InitTexture();

private:
    uint32_t* m_pixels = nullptr;
    int32_t m_width = 0, m_height = 0;

    uint32_t m_textureID = 0;
    bool m_dirty = true;
};


}
