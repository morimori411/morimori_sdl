#pragma once

#include "../Common/Vector.h"

namespace pictures{
    class Camera{
        private:
        common::Vec2<int32_t> m_xy; // カメラの座標  Camera Coordinate
        double m_zoom; // カメラのズーム率  zoom rate of camera

        public:
        Camera();
        // アクセサ  Accessor
        common::Vec2<int32_t> GetXY() const {return m_xy;}
        double GetZoom() const {return m_zoom;}
        void SetXY(common::Vec2<int32_t> xy){m_xy = xy;}
        void SetZoom(double zoom){m_zoom = zoom;}
    };
}
