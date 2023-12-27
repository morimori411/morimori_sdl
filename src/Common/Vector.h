#pragma once

#include <cstdint>

namespace common{
    struct Vec2{
        double m_x; // x
        double m_y; // y

        Vec2() = default; // コンストラクタ  Constructor
        Vec2(double x, double y); // コンストラクタ  Constructor
    };
}