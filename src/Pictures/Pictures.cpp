#include "Pictures.h"

pictures::Picture::Picture(game::Game* game, pictures::Textures* textures, std::string path_or_nickname, common::Vec2<double> xy)
:m_game(game)
,m_textures(textures)
,m_is_text(false)
,m_path_or_nickname(path_or_nickname)
,m_xy(xy)
,m_position(TOP_LEFT)
,m_scale(1.0, 1.0)
,m_angle_rad(0.0)
,m_flip(SDL_FLIP_NONE)
,m_num_of_segs(1, 1)
,m_is_camera_target(false)
,m_in_animation(false)
,m_start_seg(0)
,m_last_seg(0)
,m_fpf(1)
{
    m_srcrct = new SDL_Rect;
    m_srcrct->x = 0;
    m_srcrct->y = 0;
    m_srcrct->w = m_textures->GetTextureSize(m_path_or_nickname).x;
    m_srcrct->h = m_textures->GetTextureSize(m_path_or_nickname).y;
    m_color.r = 255;
    m_color.g = 255;
    m_color.b = 255;
    m_color.a = 255;
    // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
    if(m_textures->GetNicknameToPath().count(m_path_or_nickname)){
        // ニックネームからファイルパスを取得  Get file path from nickname
        m_path_or_nickname = m_textures->GetNicknameToPath()[m_path_or_nickname];
    }
}

pictures::Picture::Picture(game::Game* game, pictures::TextTextures* text_textures, std::string path_or_nickname, std::string text, uint16_t pt, common::Vec2<double> xy)
:m_game(game)
,m_text_textures(text_textures)
,m_is_text(true)
,m_path_or_nickname(path_or_nickname)
,m_text(text)
,m_pt(pt)
,m_xy(xy)
,m_position(TOP_LEFT)
,m_scale(1.0, 1.0)
,m_angle_rad(0.0)
,m_flip(SDL_FLIP_NONE)
,m_num_of_segs(1, 1)
,m_in_animation(false)
,m_start_seg(0)
,m_last_seg(0)
,m_fpf(1)
{
    m_srcrct = new SDL_Rect;
    m_srcrct->x = 0;
    m_srcrct->y = 0;
    m_srcrct->w = m_text_textures->GetTextureSize(m_path_or_nickname, m_text, m_pt).x;
    m_srcrct->h = m_text_textures->GetTextureSize(m_path_or_nickname, m_text, m_pt).y;
    m_color.r = 255;
    m_color.g = 255;
    m_color.b = 255;
    m_color.a = 255;
    // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
    if(m_text_textures->GetFonts()->GetNicknameToPath().count(m_path_or_nickname)){
        // ニックネームからファイルパスを取得  Get file path from nickname
        m_path_or_nickname = m_text_textures->GetFonts()->GetNicknameToPath()[m_path_or_nickname];
    }
}

pictures::Picture::~Picture(){
    delete m_srcrct;
}

bool pictures::Picture::Display(){
    bool failed;
    // テキストピクチャの場合
    if(m_is_text){
        failed = m_text_textures->Draw(m_path_or_nickname, m_text, m_pt, m_srcrct, m_xy, m_position, m_scale, m_angle_rad, m_flip, m_color);
        if(failed){
            SDL_Log("In pictures::Picture::Display(): Text could not be displayed! Path: %s Text: %s Pt: %d", m_path_or_nickname.c_str(), m_text.c_str(), m_pt);
            return 1;
        }
    // 画像ピクチャの場合   
    }else{
        failed = m_textures->Draw(m_path_or_nickname, m_srcrct, m_xy, m_position, m_scale, m_angle_rad, m_flip, m_color);
        if(failed){
            SDL_Log("In pictures::Picture::Display(): Picture could not be displayed! Path: %s", m_path_or_nickname.c_str());
            return 1;
        }
    }
    return 0;
}

