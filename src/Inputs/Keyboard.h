#pragma once

#include <vector>
#include <map>
#include <SDL.h>

namespace inputs{
    class Keyboard{
        private:
        std::vector<SDL_Scancode> m_scancodes; // 入力を受け取るキーの配列
        std::map<SDL_Scancode, int64_t> m_push_frames; // そのキーが押され続けているフレーム数、もしくは離され続けているフレーム数を取得する 前者は正の数で、後者は負の数で

        public:
        Keyboard(std::vector<SDL_Scancode> scancodes);
        // 毎フレーム実行 全てのキーの状況を取得する
        void AllKeyStates();
        // そのキーが押され続けているフレーム数、もしくは離され続けているフレーム数を返す 前者は正の数で、後者は負の数で
        int64_t GetPushFrames(SDL_Scancode scancode){return m_push_frames[scancode];}
    };
}
