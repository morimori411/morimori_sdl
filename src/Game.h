#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Common/Vector.h"

namespace game{
    // ゲーム全体を管理するクラス  Class for managing games in general
    class Game{
        private:
        std::string m_game_title; // ゲーム名
        SDL_Window* m_window; // ウィンドウオブジェクト  Window object
        SDL_Renderer* m_renderer; // レンダラーオブジェクト  Renderer object
        uint32_t m_flags_sdl_init; // SDLライブラリの初期化フラグ  Flags for SDL initialization
        uint32_t m_flags_img_init; // SDL_imageライブラリの初期化フラグ  Flags for SDL_image initialization
        common::Vec2<int32_t> m_window_size_xy; // ウィンドウの大きさ
        uint32_t m_window_flags; // ウィンドウ設定のフラグ  Flags for window settings
        uint32_t m_renderer_flags; // レンダラー設定のフラグ  Flags for renderer settings
        uint16_t m_fps;
        uint64_t m_ticks; // ゲーム開始から現在フレームの開始までのミリ秒  Milliseconds from game start to start of current frame
        uint64_t m_frames; // 現在フレームまでのフレーム数  Frames from game start to present
        bool m_is_running; // ゲームが起動中かどうか falseに設定することでゲーム終了  Whether the game is running. Set to false to exit the main loop.

        public:
        Game(std::string game_title, uint32_t flags_sdl_init, uint32_t flags_img_init, common::Vec2<uint32_t> window_size_xy, uint32_t window_flags, uint32_t renderer_flags, uint16_t fps); // コンストラクタ  Constructor
        bool Initialize(); // 初期化  Initialization
        void Shutdown(); // 終了  Shutdown
        void Wait(); // 次のフレームまで待機  Wait for next frame;

        // ゲッター  Getter
        SDL_Window* GetWindow() const {return m_window;}
        SDL_Renderer* GetRenderer() const {return m_renderer;}
        uint64_t GetTicks() const {return m_ticks;}
        uint64_t GetFrames() const {return m_frames;}
        common::Vec2<int32_t> GetWindowSizeXY() const {return m_window_size_xy;}
        bool GetIsRunning() const {return m_is_running;}
        // セッター  Setter
        void SetTicks(uint64_t ticks){m_ticks = ticks;}
        void SetFrames(uint64_t frames){m_frames = frames;}
        void SetIsRunning(bool is_running){m_is_running = is_running;}
    };
}
