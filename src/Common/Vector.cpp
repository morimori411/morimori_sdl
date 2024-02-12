#include "Vector.h"

common::Vec2::Vec2()
:m_x(0)
,m_y(0)
{
}

common::Vec2::Vec2(double x, double y)
:m_x(x)
,m_y(y)
{
}

double common::Vec2::CalcMag(){
    return std::sqrt(m_x * m_x + m_y * m_y);
}

double common::Vec2::CalcDir(){
    return std::atan2(m_y, m_x);
}