#pragma once

#include <cstdint>

namespace common{
    struct Color{
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    double dist(double x1, double y1, double x2, double y2);
}