#include "Game.h"

game::Game::Game(std::string title, uint32_t flags_sdl_init, uint32_t flags_img_init, common::Vec2<uint32_t> window_size_xy, uint32_t window_flags, uint32_t renderer_flags, uint16_t fps)
:m_game_title(title)
,m_flags_sdl_init(flags_sdl_init)
,m_flags_img_init(flags_img_init)
,m_window_size_xy(window_size_xy)
,m_window_flags(window_flags)
,m_renderer_flags(renderer_flags)
,m_fps(fps)
,m_window(nullptr)
,m_renderer(nullptr)
,m_ticks(0)
,m_frames(0)
,m_is_running(true)
{
}

bool game::Game::Initialize(){
    bool failed;

    // SDLライブラリの初期化  Initialization of SDL
    failed = SDL_Init(m_flags_sdl_init);
    if(failed){
        SDL_Log("In game::Game::Initialize(): SDL could not be initialized! SDL Error: %s", SDL_GetError());
        return 1;
    }
    // SDL_imageライブラリの初期化  Initialization of SDL_image
    failed = !IMG_Init(m_flags_img_init);
    if(failed){
        SDL_Log("In game::Game::Initialize(): SDL_image could not be initialized! SDL Error: %s", SDL_GetError());
        return 1;
    }
    // SDL_ttfライブラリの初期化  Initialization of SDL_ttf
    failed = TTF_Init();
    if(failed){
        SDL_Log("In game::Game::Initialize(): SDL_ttf could not be initialized! SDL Error: %s", SDL_GetError());
        return 1;
    }
    // ウィンドウを生成  Create window
    m_window = SDL_CreateWindow(
        m_game_title.c_str(), // title
        SDL_WINDOWPOS_CENTERED, // x
        SDL_WINDOWPOS_CENTERED, // y
        m_window_size_xy.x, // w
        m_window_size_xy.y, // h
        m_window_flags // flags
    );
    if(!m_window){
        SDL_Log("In game::Game::Initialize(): Window could not be created! SDL Error: %s", SDL_GetError());
        return 1;
    }
    SDL_GetWindowSize(m_window, &m_window_size_xy.x, &m_window_size_xy.y);
    // レンダラーを生成  Create renderer
    m_renderer = SDL_CreateRenderer(
        m_window, // window
        -1, // index
        m_renderer_flags // flags
    );
    if(!m_renderer){
        SDL_Log("In game::Game::Initialize(): Renderer could not be created! SDL Error: %s", SDL_GetError());
        return 1;
    }

    return 0;
}

void game::Game::Shutdown(){
    // レンダラーを破棄  Destroy the renderer
    SDL_DestroyRenderer(m_renderer);
    // ウィンドウを破棄  Destroy the window
    SDL_DestroyWindow(m_window);
    // ライブラリを終了  Shutdown libraries
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    // 終了メッセージ  Shutdown message;
    SDL_Log("In game::Game::Shutdown(): Game was completely shutdowned.");
}

void game::Game::Wait(){
    // Standby for game fps
    while(!SDL_TICKS_PASSED(SDL_GetTicks64(), m_ticks + 1000 / m_fps));
    m_ticks = SDL_GetTicks64();
    m_frames++;
}
