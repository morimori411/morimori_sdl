#include "Main.h"

int main(int argc, char* argv[]){
    game::Game* game = new game::Game;
    game->Initialize();
    pictures::Textures* textures = new pictures::Textures(game);
    pictures::Fonts* fonts = new pictures::Fonts;
    pictures::TextTextures* text_textures = new pictures::TextTextures(game, fonts);
    pictures::Pictures* pictures = new pictures::Pictures(game, textures, text_textures);

    // ピクチャ表示
    textures->LoadFile("data/images/aoi.jpg");
    textures->LoadFile("data/images/allow2.png");
    fonts->LoadFont("data/fonts/MPLUS1Code-Regular.ttf");
    std::vector<pictures::LayerAndNo> allows(9);
    for(int i = 0; i < 9; i++){
        allows[i].m_layer = pictures::FRONT;
        allows[i].m_no = i + 10;
        pictures->Add(allows[i], "data/images/allow2.png", {90, 90});
        pictures->SetPosition(allows[i], i + 1);
    }
    pictures::LayerAndNo aoi1(pictures::BACK, 0);
    pictures::LayerAndNo aoi2(pictures::FRONT, 0);
    pictures::LayerAndNo text1(pictures::FRONT, 1);
    pictures::LayerAndNo text2(pictures::FRONT, 2);
    pictures->Add(aoi1, "data/images/aoi.jpg", {100, 100});
    pictures->Add(aoi2, "data/images/aoi.jpg", {200, 200});
    pictures->Add(text1, "data/fonts/MPLUS1Code-Regular.ttf", "Hello World!", 40, {300, 300});
    pictures->Add(text2, "data/fonts/MPLUS1Code-Regular.ttf", "Hello World!", 20, {400, 400});
    pictures->Delete(allows[0]);
    pictures->SetPosition(aoi2, pictures::BOTTOM);
    pictures->SetRGBA(aoi2, 128, 128, 255, 255);
    pictures->SetRGBA(text1, 0, 255, 255, 255);
    pictures->SetScale(aoi1, {0.2, 0.2});
    pictures->SetScale(aoi2, {0.5, 0.5});
    pictures->SetAnimation(aoi2, {5, 5}, 0, 5 * 5, 2);
    pictures->StartAnimation(aoi2);
    // メインループ  Main loop
    SDL_Event event;
    while(game->GetIsRunning()){
        // SDL_Event
        while(SDL_PollEvent(&event)){
            // ゲーム終了  End of game
            if(event.type == SDL_QUIT) game->SetIsRunning(false);
        }

        pictures->DisplayAll();
        game->Wait();
    }

    // シャットダウン  Shutdown
    game->Shutdown();
    delete game;
    delete textures;
    delete fonts;
    delete text_textures;
    delete pictures;
    return 0;
}