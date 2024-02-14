#include "Textures.h"

pictures::Texture::Texture(SDL_Texture* texture, common::Vec2<int32_t> size)
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

bool pictures::Textures::LoadFile(std::string path, std::string nickname){
    // テクスチャが存在しているとき  When the texture already exist
    if(m_textures.count(path)){
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
    // 既に指定したニックネームが存在していた場合  When specified nickname already exists
    if(m_nickname_to_path.count(nickname)){
        // 警告を出して終了  End with warning
        SDL_Log("In pictures::Textures::Loadfile(): Specified nickname already exists! Nickname: %s", nickname.c_str());
        return 0;
    }
    // ニックネームを追加  Add nickname
    m_nickname_to_path[nickname] = path;
    return 0;
}

bool pictures::Textures::Free(std::string path_or_nickname){
    // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
    if(m_nickname_to_path.count(path_or_nickname)){
        std::string tmp_nickname = path_or_nickname;
        // ニックネームからファイルパスを取得  Get file path from nickname
        path_or_nickname = m_nickname_to_path[path_or_nickname];
        // ニックネームを削除  Delete nickname;
        m_nickname_to_path.erase(tmp_nickname);
    }
    // テクスチャが存在していなかったとき  When the texture does not exist
    if(!m_textures.count(path_or_nickname)){
        SDL_Log("In pictures::Textures::Free(): The texture you tried to free does not exist! Path: %s", path_or_nickname.c_str());
        return 1;
    }
    // テクスチャを破棄  Destroy texture
    delete m_textures[path_or_nickname];
    // mapから削除  Delete from map
    m_textures.erase(path_or_nickname);
    return 0;
}

bool pictures::Textures::Draw(std::string path_or_nickname, SDL_Rect* srcrct, common::Vec2<double> xy, uint8_t position, common::Vec2<double> scale, double angle_rad, SDL_RendererFlip flip, SDL_Color color){
    // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
    if(m_nickname_to_path.count(path_or_nickname)){
        // ニックネームからファイルパスを取得  Get file path from nickname
        path_or_nickname = m_nickname_to_path[path_or_nickname];
    }
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
    // 指定した画像のテクスチャが存在しなかったとき  When the texture of the specified image does not exist
    if(!m_textures.count(path_or_nickname)){
        // 読み込む  Load
        SDL_Log("In pictures::Textures::Draw(): Specified image was not loaded. Path: %s Now loading...", path_or_nickname.c_str());
        failed = LoadFile(path_or_nickname, path_or_nickname);
        if(failed) return 1;
    }
    // 角度を弧度法から度数法に変換  Convert angles from radian to degree
    double angle = angle_rad / M_PI * 180.0;
    // 色調補正  Color modulation
    SDL_SetTextureColorMod(m_textures[path_or_nickname]->m_texture, color.r, color.g, color.b);
    SDL_SetRenderDrawBlendMode(m_game->GetRenderer(), BLEND_MODE);
    SDL_SetTextureAlphaMod(m_textures[path_or_nickname]->m_texture, color.a);
    // 描画  Draw
    failed = SDL_RenderCopyEx(m_game->GetRenderer(), m_textures[path_or_nickname]->m_texture, srcrct, &dstrct, angle, nullptr, flip);
    if(failed){
        SDL_Log("In pictures::Textures::Draw(): Texture could not be drawn! Path: %s", path_or_nickname.c_str());
        return 1;
    }
    return 0;
}

pictures::TextTextures::TextTextures(game::Game* game, pictures::Fonts* fonts)
:m_game(game)
,m_fonts(fonts)
{
}

pictures::TextTextures::~TextTextures(){
    // 生成したテクスチャをすべて破棄する  Destroy all created textures
    for(auto itr1 = m_text_textures.begin(); itr1 != m_text_textures.end(); itr1++){
        for(auto itr2 = itr1->second.begin(); itr2 != itr1->second.end(); itr2++){
            for(auto itr3 = itr2->second.begin(); itr3 != itr2->second.end(); itr3++){
                delete itr3->second;
            }
        }
    }
}

bool pictures::TextTextures::Create(std::string path_or_nickname, std::string text, uint16_t pt){
    // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
    if(m_fonts->GetNicknameToPath().count(path_or_nickname)){
        // ニックネームからファイルパスを取得  Get file path from nickname
        path_or_nickname = m_fonts->GetNicknameToPath()[path_or_nickname];
    }
    bool failed;
    // 指定したフォントが存在しなかったとき  When the specified font does not exist
    if(!m_fonts->GetFonts().count(path_or_nickname)){
        // 読み込む  Load
        SDL_Log("In pictures::TextTextures::Create(): Specified font was not loaded. Path: %s Now loading...", path_or_nickname.c_str());
        failed = m_fonts->LoadFont(path_or_nickname, path_or_nickname);
        if(failed) return 1;
    }
    // テクスチャが存在しているとき  When the texture already exist
    if(m_text_textures[path_or_nickname][text].count(pt)){
        SDL_Log("In pictures::TextTextures::Create(): The texture is already loaded! Path: %s Text: %s Pt: %d", path_or_nickname.c_str(), text.c_str(), pt);
        return 0;
    }
    // フォントサイズの変更  Set font size
    failed = m_fonts->SetPt(path_or_nickname, pt);
    if(failed){
        SDL_Log("In pictures::TextTextures::Create: Font size could not be set! Path: %s Text: %s Pt: %d", path_or_nickname.c_str(), text.c_str(), pt);
        return 1;
    }
    // 指定したサーフェスを生成  Create a specified surface
    SDL_Color fg = {255, 255, 255, 255};
    SDL_Surface* loaded_surface = TTF_RenderUTF8_Blended_Wrapped(m_fonts->GetFont(path_or_nickname), text.c_str(), fg, 0);
    if(loaded_surface == nullptr){
        SDL_Log("In pictures::TextTextures::Create(): Texture could not be created! Path: %s Text: %s Pt: %d SDL Error: %s", path_or_nickname.c_str(), text.c_str(), pt, SDL_GetError());
        return 1;
    }
    // サーフェスからテクスチャを生成  Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_game->GetRenderer(), loaded_surface);
    if(texture == nullptr){
        SDL_Log("In pictures::TextTextures::Create(): Texture could not be created! Path: %s Text: %s Pt: %d SDL Error: %s", path_or_nickname.c_str(), text.c_str(), pt, SDL_GetError());
        return 1;
    }
    // テクスチャのサイズを取得  Get texture size
    int32_t size_int_x, size_int_y;
    SDL_QueryTexture(texture, nullptr, nullptr, &size_int_x, &size_int_y);
    // mapにテクスチャを追加  Add texture to map
    m_text_textures[path_or_nickname][text][pt] = new Texture(texture, common::Vec2(size_int_x, size_int_y));
    // テクスチャから作られたピクチャの数を0にリセット
    m_num_of_pictures[path_or_nickname][text][pt] = 0;
    // 読み込んだサーフェスを開放  Free the surface loaded
    SDL_FreeSurface(loaded_surface);
    return 0;
}

bool pictures::TextTextures::Destroy(std::string path_or_nickname, std::string text, uint16_t pt){
    // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
    if(m_fonts->GetNicknameToPath().count(path_or_nickname)){
        // ニックネームからファイルパスを取得  Get file path from nickname
        path_or_nickname = m_fonts->GetNicknameToPath()[path_or_nickname];
    }
    // テクスチャが存在していなかったとき  When the texture does not exist
    if(!m_text_textures[path_or_nickname][text].count(pt)){
        SDL_Log("In pictures::TextTextures::Destroy(): The texture you tried to free does not exist! Path: %s Text: %s Pt: %d", path_or_nickname.c_str(), text.c_str(), pt);
        return 1;
    }
    // テクスチャを破棄  Destroy texture
    delete m_text_textures[path_or_nickname][text][pt];
    // mapから削除  Delete from map
    m_text_textures[path_or_nickname][text].erase(pt);
    return 0;
}

bool pictures::TextTextures::Draw(std::string path_or_nickname, std::string text, uint16_t pt, SDL_Rect* srcrct, common::Vec2<double> xy, uint8_t position, common::Vec2<double> scale, double angle_rad, SDL_RendererFlip flip, SDL_Color color){
    // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
    if(m_fonts->GetNicknameToPath().count(path_or_nickname)){
        // ニックネームからファイルパスを取得  Get file path from nickname
        path_or_nickname = m_fonts->GetNicknameToPath()[path_or_nickname];
    }
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
    // 指定した画像のテクスチャが存在しなかったとき  When the texture of the specified image does not exist
    if(!m_text_textures[path_or_nickname][text].count(pt)){
        // 読み込む  Load
        SDL_Log("In pictures::TextTextures::Draw(): Specified texture was not created. Path: %s Text: %s Pt: %d Now creating...", path_or_nickname.c_str(), text.c_str(), pt);
        failed = Create(path_or_nickname, text, pt);
        if(failed) return 1;
    }
    // 角度を弧度法から度数法に変換  Convert angles from radian to degree
    double angle = angle_rad / M_PI * 180.0;
    // 色調補正  Color modulation
    SDL_SetTextureColorMod(m_text_textures[path_or_nickname][text][pt]->m_texture, color.r, color.g, color.b);
    SDL_SetRenderDrawBlendMode(m_game->GetRenderer(), BLEND_MODE);
    SDL_SetTextureAlphaMod(m_text_textures[path_or_nickname][text][pt]->m_texture, color.a);
    // 描画  Draw
    failed = SDL_RenderCopyEx(m_game->GetRenderer(), m_text_textures[path_or_nickname][text][pt]->m_texture, srcrct, &dstrct, angle, nullptr, flip);
    if(failed){
        SDL_Log("In pictures::TextTextures::Draw(): Texture could not be drawn! Path: %s Text: %s Pt: %d", path_or_nickname.c_str(), text.c_str(), pt);
        return 1;
    }
    return 0;
}