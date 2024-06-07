#pragma once

#include <cstdint>
#include <cmath>

const double M_2PI = M_PI * 2;

namespace common{
    struct Color{
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    double dist(double x1, double y1, double x2, double y2);
}
