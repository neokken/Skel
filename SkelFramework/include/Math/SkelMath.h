// Based on Tmpl8Math at: https://github.com/jbikker/tmpl8

#pragma once

namespace skel
{
	

//struct int2;
//struct int3;
//struct int4;
//
//struct uint2;
//struct uint3;
//struct uint4;
//
//struct float2;
//struct float3;
//struct float4;


struct alignas(8) int2
{
    constexpr int2() : x(0), y(0) {}
    constexpr int2(const int x, const int y) : x(x), y(y) {}
    constexpr explicit int2(const int a) : x(a), y(a) {}

    int& operator [] (const int i) { return cell[i]; }
    const int& operator[](const int i) const { return cell[i]; }

	int2& operator+=(const int2& rhs);
    int2& operator-=(const int2& rhs);
    int2& operator*=(int rhs);
    int2& operator/=(int rhs);

    union { struct { int x, y; }; int cell[2]; };
};

struct alignas(8) uint2
{
    constexpr uint2() : x(0u), y(0u) {}
    constexpr uint2(const uint x, const uint y) : x(x), y(y) {}
    constexpr explicit uint2(const uint a) : x(a), y(a) {}

    uint& operator [] (const int i) { return cell[i]; }
    const uint& operator[](const int i) const { return cell[i]; }

    uint2& operator+=(const uint2& rhs);
    uint2& operator-=(const uint2& rhs);
    uint2& operator*=(uint rhs);
    uint2& operator/=(uint rhs);

    union { struct { uint x, y; }; uint cell[2]; };
};

struct alignas(8) float2 {
    constexpr float2() : x(0.f), y(0.f) {}
    constexpr float2(const float x, const float y) : x(x), y(y) {}
    constexpr explicit float2(const float a) : x(a), y(a) {}

    float& operator[](const int i) { return cell[i]; }
    const float& operator[](const int i) const { return cell[i]; }
    float2& operator+=(const float2& rhs);
    float2& operator-=(const float2& rhs);
    float2& operator*=(float rhs);
    float2& operator/=(float rhs);

    union { struct { float x, y; }; float cell[2]; };
};


struct alignas(16) int3
{
    constexpr int3() : x(0), y(0), z(0) {}
    constexpr int3(const int x, const int y, const int z) : x(x), y(y), z(z), dummy(0) {}
    constexpr explicit int3(const int a) : x(a), y(a), z(a), dummy(0) {}

    int& operator[] (const int i) { return cell[i]; }
    const int& operator[] (const int i) const { return cell[i]; }
    int3& operator+=(const int3& rhs);
    int3& operator-=(const int3& rhs);
    int3& operator*=(int rhs);
    int3& operator/=(int rhs);

    union { struct { int x, y, z; int dummy; }; int cell[4]; };
};

struct alignas(16) uint3
{
    constexpr uint3() : x(0u), y(0u), z(0u) {}
    constexpr uint3(const uint x, const uint y, const uint z) : x(x), y(y), z(z), dummy(0) {}
    constexpr explicit uint3(const uint a) : x(a), y(a), z(a), dummy(0) {}

    uint& operator[] (const int i) { return cell[i]; }
    const uint& operator[] (const int i) const { return cell[i]; }
    uint3& operator+=(const uint3& rhs);
    uint3& operator-=(const uint3& rhs);
    uint3& operator*=(uint rhs);
    uint3& operator/=(uint rhs);

    union { struct { uint x, y, z; uint dummy; }; uint cell[4]; };
};

struct alignas(16) float3
{
    constexpr float3() : x(0.f), y(0.f), z(0.f) {}
    constexpr float3(const float x, const float y, const float z) : x(x), y(y), z(z), dummy(0) {}
    constexpr explicit float3(const float a) : x(a), y(a), z(a), dummy(0) {}

    float& operator[](const int i) { return cell[i]; }
    const float& operator[](const int i) const { return cell[i]; }
    float3& operator+=(const float3& rhs);
    float3& operator-=(const float3& rhs);
    float3& operator*=(float rhs);
    float3& operator/=(float rhs);

    union { struct { float x, y, z, dummy; }; float cell[4]; };
};


struct alignas(16) int4
{
    constexpr int4() : x(0), y(0), z(0) , w(0) {}
    constexpr int4(const int x, const int y, const int z, const int w) : x(x), y(y), z(z), w(w) {}
    constexpr explicit int4(const int a) : x(a), y(a), z(a), w(a) {}
    constexpr explicit int4(const int3& a, const int w) : x(a.x), y(a.y), z(a.z), w(w) {}

