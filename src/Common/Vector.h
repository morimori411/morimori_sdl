#pragma once

#include <cstdint>
#include <cmath>

namespace common{
    struct Vec2{
        double m_x;
        double m_y;

        // コンストラクタ  Constructor
        Vec2();
        Vec2(double x, double y);
        // ベクトルの長さを求める  Calculate the magnitude of the vector
        double CalcMag();
        // ベクトルの向きを求める  Calculate the direction of the vector
        double CalcDir();

        // 演算子のオーバーロード
        common::Vec2 operator+() const {return *this;}
        common::Vec2 operator-() const {
            common::Vec2 vec;
            vec.m_x = -m_x;
            vec.m_y = -m_y;
            return vec;
        }
        friend common::Vec2 operator+(const common::Vec2 vec1, const common::Vec2 vec2){
            common::Vec2 vec;
            vec.m_x = vec1.m_x + vec2.m_x;
            vec.m_y = vec1.m_y + vec2.m_y;
            return vec;
        }
        friend common::Vec2 operator-(const common::Vec2 vec1, const common::Vec2 vec2){
            common::Vec2 vec;
            vec.m_x = vec1.m_x - vec2.m_x;
            vec.m_y = vec1.m_y - vec2.m_y;
            return vec;
        }
        friend common::Vec2 operator*(const common::Vec2 vec1, const double k){
            common::Vec2 vec;
            vec.m_x = vec1.m_x * k;
            vec.m_y = vec1.m_y * k;
            return vec;
        }
        friend common::Vec2 operator/(const common::Vec2 vec1, const double k){
            common::Vec2 vec;
            vec.m_x = vec1.m_x / k;
            vec.m_y = vec1.m_y / k;
            return vec;
        }
        common::Vec2 operator+=(const common::Vec2 vec){
            m_x += vec.m_x;
            m_y += vec.m_y;
            return *this;
        }
        common::Vec2 operator-=(const common::Vec2 vec){
            m_x -= vec.m_x;
            m_y -= vec.m_y;
            return *this;
        }
        common::Vec2 operator*=(const double k){
            m_x *= k;
            m_y *= k;
            return *this;
        }
        common::Vec2 operator/=(const double k){
            m_x /= k;
            m_y /= k;
            return *this;
        }
    };
}