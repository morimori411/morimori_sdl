#pragma once

#include <math.h>
#include <string>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include "../Game.h"
#include "../Common/Vector.h"

// ピクチャに関する名前空間  Namespace for pictures
namespace pictures{
    // ピクチャの表示を合わせる位置 テンキーに対応  Position to align the picture display. Numeric keypad support.
    enum Position{
        BOTTOM_LEFT = 1,
        BOTTOM,
        BOTTOM_RIGHT,
        LEFT,
        CENTER,
        RIGHT,
        TOP_LEFT,
        TOP,
        TOP_RIGHT
    };

    // 定数  Constants
    const SDL_BlendMode BLEND_MODE = SDL_BLENDMODE_BLEND; // ゲーム名  Name of game

    // SDL_textureとそのサイズを管理する構造体  Structure to manage SDL_Texture and sizes
    struct Texture{
        SDL_Texture* m_texture; // SDL_Textureのオブジェクト  SDL_Texture object
        common::Vec2 m_size; // 画像ファイルのサイズ  Size of image file

        // コンストラクタ  Constructor
        Texture(SDL_Texture* texture, common::Vec2 size);
        // デストラクタ  Destructor
        ~Texture();
    };

    // 複数のテクスチャを管理するクラス  Class to manage multiple textures
    class Textures{
        private:
        game::Game* m_game; // ゲームオブジェクト  Game object
        std::map<std::string, pictures::Texture*> m_textures; // 画像ファイルのパスとそのファイルから作られたテクスチャを管理するmap  A map that manages image file paths and the textures created from them

        public:
        Textures(game::Game* game); // コンストラクタ  Constructor
        ~Textures(); // デストラクタ  Destructor
        bool LoadFile(std::string path); // ファイルパスを指定して画像を読み込む  Load an image by specifying the file path
        bool Free(std::string path); // ファイルパスを指定して読み込んだ画像を開放  Free the loaded image by specifying the file path
        bool Draw(std::string path, SDL_Rect* srcrct, common::Vec2 xy, uint8_t position, common::Vec2 scale, double angle_rad, SDL_RendererFlip flip, SDL_Color color); // ファイルパスを指定してテクスチャを描画  Draw texture by specifying the file path

        // ゲッター  Getter
        common::Vec2 GetTextureSize(std::string path){
            // テクスチャのイテレータを取得  Get texture iterator 
            auto itr = m_textures.find(path);
            if(itr == m_textures.end()){
                // 読み込む  Load
                SDL_Log("Specified image was not loaded. Path: %s Now loading...", path.c_str());
                LoadFile(path);
            }
            return m_textures[path]->m_size;
        }
    };
}