    int& operator[] (const int i) { return cell[i]; }
    const int& operator[] (const int i) const { return cell[i]; }
    int4& operator+=(const int4& rhs);
    int4& operator-=(const int4& rhs);
    int4& operator*=(int rhs);
    int4& operator/=(int rhs);

    union { struct { int x, y, z, w; }; int cell[4]; };
};

struct alignas(16) uint4
{
    constexpr uint4() : x(0u), y(0u), z(0u), w(0u) {}
    constexpr uint4(const uint x, const uint y, const uint z, const uint w) : x(x), y(y), z(z), w(w) {}
    constexpr explicit uint4(const uint a) : x(a), y(a), z(a), w(a) {}
    constexpr explicit uint4(const uint3& a, const uint w) : x(a.x), y(a.y), z(a.z), w(w) {}

    uint& operator [] (const int i) { return cell[i]; }
    const uint& operator[] (const int i) const { return cell[i]; }
    uint4& operator+=(const uint4& rhs);
    uint4& operator-=(const uint4& rhs);
    uint4& operator*=(uint rhs);
    uint4& operator/=(uint rhs);

    union { struct { uint x, y, z, w; }; uint cell[4]; };
};

struct alignas(16) float4
{
    constexpr float4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
    constexpr float4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
    constexpr explicit float4(const float a) : x(a), y(a), z(a), w(a) {}
    constexpr explicit float4(const float3& a, const float w) : x(a.x), y(a.y), z(a.z), w(w) {}

    float& operator[] (const int i) { return cell[i]; }
    const float& operator[] (const int i) const { return cell[i]; }
    float4& operator+=(const float4& rhs);
    float4& operator-=(const float4& rhs);
    float4& operator*=(float rhs);
    float4& operator/=(float rhs);

