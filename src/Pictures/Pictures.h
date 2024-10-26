#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "../Game.h"
#include "../Common/Vector.h"
#include "Fonts.h"
#include "Textures.h"
#include "Camera.h"

namespace pictures{
    class Picture{
        private:
        game::Game *m_game;
        pictures::Textures* m_textures;
        pictures::TextTextures* m_text_textures;
        bool m_is_text; // ピクチャが画像かテキストか  Whether the picture is an image or text
        std::string m_path_or_nickname; // 画像ファイルのパスまたはニックネーム  Image file path or nickname
        std::string m_text; // (テキストのみ)表示テキスト  // (Only text) Display text
        uint16_t m_pt; // (テキストのみ)テキストのサイズ  // (Only text) Text size
        SDL_Rect* m_srcrct; // SDL_RenderCopy source rect
        common::Vec2<int32_t> m_xy; // ピクチャの座標  Picture coordinates
        uint8_t m_position; // ピクチャの表示を合わせる位置  Position to align the picture display
        common::Vec2<double> m_scale; // 表示スケール  Display scale
        double m_angle_rad; // ピクチャの回転角度[rad]  Picture rotation angle[rad]
        SDL_RendererFlip m_flip; // ピクチャの縦横反転  Flip picture horizontally and vertically
        SDL_Color m_color; // 色調補正  Color modulation
        bool m_is_camera_target; // Whether the camera control is affected or not
        bool m_in_animation; // アニメーション中かどうか  Whether animation is in progress or not
        int64_t m_start_game_frame; // アニメーションのゲーム起動から数えた開始フレーム  Start frame of animation counted from game startup.
        common::Vec2<int32_t> m_num_of_segs; // ピクチャの分割数（アニメーション用）  Number of segmentation of picture (for animation) 
        int32_t m_start_seg; // アニメーションの開始区画（範囲に含まれる 0始まり）  Animation start segment (included within range. zero-indexed)
        int32_t m_last_seg; // アニメーションの終了区画（範囲に含まれない 0始まり）  Animation final segment (not included within range. zero-indexed)
        double m_fpf; // ゲームの1フレームあたりのアニメーションのフレーム数  Frames of animation per frame of game

        public:
        // コンストラクタ  Constructor
        Picture(game::Game* game, pictures::Textures* textures, std::string path_or_nickname, common::Vec2<double> xy);
        Picture(game::Game* game, pictures::TextTextures* text_textures, std::string path_or_nickname, std::string text, uint16_t pt, common::Vec2<double> xy);
        // デストラクタ  Destructor
        ~Picture();
        // ピクチャを画面に表示  Display picture on screen
        bool Display();
        // アニメーションを表示  Draw animation
        bool Animation();   