bool pictures::Picture::Animation(){
    bool failed;
    int64_t current_game_frame = m_game->GetFrames();
    // アニメーション開始からのフレーム数  Number of frames since the start of the animation
    double frames_from_animation_start = current_game_frame - m_start_game_frame;
    // アニメーションのフレーム数  // Num of frames of animation
    double animation_frames = m_last_seg - m_start_seg;
    // アニメーション1ループにかかるフレーム数  Number of frames per animation loop
    double frames_per_loop = animation_frames * m_fpf;
    // 現在のアニメーションループの開始からのゲームのフレーム数を計算  Calculates the number of game frames since the start of the current animation loop
    frames_from_animation_start = std::fmod(frames_from_animation_start, frames_per_loop);
    // 現在のアニメーションのコマ番号  Frame number of the current animation
    int32_t current_frame_number = int32_t(frames_from_animation_start / m_fpf);
    // 分割されたフレームの座標  Coordinates of split frames
    common::Vec2 frame_xy(current_frame_number % int32_t(m_num_of_segs.x), int32_t(current_frame_number / m_num_of_segs.y));
    // srcrctを計算  Culculate the srcrct
    SDL_Rect* tmp_srcrct = m_srcrct;
    common::Vec2 xy(
        m_textures->GetTextureSize(m_path_or_nickname).x / m_num_of_segs.x * frame_xy.x,
        m_textures->GetTextureSize(m_path_or_nickname).y / m_num_of_segs.y * frame_xy.y
    );
    common::Vec2 wh(
        m_textures->GetTextureSize(m_path_or_nickname).x / m_num_of_segs.x,
        m_textures->GetTextureSize(m_path_or_nickname).y / m_num_of_segs.y
    );
    SetClipXYAndSize(xy, wh);
    // 表示  Draw
    failed = Display();
    if(failed){
        SDL_Log("In pictures::Picture::Animation(): Animation could not displayed! Path: %s", m_path_or_nickname.c_str());
        m_srcrct = tmp_srcrct;
        return 1;
    }
    m_srcrct = tmp_srcrct;
    return 0;
}

pictures::LayerNo::LayerNo(){}

pictures::LayerNo::LayerNo(int32_t layer, int32_t no)
:m_layer(layer)
,m_no(no)
{
}

pictures::Pictures::Pictures(game::Game* game, pictures::Textures* textures, pictures::TextTextures* text_textures, pictures::Camera* camera)
:m_game(game)
,m_textures(textures)
,m_text_textures(text_textures)
,m_camera(camera)
,changed(false)
{
}

pictures::Pictures::~Pictures(){
    // ピクチャをすべて破棄する  Destroy all picutures
    for(auto itr1 = m_pictures.begin(); itr1 != m_pictures.end(); itr1++){
        for(auto itr2 = itr1->second.begin(); itr2 != itr1->second.end(); itr2++){
            delete itr2->second;
        }
    }
}

bool pictures::Pictures::Add(pictures::LayerNo layer_and_no, std::string path_or_nickname, common::Vec2<double> xy){
    int32_t layer = layer_and_no.m_layer;
    int32_t no = layer_and_no.m_no;
    // そのレイヤーのピクチャ番号に割り当てられたピクチャが既に存在する場合
    if(m_pictures[layer].count(no)){
        // エラーを表示して異常終了
        SDL_Log("In pictures::Pictures::Add(): A picture already exists at the specified number!: Layer: %d : No: %d", layer, no);
        return 1;
    }
    // 新しいピクチャを作りmapに追加  Create a new picture and add to the map
    m_pictures[layer][no] = new pictures::Picture(m_game, m_textures, path_or_nickname, xy);
    return 0;
}

bool pictures::Pictures::Add(pictures::LayerNo layer_and_no, std::string path_or_nickname, std::string text, uint16_t pt, common::Vec2<double> xy){
    // ニックネームに対応するファイルパスが存在するとき  When the file path matching the nickname exists
    if(GetTextTextures()->GetFonts()->GetNicknameToPath().count(path_or_nickname)){
        // ニックネームからファイルパスを取得  Get file path from nickname
        path_or_nickname = GetTextTextures()->GetFonts()->GetNicknameToPath()[path_or_nickname];
    }
    int32_t layer = layer_and_no.m_layer;
    int32_t no = layer_and_no.m_no;
    // そのレイヤーのピクチャ番号に割り当てられたピクチャが既に存在する場合
    if(m_pictures[layer].count(no)){
        // エラーを表示して異常終了
        SDL_Log("In pictures::Pictures::Add(): A picture already exists at the specified number!: Layer: %d : No: %d", layer, no);
        return 1;
    }
    // テクスチャがまだ作られていない場合
    if(!m_text_textures->GetTextTextures()[path_or_nickname][text].count(pt)){
        // 新しいテクスチャを作成  Create new texture
        m_text_textures->Create(path_or_nickname, text, pt);
    }
    // テクスチャから作られたピクチャの数を+1  +1 to the number of pictures created from the texture
    uint32_t current_num_of_pictures = m_text_textures->GetNumOfPictures(path_or_nickname, text, pt);
    m_text_textures->SetNumOfPictures(path_or_nickname, text, pt, current_num_of_pictures + 1);
    // 新しいピクチャを作りmapに追加  Create a new picture and add to the map
    m_pictures[layer][no] = new pictures::Picture(m_game, m_text_textures, path_or_nickname, text, pt, xy);
    return 0;
}

