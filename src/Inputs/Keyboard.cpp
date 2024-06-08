#include "Keyboard.h"

inputs::Keyboard::Keyboard(std::vector<SDL_Scancode> scancodes)
:m_scancodes(scancodes){}

void inputs::Keyboard::AllKeyStates(){
    const uint8_t* keyboard_state = SDL_GetKeyboardState(nullptr);
    for(auto scancode : m_scancodes){
        // キーが押されているなら
        if(keyboard_state[scancode]){
            // 現時点で負の数が格納されている(つまり直前フレームまで離されていた)なら
            if(m_push_frames[scancode] < 0){
                // 1を代入
                m_push_frames[scancode] = 1;
            // 現時点で正の数が格納されている(つまり直前フレームまで押されていた)なら
            }else{
                // 1増やす
                m_push_frames[scancode] += 1;
            }
        // キーが押されていないなら
        }else{
            // 現時点で負の数が格納されている(つまり直前フレームまで離されていた)なら
            if(m_push_frames[scancode] < 0){
                // 1を減らす
                m_push_frames[scancode] -= 1;
            // 現時点で正の数が格納されている(つまり直前フレームまで押されていた)なら
            }else{
                // -1を代入
                m_push_frames[scancode] = -1;
            }
        }
    }
}
