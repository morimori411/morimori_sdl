#pragma once

#include <vector>
#include <map>
#include <SDL.h>
#include "../Common/Vector.h"

namespace inputs{
    class Mouse{
        private:
        std::vector<int64_t> m_push_frames; // そのキーが押され続けているフレーム数、もしくは離され続けているフレーム数を取得する 前者は正の数で、後者は負の数で
        common::Vec2<int32_t> m_mouse_coord; // マウスカーソルの座標(ウィンドウ左上基準)
        common::Vec2<int32_t> m_wheel; // マウスホイール

        public:
        Mouse();
        // 毎フレーム実行 全てのマウスの状況を取得する
        void AllMouseStates(common::Vec2<int32_t> wheel);
        // そのボタンが押され続けているフレーム数、もしくは離され続けているフレーム数を返す 前者は正の数で、後者は負の数で
        int64_t GetPushFrames(int32_t sdl_button){return m_push_frames[sdl_button];}
        // マウスの座標を取得
        common::Vec2<int32_t> GetMouseCoord(){return m_mouse_coord;}
        // マウスホイールの状態を取得
        common::Vec2<int32_t> GetWheel(){return m_wheel;}
    };
}
