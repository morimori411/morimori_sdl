#pragma once

#include <string>
#include <map>
#include <SDL_ttf.h>

namespace pictures{

    // 複数のフォントを管理するクラス  Class to manage multiple fonts
    class Fonts{
        private:
        std::map<std::string, TTF_Font*> m_fonts; // フォントファイルのパスと読み込まれたフォントオブジェクトを管理するmap  A map that manages font file paths and loaded font objects

        public:
        // コンストラクタ  Constructor
        Fonts();
        // デストラクタ  Destructor
        ~Fonts();
        // ファイルパスを指定してフォントを読み込む  Load a font by specifying the file path
        bool LoadFont(std::string path);
        // ファイルパスを指定して読み込んだフォントを開放  Free the loaded font by specifying the file path
        bool Free(std::string path);

        // ゲッター  Getter
        std::map<std::string, TTF_Font*> GetFonts() const {return m_fonts;}
        TTF_Font* GetFont(std::string path) {return m_fonts[path];}
        // セッター  Setter
        bool SetPt(std::string path, uint16_t pt){
            int failed;
            failed = TTF_SetFontSize(m_fonts[path], pt);
            if(failed == -1){
                SDL_Log("In pictures::Fonts::GetFont(): Font size could not be set! Path: %s Pt: %d", path.c_str(), pt);
                return 1;
            }
            return 0;
        }
    };
}