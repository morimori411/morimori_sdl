#pragma once

#include "../Common/Vector.h"

namespace pictures{
    class Camera{
        private:
        common::Vec2<double> m_xy; // カメラの座標  Camera Coordinate
        double m_zoom; // カメラのズーム率  zoom rate of camera

        public:
        Camera();
        // アクセサ  Accessor
        common::Vec2<double> GetXY() const {return m_xy;}
        double GetZoom() const {return m_zoom;}
        void SetXY(common::Vec2<double> xy){m_xy = xy;}
        void SetZoom(double zoom){m_zoom = zoom;}
    };
}