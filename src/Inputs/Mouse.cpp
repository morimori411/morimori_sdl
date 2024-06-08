#include "Mouse.h"

inputs::Mouse::Mouse()
:m_push_frames({0, 0, 0, 0}){}

void inputs::Mouse::AllMouseStates(common::Vec2<int32_t> wheel){
    m_wheel = wheel;
    uint32_t mouse_state = SDL_GetMouseState(&m_mouse_coord.x, &m_mouse_coord.y);
    for(int i = 1; i <= 3; i++){
        // キーが押されているなら
        if(mouse_state & SDL_BUTTON(i)){
            // 現時点で負の数が格納されている(つまり直前フレームまで離されていた)なら
            if(m_push_frames[i] < 0){
                // 1を代入
                m_push_frames[i] = 1;
            // 現時点で正の数が格納されている(つまり直前フレームまで押されていた)なら
            }else{
                // 1増やす
                m_push_frames[i] += 1;
            }
        // キーが押されていないなら
        }else{
            // 現時点で負の数が格納されている(つまり直前フレームまで離されていた)なら
            if(m_push_frames[i] < 0){
                // 1を減らす
                m_push_frames[i] -= 1;
            // 現時点で正の数が格納されている(つまり直前フレームまで押されていた)なら
            }else{
                // -1を代入
                m_push_frames[i] = -1;
            }
        }
    }
}
