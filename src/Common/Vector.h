#pragma once

#include <cstdint>
#include <cmath>

namespace common{
    template <typename T>
    struct Vec2{
        T x;
        T y;

        // コンストラクタ  Constructor
        Vec2()
        :x(0)
        ,y(0){
        }
        Vec2(T x, T y)
        :x(x)
        ,y(y){
        }
        // ベクトルの長さを求める  Calculate the magnitude of the vector
        double CalcMag() const {
            return std::sqrt(x * x + y * y);
        }
        // ベクトルの向きを求める  Calculate the direction of the vector
        double CalcDir() const {
            return std::atan2(y, x);
        }

        // 型変換  Typecast
        template<typename U>
        Vec2(const Vec2<U>& other)
            :x(other.x)
            ,y(other.y){
        }
        // 演算子のオーバーロード  Operator overload
        common::Vec2<T> operator+() const {return *this;}
        common::Vec2<T> operator-() const {
            common::Vec2<T> vec;
            vec.x = -x;
            vec.y = -y;
            return vec;
        }
        friend common::Vec2<T> operator+(const common::Vec2<T> vec1, const common::Vec2<T> vec2){
            common::Vec2<T> vec;
            vec.x = vec1.x + vec2.x;
            vec.y = vec1.y + vec2.y;
            return vec;
        }
        friend common::Vec2<T> operator-(const common::Vec2<T> vec1, const common::Vec2<T> vec2){
            common::Vec2<T> vec;
            vec.x = vec1.x - vec2.x;
            vec.y = vec1.y - vec2.y;
            return vec;
        }
        friend common::Vec2<T> operator*(const common::Vec2<T> vec1, const double k){
            common::Vec2<T> vec;
            vec.x = vec1.x * k;
            vec.y = vec1.y * k;
            return vec;
        }
        friend common::Vec2<T> operator/(const common::Vec2<T> vec1, const double k){
            common::Vec2<T> vec;
            vec.x = vec1.x / k;
            vec.y = vec1.y / k;
            return vec;
        }
        common::Vec2<T> operator+=(const common::Vec2<T> vec){
            x += vec.x;
            y += vec.y;
            return *this;
        }
        common::Vec2<T> operator-=(const common::Vec2<T> vec){
            x -= vec.x;
            y -= vec.y;
            return *this;
        }
        common::Vec2<T> operator*=(const T k){
            x *= k;
            y *= k;
            return *this;
        }
        common::Vec2<T> operator/=(const T k){
            x /= k;
            y /= k;
            return *this;
        }
    };
}