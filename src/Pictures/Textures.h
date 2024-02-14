#pragma once

#include <math.h>
#include <string>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include "../Game.h"
#include "../Common/Vector.h"
#include "Fonts.h"

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
    const SDL_BlendMode BLEND_MODE = SDL_BLENDMODE_BLEND;

    // SDL_textureとそのサイズを管理する構造体  Structure to manage SDL_Texture and sizes
    struct Texture{
        SDL_Texture* m_texture; // SDL_Textureのオブジェクト  SDL_Texture object
        common::Vec2<int32_t> m_size; // 画像ファイルのサイズ  Size of image file

        // コンストラクタ  Constructor
        Texture(SDL_Texture* texture, common::Vec2<int32_t> size);
        // デストラクタ  Destructor
        ~Texture();
    };

    // 複数のテクスチャを管理するクラス  Class to manage multiple textures
    class Textures{
        private:
        game::Game* m_game; // ゲームオブジェクト  Game object
        // 画像ファイルのパスとそのファイルから作られたテクスチャを管理するmap  A map that manages image file paths and the textures created from them
        std::map<std::string, pictures::Texture*> m_textures;
        std::map<std::string, std::string> m_nickname_to_path; // ファイル名のニックネーム  File Nickname

        public:
        // コンストラクタ  Constructor
        Textures(game::Game* game);
        // デストラクタ  Destructor
        ~Textures();
        // ファイルパスを指定して画像を読み込む  Load an image by specifying the file path
        bool LoadFile(std::string path, std::string nickname);
        // ファイルパスを指定して読み込んだ画像を開放  Free the loaded image by specifying the file path
        bool Free(std::string path_or_nickname);
        // ファイルパスを指定してテクスチャを描画  Draw texture by specifying the file path
        bool Draw(std::string path_or_nickname, SDL_Rect* srcrct, common::Vec2<double> xy, uint8_t position, common::Vec2<double> scale, double angle_rad, SDL_RendererFlip flip, SDL_Color color);

        // ゲッター  Getter
        std::map<std::string, std::string> GetNicknameToPath() const {return m_nickname_to_path;}
        common::Vec2<int32_t> GetTextureSize(std::string path_or_nickname){
            // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
            if(m_nickname_to_path.count(path_or_nickname)){
                // ニックネームからファイルパスを取得  Get file path from nickname
                path_or_nickname = m_nickname_to_path[path_or_nickname];
            }
            // テクスチャのイテレータを取得  Get texture iterator 
            auto itr = m_textures.find(path_or_nickname);
            if(itr == m_textures.end()){
                // 読み込む  Load
                SDL_Log("Specified image was not loaded. Path: %s Now loading...", path_or_nickname.c_str());
                LoadFile(path_or_nickname, path_or_nickname);
            }
            return m_textures[path_or_nickname]->m_size;
        }
    };

    // 複数のテキストテクスチャを管理するクラス  Class to manage multiple text textures
    class TextTextures{
        private:
        game::Game* m_game; // ゲームオブジェクト  Game object
        pictures::Fonts* m_fonts;
        // フォントファイルのパス、テキスト、フォントサイズとそのフォントから作られたテクスチャを管理するmap  A map that manages font file paths, text, font size and the textures created from that font
        std::map<std::string, std::map<std::string, std::map<uint16_t, pictures::Texture*>>> m_text_textures;
        // 各テクスチャから作られたピクチャの数を管理するmap  A map that manages the number of pictures created from each texture
        std::map<std::string, std::map<std::string, std::map<uint16_t, uint32_t>>> m_num_of_pictures;

        public:
        // コンストラクタ  Constructor
        TextTextures(game::Game* game, pictures::Fonts* fonts);
        // デストラクタ  Destructor
        ~TextTextures();
        // ファイルパス、テキスト、フォントサイズを指定してフォントからテクスチャを生成する  Create a texture from a font by specifying the file path, text, and font size
        bool Create(std::string path_or_nickname, std::string text, uint16_t pt);
        // ファイルパス、テキスト、フォントサイズを指定してテクスチャを破棄
        bool Destroy(std::string path_or_nickname, std::string text, uint16_t pt);
        // ファイルパス、テキスト、フォントサイズを指定してテクスチャを描画
        bool Draw(std::string path_or_nickname, std::string text, uint16_t pt, SDL_Rect* srcrct, common::Vec2<double> xy, uint8_t position, common::Vec2<double> scale, double angle_rad, SDL_RendererFlip flip, SDL_Color color);

        // ゲッター  Getter
        pictures::Fonts* GetFonts() const {return m_fonts;}
        std::map<std::string, std::map<std::string, std::map<uint16_t, pictures::Texture*>>> GetTextTextures(){return m_text_textures;}
        common::Vec2<int32_t> GetTextureSize(std::string path_or_nickname, std::string text, uint16_t pt){
            // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
            if(m_fonts->GetNicknameToPath().count(path_or_nickname)){
                // ニックネームからファイルパスを取得  Get file path from nickname
                path_or_nickname = m_fonts->GetNicknameToPath()[path_or_nickname];
            }
            // 指定したテクスチャが存在しない場合  When Specified texture does not exist 
            if(!m_text_textures[path_or_nickname][text].count(pt)){
                // 生成する  Create
                SDL_Log("In pictures::TextTextures::GetTextureSize: Specified texture was not loaded. Path: %s Text: %s Pt: %d Now loading...", path_or_nickname.c_str(), text.c_str(), pt);
                Create(path_or_nickname, text, pt);
            }
            return m_text_textures[path_or_nickname][text][pt]->m_size;
        }
        uint32_t GetNumOfPictures(std::string path_or_nickname, std::string text, uint16_t pt){
            // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
            if(m_fonts->GetNicknameToPath().count(path_or_nickname)){
                // ニックネームからファイルパスを取得  Get file path from nickname
                path_or_nickname = m_fonts->GetNicknameToPath()[path_or_nickname];
            }
            // 指定したテクスチャが存在しない場合
            if(!m_text_textures[path_or_nickname][text].count(pt)){
                // 生成する  Create
                SDL_Log("In pictures::TextTextures::GetNumOfPictures: Specified texture was not loaded. Path: %s Text: %s Pt: %d Now loading...", path_or_nickname.c_str(), text.c_str(), pt);
                Create(path_or_nickname, text, pt);
            }
            return m_num_of_pictures[path_or_nickname][text][pt];
        }
        // セッター  Setter
        void SetNumOfPictures(std::string path_or_nickname, std::string text, uint16_t pt, uint32_t num_of_pictures){
            // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
            if(m_fonts->GetNicknameToPath().count(path_or_nickname)){
                // ニックネームからファイルパスを取得  Get file path from nickname
                path_or_nickname = m_fonts->GetNicknameToPath()[path_or_nickname];
            }
            // 指定したテクスチャが存在しない場合
            if(!m_text_textures[path_or_nickname][text].count(pt)){
                // 生成する  Create
                SDL_Log("In pictures::TextTextures::SetNumOfPictures: Specified texture was not loaded. Path: %s Text: %s Pt: %d Now loading...", path_or_nickname.c_str(), text.c_str(), pt);
                Create(path_or_nickname, text, pt);
            }
            m_num_of_pictures[path_or_nickname][text][pt] = num_of_pictures;
        }
    };
}