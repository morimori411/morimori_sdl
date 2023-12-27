#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

namespace game{
    // 定数  Constants
    const std::string GAME_TITLE = ""; // ゲーム名  Name of game
    const int16_t WINDOW_WIDTH = 540; // ウィンドウの横幅  Width of window
    const int16_t WINDOW_HEIGHT = 540; // ウィンドウの縦幅  Height of window
    const uint32_t FLAGS_OF_SDL_INIT = SDL_INIT_EVERYTHING; // SDLライブラリの初期化フラグ  Flags for SDL initialization
    const uint32_t FLAGS_OF_IMG_INIT = IMG_INIT_PNG | IMG_INIT_JPG; // SDL_imageライブラリの初期化フラグ  Flags for SDL_image initialization
    const uint32_t WINDOW_FLAGS = 0; // ウィンドウ設定のフラグ  Flags for window settings
    const uint32_t RENDERER_FLAGS = 0; // レンダラー設定のフラグ  Flags for renderer settings
    const uint8_t RENDER_SCALE = 1; // レンダースケール  The render scale
    const uint16_t FPS = 60; // fps

    // ゲーム全体を管理するクラス  Class for managing games in general
    class Game{
        private:
        SDL_Window* m_window; // ウィンドウオブジェクト  Window object
        SDL_Renderer* m_renderer; // レンダラーオブジェクト  Renderer object
        uint64_t m_ticks; // ゲーム開始から現在フレームの開始までのミリ秒  Milliseconds from game start to start of current frame
        uint64_t m_frames; // 現在フレームまでのフレーム数  Frames from game start to present
        bool m_is_running; // ゲームが起動中かどうか falseに設定することでゲーム終了  Whether the game is running. Set to false to exit the main loop.

        public:
        Game(); // コンストラクタ  Constructor
        bool Initialize(); // 初期化  Initialization
        void Shutdown(); // 終了  Shutdown
        void Wait(); // 次のフレームまで待機  Wait for next frame;

        // ゲッター  Getter
        SDL_Window* GetWindow() const {return m_window;}
        SDL_Renderer* GetRenderer() const {return m_renderer;}
        uint64_t GetTicks() const {return m_ticks;}
        uint64_t GetFrames() const {return m_frames;}
        bool GetIsRunning() const {return m_is_running;}
        // セッター  Setter
        void SetTicks(uint64_t ticks){m_ticks = ticks;}
        void SetFrames(uint64_t frames){m_frames = frames;}
        void SetIsRunning(bool is_running){m_is_running = is_running;}
    };
}