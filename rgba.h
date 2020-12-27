#pragma once

#include <string>

struct RgbaData
{
    float r;
    float g;
    float b;
    float a;
};


inline std::ostream& operator<<(std::ostream &o, const RgbaData& rgba)
{
    o << "RGBa(" << rgba.r << ", " << rgba.g << ", " << rgba.b << ", " << rgba.a << ')';
    return o;
}