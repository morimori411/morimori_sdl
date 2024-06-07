#include "Vector.h"

common::Vec2<double> common::MagAndDir(double mag, double dir){
    return common::Vec2<double>(mag * std::cos(dir), mag * std::sin(dir));
}