bool pictures::Pictures::Delete(pictures::LayerNo layer_and_no){
    bool failed;
    int32_t layer = layer_and_no.m_layer;
    int32_t no = layer_and_no.m_no;
    // そのレイヤーのピクチャ番号に割り当てられたピクチャが存在しない場合
    if(!m_pictures[layer].count(no)){
        // エラーを表示して終了
        SDL_Log("In pictures::Pictures::Delete(): A picture does not exists at the specified number!: Layer: %d : No: %d", layer, no);
        return 0;
    }
    // テキストの場合  if text
    if(m_pictures[layer][no]->GetIsText()){
        std::string path_or_nickname = m_pictures[layer][no]->GetPathOrNickname();
        std::string text = m_pictures[layer][no]->GetText();
        uint16_t pt = m_pictures[layer][no]->GetPt();
        // テクスチャから作られたピクチャの数を-1  -1 to the number of pictures created from the texture
        uint32_t current_num_of_pictures = m_text_textures->GetNumOfPictures(path_or_nickname, text, pt);
        m_text_textures->SetNumOfPictures(path_or_nickname, text, pt, current_num_of_pictures - 1);
        // テクスチャから作られたピクチャの数が0になった場合  When the number of pictures created from a texture becomes 0
        if(current_num_of_pictures - 1 == 0){
            // テクスチャを破棄する  Destroy texture
            failed = m_text_textures->Destroy(path_or_nickname, text, pt);
            if(failed){
                SDL_Log("In pictures::Pictures::Delete(): Text texture could not be destroyed! Path: %s Text: %s Pt: %d", path_or_nickname.c_str(), text.c_str(), pt);
                return 1;
            }
        }
    }
    // ピクチャを破棄する  Destroy picture
    delete m_pictures[layer][no];
    m_pictures[layer].erase(no);
    return 0;
}

bool pictures::Pictures::DeleteAll(int32_t layer){
    int32_t num_pic = m_pictures[layer].size();
    for(int i = 0; i < num_pic; i++){
        Delete({layer, m_pictures[layer].begin()->first});
    }
    return 0;
}

void pictures::Pictures::StartAnimation(pictures::LayerNo layer_and_no){
    int32_t layer = layer_and_no.m_layer;
    int32_t no = layer_and_no.m_no;
    m_pictures[layer][no]->SetInAnimation(true);
    m_pictures[layer][no]->SetStartGameFrame(m_game->GetFrames());
}

void pictures::Pictures::StopAnimation(pictures::LayerNo layer_and_no){
    int32_t layer = layer_and_no.m_layer;
    int32_t no = layer_and_no.m_no;
    m_pictures[layer][no]->SetInAnimation(false);
}

bool pictures::Pictures::DisplayAll(){
    bool failed;
    SDL_RenderClear(m_game->GetRenderer());
    // 全てのピクチャを表示  Display all pictures
    for(auto itr1 = m_pictures.begin(); itr1 != m_pictures.end(); itr1++){
        for(auto itr2 = itr1->second.begin(); itr2 != itr1->second.end(); itr2++){
            pictures::Picture* picture = itr2->second;
            common::Vec2 old_xy = picture->GetXY();
            common::Vec2 old_scale = picture->GetScale();
            // カメラに応じて表示座標とサイズを調整  Adjust display coordinates and size according to camera
            if(picture->GetIsCameraTarget()){
                picture->SetXY((picture->GetXY() - m_camera->GetXY()) * m_camera->GetZoom());
                picture->SetScale(picture->GetScale() * m_camera->GetZoom());
            }
            // アニメーション中の時  When in animation
            if(itr2->second->GetInAnimation()){
                failed = itr2->second->Animation();
                if(failed){
                    SDL_Log("In pictures::Pictures::DisplayAll(): Picture could not be animated!");
                    return 1;
                }
            // アニメーション中でない時  When not in animation
            }else{
                failed = itr2->second->Display();
                if(failed){
                    SDL_Log("In pictures::Pictures::DisplayAll(): Pictures could not be displayed!");
                    return 1;
                }
            }
            // カメラに応じて調整した表示座標とサイズを元に戻す  Restore display coordinates and size adjusted according to camera
            if(picture->GetIsCameraTarget()){
                picture->SetXY(old_xy);
                picture->SetScale(old_scale);
            }
        }
    }
    SDL_RenderPresent(m_game->GetRenderer());
    return 0;
}
