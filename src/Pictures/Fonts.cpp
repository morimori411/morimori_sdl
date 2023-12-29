#include "Fonts.h"

pictures::Fonts::Fonts(){}

pictures::Fonts::~Fonts(){
    // 読み込んだフォントをすべて破棄する  Destroy all loaded fonts
    for(auto itr = m_fonts.begin(); itr != m_fonts.end(); itr++){
        TTF_CloseFont(itr->second);
    }
}

bool pictures::Fonts::LoadFont(std::string path){
    // フォントのイテレータを取得  Get font iterator
    auto itr = m_fonts.find(path);
    // フォントが存在しているとき  When the font already exist
    if(itr != m_fonts.end()){
        SDL_Log("In pictures::Fonts::Loafile(): The font is already loaded! Path: %s", path.c_str());
        return 0;
    }
    // 指定したパスからフォントを読み込む  Load a font by specifying the file path
    m_fonts[path] = TTF_OpenFont(path.c_str(), 0);
    if(m_fonts[path] == nullptr){
        SDL_Log("In pictures::Fonts::Loafile(): Font could not be loaded! Path: %s SDL Error: %s", path.c_str(), SDL_GetError());
        return 1;
    }
    return 0;
}

bool pictures::Fonts::Free(std::string path){
    // フォントのイテレータを取得  Get font iterator
    auto itr = m_fonts.find(path);
    // フォントが存在していなかったとき  When the font does not exist
    if(itr == m_fonts.end()){
        SDL_Log("In pictures::Fonts::Free(): The font you tried to free does not exist! Path: %s", path.c_str());
        return 1;
    }
    // フォントを破棄  Delete font
    TTF_CloseFont(m_fonts[path]);
    // mapから削除  Delete from map
    m_fonts.erase(path);
    return 0;
}