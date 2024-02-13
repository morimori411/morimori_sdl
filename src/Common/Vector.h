#pragma once

#include <cstdint>
#include <cmath>

namespace common{
    template <typename T>
    struct Vec2{
        T m_x;
        T m_y;

        // コンストラクタ  Constructor
        Vec2()
        :m_x(0)
        ,m_y(0){
        }
        Vec2(T x, T y)
        :m_x(x)
        ,m_y(y){
        }
        // ベクトルの長さを求める  Calculate the magnitude of the vector
        double CalcMag() const {
            return std::sqrt(m_x * m_x + m_y * m_y);
        }
        // ベクトルの向きを求める  Calculate the direction of the vector
        double CalcDir() const {
            return std::atan2(m_y, m_x);
        }

        // 型変換  Typecast
        template<typename U>
        Vec2(const Vec2<U>& other)
            :m_x(other.m_x)
            ,m_y(other.m_y){
        }
        // 演算子のオーバーロード  Operator overload
        common::Vec2<T> operator+() const {return *this;}
        common::Vec2<T> operator-() const {
            common::Vec2<T> vec;
            vec.m_x = -m_x;
            vec.m_y = -m_y;
            return vec;
        }
        friend common::Vec2<T> operator+(const common::Vec2<T> vec1, const common::Vec2<T> vec2){
            common::Vec2<T> vec;
            vec.m_x = vec1.m_x + vec2.m_x;
            vec.m_y = vec1.m_y + vec2.m_y;
            return vec;
        }
        friend common::Vec2<T> operator-(const common::Vec2<T> vec1, const common::Vec2<T> vec2){
            common::Vec2<T> vec;
            vec.m_x = vec1.m_x - vec2.m_x;
            vec.m_y = vec1.m_y - vec2.m_y;
            return vec;
        }
        friend common::Vec2<T> operator*(const common::Vec2<T> vec1, const double k){
            common::Vec2<T> vec;
            vec.m_x = vec1.m_x * k;
            vec.m_y = vec1.m_y * k;
            return vec;
        }
        friend common::Vec2<T> operator/(const common::Vec2<T> vec1, const double k){
            common::Vec2<T> vec;
            vec.m_x = vec1.m_x / k;
            vec.m_y = vec1.m_y / k;
            return vec;
        }
        common::Vec2<T> operator+=(const common::Vec2<T> vec){
            m_x += vec.m_x;
            m_y += vec.m_y;
            return *this;
        }
        common::Vec2<T> operator-=(const common::Vec2<T> vec){
            m_x -= vec.m_x;
            m_y -= vec.m_y;
            return *this;
        }
        common::Vec2<T> operator*=(const T k){
            m_x *= k;
            m_y *= k;
            return *this;
        }
        common::Vec2<T> operator/=(const T k){
            m_x /= k;
            m_y /= k;
            return *this;
        }
    };
}