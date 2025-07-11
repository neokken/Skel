#pragma once


namespace skel
{
	

inline skel::float3 HSVtoRGB(float h, float s, float v)
{
    skel::float3 rgb;

    if (s == 0.0f)
    {
        // Achromatic (gray)
        rgb = { v, v, v };
        return rgb;
    }

    h = fmodf(h, 1.0f) * 6.0f; // [0, 6)
    int i = static_cast<int>(floorf(h));
    float f = h - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0: rgb = { v, t, p }; break;
    case 1: rgb = { q, v, p }; break;
    case 2: rgb = { p, v, t }; break;
    case 3: rgb = { p, q, v }; break;
    case 4: rgb = { t, p, v }; break;
    case 5: rgb = { v, p, q }; break;
    default: rgb = { 0, 0, 0 }; break; // Fallback
    }

    return rgb;
}


inline skel::float3 HSVtoRGB(const skel::float3& hsv)
{
    return HSVtoRGB(hsv.x, hsv.y, hsv.z);
}


inline skel::float3 RGBtoHSV(float r, float g, float b)
{

    float max = std::max({ r, g, b });
    float min = std::min({ r, g, b });
    float delta = max - min;

    skel::float3 hsv;
    hsv.z = max;

    if (max == 0.0f)
    {
        hsv.y = 0.0f;
        hsv.x = 0.0f; // Undefined hue
        return hsv;
    }

    hsv.y = delta / max;

    if (delta == 0.0f)
    {
        hsv.x = 0.0f;
    }
    else
    {
        if (max == r)
            hsv.x = fmodf((g - b) / delta, 6.0f);
        else if (max == g)
            hsv.x = ((b - r) / delta) + 2.0f;
        else // max == b
            hsv.x = ((r - g) / delta) + 4.0f;

        hsv.x /= 6.0f;
        if (hsv.x < 0.0f)
            hsv.x += 1.0f;
    }

    return hsv;
}


inline skel::float3 RGBtoHSV(const skel::float3& rgb)
{
    return RGBtoHSV(rgb.x, rgb.y, rgb.z);
}


inline uint ColorToUint32(const skel::float4& rgba)
{
    const uint a = static_cast<uint>(lround(rgba.w * 255.0f));
    const uint r = static_cast<uint>(lround(rgba.x * 255.0f));
    const uint g = static_cast<uint>(lround(rgba.y * 255.0f));
    const uint b = static_cast<uint>(lround(rgba.z * 255.0f));

    return  (a << 24) | (r << 16) | (g << 8) | b;
}

inline float4 Uint32ToColor(const uint argb)
{
    float a = (argb >> 24 & 0xFF) / 255.0f;
    float r = (argb >> 16 & 0xFF) / 255.0f;
    float g = (argb >> 8 & 0xFF) / 255.0f;
    float b = (argb & 0xFF) / 255.0f;

    return { r, g, b, a };
}


}