        // ゲッター  Getter
        pictures::Textures* GetTextures() const {return m_textures;}
        pictures::TextTextures* GetTextTextures() const {return m_text_textures;}
        bool GetIsText() const {return m_is_text;}
        std::string GetPathOrNickname() const {return m_path_or_nickname;}
        std::string GetText() const {return m_text;}
        uint16_t GetPt() const {return m_pt;}
        common::Vec2<int32_t> GetXY() const {return m_xy;}
        common::Vec2<double> GetScale() const {return m_scale;}
        bool GetIsCameraTarget() const {return m_is_camera_target;}
        bool GetInAnimation() const {return m_in_animation;}
        // セッター  Setter
        void SetXY(common::Vec2<int32_t> xy){m_xy = xy;}
        void SetPosition(uint8_t position){m_position = position;}
        void SetClipXYAndSize(common::Vec2<int32_t> xy, common::Vec2<int32_t> wh){
            m_srcrct->x = xy.x;
            m_srcrct->y = xy.y;
            m_srcrct->w = wh.x;
            m_srcrct->h = wh.y;
        }
        void SetClipEdge(int32_t right, int32_t bottom, int32_t left, int32_t top){
            m_srcrct->x = left;
            m_srcrct->y = top;
            m_srcrct->w = right - left;
            m_srcrct->h = bottom - top;
        }
        void SetScale(common::Vec2<double> scale){m_scale = scale;}
        void SetAngle(double angle_rad){m_angle_rad = angle_rad;}
        void SetFlip(SDL_RendererFlip flip){m_flip = flip;}
        void SetInAnimation(bool in_animation){m_in_animation = in_animation;}
        void SetStartGameFrame(int64_t start_game_frame){m_start_game_frame = start_game_frame;}
        void SetIsCameraTarget(bool is_camera_target){m_is_camera_target = is_camera_target;}
        void SetAnimation(common::Vec2<int32_t> num_of_segs, int32_t start_frame, int32_t last_frame, double fpf){
            m_num_of_segs = num_of_segs;
            m_start_seg = start_frame;
            m_last_seg = last_frame;
            m_fpf = fpf;
        }
        void SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
            m_color.r = r;
            m_color.g = g;
            m_color.b = b;
            m_color.a = a;
        }
    };

    struct LayerNo{
        int32_t m_layer;
        int32_t m_no;

        LayerNo();
        LayerNo(int32_t layer, int32_t no);
    };

    class Pictures{
        private:
        game::Game* m_game;
        std::map<int32_t, std::map<int32_t, pictures::Picture*>> m_pictures; // 表示するピクチャをレイヤーとレイヤー内の番号で管理する  Manage pictures to be displayed by layer and number in the layer
        pictures::Textures* m_textures;
        pictures::TextTextures* m_text_textures;
        pictures::Camera* m_camera;
        bool changed; // 画面に変化がない場合にDisplayAll()が動かないようにするためのフラグ  Flag to prevent DisplayAll() from working if there is no change on the screen
        
        public:
        // コンストラクタ  Constructor
        Pictures(game::Game* game, pictures::Textures* textures, pictures::TextTextures* text_textures, pictures::Camera* camera);
        // デストラクタ  Destructor
        ~Pictures();
        // 表示するピクチャを追加する  Add picture on display
        bool Add(pictures::LayerNo layer_no, std::string path_or_nickname, common::Vec2<int32_t> xy);
        // 表示するテキストを追加する  Add text on display
        bool Add(pictures::LayerNo layer_no, std::string path_or_nickname, std::string text, uint16_t pt, common::Vec2<int32_t> xy);
        // ピクチャを削除  Delete a picture
        bool Delete(pictures::LayerNo layer_no);
        // 指定したレイヤーの全てのピクチャを削除
        bool DeleteAll(int32_t layer);
        // 指定したピクチャのアニメーションを開始  Starts animation of the specified picture
        void StartAnimation(pictures::LayerNo layer_no);
        // 指定したピクチャのアニメーションを終了  Stop animation of the specified picture
        void StopAnimation(pictures::LayerNo layer_no);
        // 全てのピクチャを表示 毎フレームの最後に呼び出す  Displays all pictures. Called at the end of every frame.
        bool DisplayAll();

        // ゲッター  Getter
        pictures::Textures* GetTextures() const {return m_textures;}
        pictures::TextTextures* GetTextTextures() const {return m_text_textures;}
        // セッター  Setter
        bool SetXY(pictures::LayerNo layer_no, common::Vec2<int32_t> xy){
            if(!m_pictures[layer_no.m_layer].count(layer_no.m_no)) return 1;
            m_pictures[layer_no.m_layer][layer_no.m_no]->SetXY(xy);
            return 0;
        }
        bool SetPosition(pictures::LayerNo layer_no, uint8_t position){
            if(!m_pictures[layer_no.m_layer].count(layer_no.m_no)) return 1;
            m_pictures[layer_no.m_layer][layer_no.m_no]->SetPosition(position);
            return 0;
        }
        bool SetClipXYAndSize(pictures::LayerNo layer_no, common::Vec2<int32_t> xy, common::Vec2<int32_t> wh){
            if(!m_pictures[layer_no.m_layer].count(layer_no.m_no)) return 1;
            m_pictures[layer_no.m_layer][layer_no.m_no]->SetClipXYAndSize(xy, wh);
            return 0;
        }
        bool SetClipEdge(pictures::LayerNo layer_no, int32_t right, int32_t bottom, int32_t left, int32_t top){
            if(!m_pictures[layer_no.m_layer].count(layer_no.m_no)) return 1;
            m_pictures[layer_no.m_layer][layer_no.m_no]->SetClipEdge(right, bottom, left, top);
            return 0;
        }
        bool SetScale(pictures::LayerNo layer_no, common::Vec2<double> scale){
            if(!m_pictures[layer_no.m_layer].count(layer_no.m_no)) return 1;
            m_pictures[layer_no.m_layer][layer_no.m_no]->SetScale(scale);
            return 0;
        }
        bool SetAngle(pictures::LayerNo layer_no, double angle_rad){
            if(!m_pictures[layer_no.m_layer].count(layer_no.m_no)) return 1;
            m_pictures[layer_no.m_layer][layer_no.m_no]->SetAngle(angle_rad);
            return 0;
        }
        bool SetFlip(pictures::LayerNo layer_no, SDL_RendererFlip flip){
            if(!m_pictures[layer_no.m_layer].count(layer_no.m_no)) return 1;
            m_pictures[layer_no.m_layer][layer_no.m_no]->SetFlip(flip);
            return 0;
        }
        bool SetIsCameraTarget(pictures::LayerNo layer_no, bool is_camera_target){
            if(!m_pictures[layer_no.m_layer].count(layer_no.m_no)) return 1;
            m_pictures[layer_no.m_layer][layer_no.m_no]->SetIsCameraTarget(is_camera_target);
            return 0;
        }
        bool SetAnimation(pictures::LayerNo layer_no, common::Vec2<int32_t> num_of_segs, int32_t start_frame, int32_t last_frame, double fpf){
            if(!m_pictures[layer_no.m_layer].count(layer_no.m_no)) return 1;
            m_pictures[layer_no.m_layer][layer_no.m_no]->SetAnimation(num_of_segs, start_frame, last_frame, fpf);
            return 0;
        }
        bool SetRGBA(pictures::LayerNo layer_no, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
            if(!m_pictures[layer_no.m_layer].count(layer_no.m_no)) return 1;
            m_pictures[layer_no.m_layer][layer_no.m_no]->SetRGBA(r, g, b, a);
            return 0;
        }
    };
}
