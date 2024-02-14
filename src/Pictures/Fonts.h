#pragma once

#include <string>
#include <map>
#include <SDL_ttf.h>

namespace pictures{

    // 複数のフォントを管理するクラス  Class to manage multiple fonts
    class Fonts{
        private:
        std::map<std::string, TTF_Font*> m_fonts; // フォントファイルのパスと読み込まれたフォントオブジェクトを管理するmap  A map that manages font file paths and loaded font objects
        std::map<std::string, std::string> m_nickname_to_path; // ファイル名のニックネーム  File Nickname

        public:
        // コンストラクタ  Constructor
        Fonts();
        // デストラクタ  Destructor
        ~Fonts();
        // ファイルパスを指定してフォントを読み込む  Load a font by specifying the file path
        bool LoadFont(std::string path, std::string nickname);
        // ファイルパスを指定して読み込んだフォントを開放  Free the loaded font by specifying the file path
        bool Free(std::string path_or_nickname);

        // ゲッター  Getter
        std::map<std::string, TTF_Font*> GetFonts() const {return m_fonts;}
        std::map<std::string, std::string> GetNicknameToPath() const {return m_nickname_to_path;}
        TTF_Font* GetFont(std::string path_or_nickname){
            // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
            if(m_nickname_to_path.count(path_or_nickname)){
                // ニックネームからファイルパスを取得  Get file path from nickname
                path_or_nickname = m_nickname_to_path[path_or_nickname];
            }
            return m_fonts[path_or_nickname];
        }
        // セッター  Setter
        bool SetPt(std::string path_or_nickname, uint16_t pt){
            // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
            if(m_nickname_to_path.count(path_or_nickname)){
                // ニックネームからファイルパスを取得  Get file path from nickname
                path_or_nickname = m_nickname_to_path[path_or_nickname];
            }
            int failed;
            failed = TTF_SetFontSize(m_fonts[path_or_nickname], pt);
            if(failed == -1){
                SDL_Log("In pictures::Fonts::GetFont(): Font size could not be set! Path: %s Pt: %d", path_or_nickname.c_str(), pt);
                return 1;
            }
            return 0;
        }
    };
}