#pragma once

#include <cstdint>
#include <cmath>
#include "Common.h"

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
        // xとyを反対にしたベクトルを返す  Return Vec2 which switched x and y
        Vec2<T> Switch() const {
            return Vec2<T>(y, x);
        }
        // dir分回転させたベクトルを返す
        Vec2<T> Rotate(double dir) const {
            double rotated_dir = std::fmod((CalcDir() + dir) + M_2PI, M_2PI);
            return Vec2<T>(CalcMag() * std::cos(rotated_dir), CalcMag() * std::sin(rotated_dir));
        }
        Vec2<double> Round() const {
            return Vec2<double>(std::round(x), std::round(y));
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
        friend bool operator<(const common::Vec2<T> vec1, const common::Vec2<T> vec2){
            if(vec1.x == vec2.x){
                return vec1.y < vec2.y;
            }else{
                return vec1.x < vec2.x;
            }
        }
        friend bool operator>(const common::Vec2<T> vec1, const common::Vec2<T> vec2){return vec2 < vec1;}
        friend bool operator<=(const common::Vec2<T> vec1, const common::Vec2<T> vec2){return !(vec1 > vec2);}
        friend bool operator>=(const common::Vec2<T> vec1, const common::Vec2<T> vec2){return !(vec1 < vec2);}
        friend bool operator==(const common::Vec2<T> vec1, const common::Vec2<T> vec2){return vec1 <= vec2 && vec1 >= vec2;}
        friend bool operator!=(const common::Vec2<T> vec1, const common::Vec2<T> vec2){return !(vec1 == vec2);}
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

    Vec2<double> MagAndDir(double mag, double dir);
}
