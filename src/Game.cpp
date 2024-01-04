#include "Game.h"

game::Game::Game()
:m_window(nullptr)
,m_renderer(nullptr)
,m_ticks(0)
,m_frames(0)
,m_is_running(true)
{
}

bool game::Game::Initialize(){
    bool failed;

    // SDLライブラリの初期化  Initialization of SDL
    failed = SDL_Init(FLAGS_OF_SDL_INIT);
    if(failed){
        SDL_Log("In game::Game::Initialize(): SDL could not be initialized! SDL Error: %s", SDL_GetError());
        return 1;
    }
    // SDL_imageライブラリの初期化  Initialization of SDL_image
    failed = !IMG_Init(FLAGS_OF_IMG_INIT);
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
        game::GAME_TITLE.c_str(), // title
        SDL_WINDOWPOS_CENTERED, // x
        SDL_WINDOWPOS_CENTERED, // y
        game::WINDOW_WIDTH * game::RENDER_SCALE, // w
        game::WINDOW_HEIGHT * game::RENDER_SCALE, // h
        game::WINDOW_FLAGS // flags
    );
    if(!m_window){
        SDL_Log("In game::Game::Initialize(): Window could not be created! SDL Error: %s", SDL_GetError());
        return 1;
    }
    // レンダラーを生成  Create renderer
    m_renderer = SDL_CreateRenderer(
        m_window, // window
        -1, // index
        game::RENDERER_FLAGS // flags
    );
    if(!m_renderer){
        SDL_Log("In game::Game::Initialize(): Renderer could not be created! SDL Error: %s", SDL_GetError());
        return 1;
    }
    // レンダースケールを設定  Set render scale
    failed = SDL_RenderSetScale(m_renderer, game::RENDER_SCALE, game::RENDER_SCALE);
    if(failed){
        SDL_Log("In game::Game::Initialize(): Render scale could not be set! SDL Error: %s", SDL_GetError());
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
    while(!SDL_TICKS_PASSED(SDL_GetTicks64(), m_ticks + 1000 / FPS));
    m_ticks = SDL_GetTicks64();
    m_frames++;
}