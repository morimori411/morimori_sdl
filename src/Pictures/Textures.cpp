#include "Textures.h"

pictures::Texture::Texture(SDL_Texture* texture, common::Vec2 size)
:m_texture(texture)
,m_size(size)
{
}

pictures::Texture::~Texture(){
    SDL_DestroyTexture(m_texture);
}

pictures::Textures::Textures(game::Game* game)
:m_game(game)
{
}

pictures::Textures::~Textures(){
    // 読み込んだテクスチャをすべて破棄する  Destroy all loaded textures
    for(auto itr = m_textures.begin(); itr != m_textures.end(); itr++){
        delete itr->second;
    }
}

bool pictures::Textures::LoadFile(std::string path){
    // テクスチャのイテレータを取得  Get texture iterator
    auto itr = m_textures.find(path);
    // テクスチャが存在しているとき  When the texture already exist
    if(itr != m_textures.end()){
        SDL_Log("In pictures::Textures::Loafile(): The texture is already loaded! Path: %s", path.c_str());
        return 0;
    }
    // 指定したパスから画像を読み込む  Load an image by specifying the file path
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    if(loaded_surface == nullptr){
        SDL_Log("In pictures::Textures::Loafile(): Image could not be loaded! Path: %s SDL Error: %s", path.c_str(), SDL_GetError());
        return 1;
    }
    // サーフェスからテクスチャを生成  Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_game->GetRenderer(), loaded_surface);
    if(texture == nullptr){
        SDL_Log("In pictures::Textures::Loafile(): Texture could not be created! Path: %s SDL Error: %s", path.c_str(), SDL_GetError());
        return 1;
    }
    // テクスチャのサイズを取得  Get texture size
    int32_t size_int_x, size_int_y;
    SDL_QueryTexture(texture, nullptr, nullptr, &size_int_x, &size_int_y);
    // mapにテクスチャを追加  Add texture to map
    m_textures[path] = new Texture(texture, common::Vec2(size_int_x, size_int_y));
    // 読み込んだサーフェスを開放  Free the surface loaded
    SDL_FreeSurface(loaded_surface);
    return 0;
}

bool pictures::Textures::Free(std::string path){
    // テクスチャのイテレータを取得  Get texture iterator
    auto itr = m_textures.find(path);
    // テクスチャが存在していなかったとき  When the texture does not exist
    if(itr == m_textures.end()){
        SDL_Log("In pictures::Textures::Free(): The texture you tried to free does not exist! Path: %s", path.c_str());
        return 1;
    }
    // テクスチャを破棄  Destroy texture
    delete m_textures[path];
    // mapを削除  Delete from map
    m_textures.erase(path);
    return 0;
}

bool pictures::Textures::Draw(std::string path, SDL_Rect* srcrct, common::Vec2 xy, uint8_t position, common::Vec2 scale, double angle_rad, SDL_RendererFlip flip, SDL_Color color){
    SDL_Rect dstrct; // SDL_RenderCopyEx dstrct
    dstrct.x = xy.m_x;
    dstrct.y = xy.m_y;
    dstrct.w = srcrct->w * scale.m_x;
    dstrct.h = srcrct->h * scale.m_y;
    // ピクチャの表示位置を合わせる  Align the display position of the picture
    switch(position){
        case BOTTOM_LEFT: dstrct.y -= dstrct.h; break;
        case BOTTOM: dstrct.x -= dstrct.w / 2; dstrct.y -= dstrct.h; break;
        case BOTTOM_RIGHT: dstrct.x -= dstrct.w; dstrct.y -= dstrct.h; break;
        case LEFT: dstrct.y -= dstrct.h / 2; break;
        case CENTER: dstrct.x -= dstrct.w / 2; dstrct.y -= dstrct.h / 2; break;
        case RIGHT: dstrct.x -= dstrct.w; dstrct.y -= dstrct.h / 2; break;
        case TOP_LEFT: break;
        case TOP: dstrct.x -= dstrct.w / 2; break;
        case TOP_RIGHT: dstrct.x -= dstrct.w; break;
    }
    bool failed = false;
    // テクスチャのイテレータを取得  Get texture iterator
    auto itr = m_textures.find(path);
    // 指定した画像のテクスチャが存在しなかったとき  When the texture of the specified image does not exist
    if(itr == m_textures.end()){
        // 読み込む  Load
        SDL_Log("In pictures::Textures::Draw(): Specified image was not loaded. Path: %s Now loading...", path.c_str());
        failed = LoadFile(path);
        if(failed) return 1;
    }
    // 角度を弧度法から度数法に変換  Convert angles from radian to degree
    double angle = angle_rad / M_PI * 180.0;
    // 色調補正  Color modulation
    SDL_SetTextureColorMod(m_textures[path]->m_texture, color.r, color.g, color.b);
    SDL_SetRenderDrawBlendMode(m_game->GetRenderer(), BLEND_MODE);
    SDL_SetTextureAlphaMod(m_textures[path]->m_texture, color.a);
    // 描画  Draw
    failed = SDL_RenderCopyEx(m_game->GetRenderer(), m_textures[path]->m_texture, srcrct, &dstrct, angle, nullptr, flip);
    if(failed){
        SDL_Log("In pictures::Textures::Draw(): Texture could not be drawn! Path: %s", path.c_str());
        return 1;
    }
    return 0;
}