    union { struct { float x, y, z, w; }; float cell[4]; };
};



// Operator Overloads for Arithmetic

inline int2 operator+(const int2& a, const int2& b) { return { a.x + b.x, a.y + b.y }; }
inline int2 operator-(const int2& a, const int2& b) { return { a.x - b.x, a.y - b.y }; }
inline int2 operator*(const int2& a, const int b)   { return { a.x * b, a.y*b }; }
inline int2 operator*(const int a, const int2& b) { return { b.x * a, b.y * a }; }
inline int2 operator/(const int2& a, const int b)   { return { a.x / b, a.y / b }; }
inline int2 operator-(const int2& v) { return { -v.x, -v.y }; }
inline int2& int2::operator+=(const int2& rhs) { x += rhs.x; y += rhs.y; return *this;}
inline int2& int2::operator-=(const int2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
inline int2& int2::operator*=(const int rhs) { x *= rhs; y *= rhs; return *this; }
inline int2& int2::operator/=(const int rhs) { x /= rhs; y /= rhs; return *this; }

inline uint2 operator+(const uint2& a, const uint2& b) { return { a.x + b.x, a.y + b.y }; }
inline uint2 operator-(const uint2& a, const uint2& b) { return { a.x - b.x, a.y - b.y }; }
inline uint2 operator*(const uint2& a, const uint b) { return { a.x * b, a.y * b }; }
inline uint2 operator*(const uint a, const uint2& b) { return { b.x * a, b.y * a }; }
inline uint2 operator/(const uint2& a, const uint b) { return { a.x / b, a.y / b }; }
inline uint2& uint2::operator+=(const uint2& rhs) { x += rhs.x; y += rhs.y; return *this; }
inline uint2& uint2::operator-=(const uint2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
inline uint2& uint2::operator*=(const uint rhs) { x *= rhs; y *= rhs; return *this; }
inline uint2& uint2::operator/=(const uint rhs) { x /= rhs; y /= rhs; return *this; }

inline float2 operator+(const float2& a, const float2& b) { return { a.x + b.x, a.y + b.y }; }
inline float2 operator-(const float2& a, const float2& b) { return { a.x - b.x, a.y - b.y }; }
inline float2 operator*(const float2& a, const float b) { return { a.x * b, a.y * b }; }
inline float2 operator*(const float a, const float2& b) { return { b.x * a, b.y * a }; }
inline float2 operator/(const float2& a, const float b) { return { a.x / b, a.y / b }; }
inline float2 operator-(const float2& v) { return { -v.x, -v.y }; }
inline float2& float2::operator+=(const float2& rhs) { x += rhs.x; y += rhs.y; return *this; }
inline float2& float2::operator-=(const float2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
inline float2& float2::operator*=(const float rhs) { x *= rhs; y *= rhs; return *this; }
inline float2& float2::operator/=(const float rhs) { x /= rhs; y /= rhs; return *this; }

inline int3 operator+(const int3& a, const int3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z}; }
inline int3 operator-(const int3& a, const int3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z}; }
inline int3 operator*(const int3& a, const int b) { return { a.x * b, a.y * b, a.z * b }; }
inline int3 operator*(const int a, const int3& b) { return { b.x * a, b.y * a, b.z * a }; }
inline int3 operator/(const int3& a, const int b) { return { a.x / b, a.y / b, a.z / b }; }
inline int3 operator-(const int3& v) { return { -v.x, -v.y , -v.z}; }
inline int3& int3::operator+=(const int3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
inline int3& int3::operator-=(const int3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
inline int3& int3::operator*=(const int rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
inline int3& int3::operator/=(const int rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }

inline uint3 operator+(const uint3& a, const uint3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
inline uint3 operator-(const uint3& a, const uint3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
inline uint3 operator*(const uint3& a, const uint b) { return { a.x * b, a.y * b, a.z * b }; }
inline uint3 operator*(const uint a, const uint3& b) { return { b.x * a, b.y * a, b.z * a }; }
inline uint3 operator/(const uint3& a, const uint b) { return { a.x / b, a.y / b, a.z / b }; }
inline uint3& uint3::operator+=(const uint3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
inline uint3& uint3::operator-=(const uint3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
inline uint3& uint3::operator*=(const uint rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
inline uint3& uint3::operator/=(const uint rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }

inline float3 operator+(const float3& a, const float3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
inline float3 operator-(const float3& a, const float3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
inline float3 operator*(const float3& a, const float b) { return { a.x * b, a.y * b, a.z * b }; }
inline float3 operator*(const float a, const float3& b) { return { b.x * a, b.y * a, b.z * a }; }
inline float3 operator/(const float3& a, const float b) { return { a.x / b, a.y / b, a.z / b }; }
inline float3 operator-(const float3& v) { return { -v.x, -v.y , -v.z }; }
inline float3& float3::operator+=(const float3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
inline float3& float3::operator-=(const float3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
inline float3& float3::operator*=(const float rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
inline float3& float3::operator/=(const float rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }

inline int4 operator+(const int4& a, const int4& b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
inline int4 operator-(const int4& a, const int4& b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
inline int4 operator*(const int4& a, const int b) { return { a.x * b, a.y * b, a.z * b, a.w * b  }; }
inline int4 operator*(const int a, const int4& b) { return { b.x * a, b.y * a, b.z * a, b.w * a }; }
inline int4 operator/(const int4& a, const int b) { return { a.x / b, a.y / b, a.z / b, a.w / b  }; }
inline int4 operator-(const int4& v) { return { -v.x, -v.y , -v.z, -v.w }; }
inline int4& int4::operator+=(const int4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
inline int4& int4::operator-=(const int4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this;}
inline int4& int4::operator*=(const int rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
inline int4& int4::operator/=(const int rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }

inline uint4 operator+(const uint4& a, const uint4& b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
inline uint4 operator-(const uint4& a, const uint4& b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
inline uint4 operator*(const uint4& a, const uint b) { return { a.x * b, a.y * b, a.z * b, a.w * b }; }
inline uint4 operator*(const uint a, const uint4& b) { return { b.x * a, b.y * a, b.z * a, b.w * a }; }
inline uint4 operator/(const uint4& a, const uint b) { return { a.x / b, a.y / b, a.z / b, a.w / b }; }
inline uint4& uint4::operator+=(const uint4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
inline uint4& uint4::operator-=(const uint4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
inline uint4& uint4::operator*=(const uint rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
inline uint4& uint4::operator/=(const uint rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }

inline float4 operator+(const float4& a, const float4& b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
inline float4 operator-(const float4& a, const float4& b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
inline float4 operator*(const float4& a, const float b) { return { a.x * b, a.y * b, a.z * b, a.w * b }; }
inline float4 operator*(const float a, const float4& b) { return { b.x * a, b.y * a, b.z * a, b.w * a }; }
inline float4 operator/(const float4& a, const float b) { return { a.x / b, a.y / b, a.z / b, a.w / b }; }
inline float4 operator-(const float4& v) { return { -v.x, -v.y , -v.z, -v.w }; }
inline float4& float4::operator+=(const float4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
inline float4& float4::operator-=(const float4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
inline float4& float4::operator*=(const float rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
inline float4& float4::operator/=(const float rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }


// Basic Math Functions (inline helpers)

// float only yet

inline float dot(const float2& a, const float2& b) { return a.x * b.x + a.y * b.y; }
inline float dot(const float3& a, const float3& b) { return a.x * b.x + a.y * b.y + a.z * b.z;}
inline float dot(const float4& a, const float4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

inline float3 cross(const float3& a, const float3& b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }

inline float lengthSq(const float2& v) { return dot(v, v); }
inline float lengthSq(const float3& v) { return dot(v, v); }
inline float lengthSq(const float4& v) { return dot(v, v); }

inline float length(const float2& v) { return sqrt(dot(v, v)); }
inline float length(const float3& v) { return sqrt(dot(v, v)); }
inline float length(const float4& v) { return sqrt(dot(v, v)); }

inline float2 normalize(const float2& v) {return v / length(v);}
inline float3 normalize(const float3& v) {return v / length(v); }
inline float4 normalize(const float4& v) {return v / length(v); }

inline float2 safeNormalize(const float2& v, const float epsilon = 1e-6f) { const float len = length(v); return (len < epsilon) ? float2(0.f) : (v / len);}
inline float3 safeNormalize(const float3& v, const float epsilon = 1e-6f) { const float len = length(v); return (len < epsilon) ? float3(0.f) : (v / len); }
inline float4 safeNormalize(const float4& v, const float epsilon = 1e-6f) { const float len = length(v); return (len < epsilon) ? float4(0.f) : (v / len) ; }

inline float distance(const float2& a, const float2& b) { return length(b - a); }
inline float distance(const float3& a, const float3& b) { return length(b - a); }
inline float distance(const float4& a, const float4& b) { return length(b - a); }

inline float distanceSq(const float2& a, const float2& b) { return lengthSq(b - a); }
inline float distanceSq(const float3& a, const float3& b) { return lengthSq(b - a); }
inline float distanceSq(const float4& a, const float4& b) { return lengthSq(b - a); }

inline float2 lerp(const float2& a, const float2& b, const float t) { return a * (1.f - t) + b * t; }
inline float3 lerp(const float3& a, const float3& b, const float t) { return a * (1.f - t) + b * t; }
inline float4 lerp(const float4& a, const float4& b, const float t) { return a * (1.f - t) + b * t; }


// casting to other types

inline int2 ToInt2(const uint2& value) { return { static_cast<int>(value.x),static_cast<int>(value.y) }; }
inline int2 ToInt2(const float2& value) { return { static_cast<int>(value.x),static_cast<int>(value.y) }; }
inline int2 ToInt2Floor(const float2& value) { return { static_cast<int>(std::floor(value.x)),static_cast<int>(std::floor(value.y)) }; }
inline int2 ToInt2Ceil(const float2& value) { return { static_cast<int>(std::ceil(value.x)),static_cast<int>(std::ceil(value.y)) }; }
inline int2 ToInt2Round(const float2& value) { return { static_cast<int>(std::round(value.x)),static_cast<int>(std::round(value.y)) }; }

inline uint2 ToUInt2(const int2& value ) { return { static_cast<uint>(value.x),static_cast<uint>(value.y) }; }
inline uint2 ToUInt2(const float2& value ) { return { static_cast<uint>(value.x),static_cast<uint>(value.y) }; }
inline uint2 ToUInt2Floor(const float2& value ) { return { static_cast<uint>(std::floor(value.x)),static_cast<uint>(std::floor(value.y)) }; }
inline uint2 ToUInt2Ceil(const float2& value ) { return { static_cast<uint>(std::ceil(value.x)),static_cast<uint>(std::ceil(value.y)) }; }
inline uint2 ToUInt2Round(const float2& value ) { return { static_cast<uint>(std::round(value.x)),static_cast<uint>(std::round(value.y)) }; }

inline float2 ToFloat2(const int2& value) { return { static_cast<float>(value.x),static_cast<float>(value.y) }; }
inline float2 ToFloat2(const uint2& value) { return { static_cast<float>(value.x),static_cast<float>(value.y) }; }


}
