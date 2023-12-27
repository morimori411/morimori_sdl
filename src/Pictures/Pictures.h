#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "../Game.h"
#include "../Common/Vector.h"
#include "../Pictures/Textures.h"

namespace pictures{
    // ピクチャの表示レイヤ 後ろにあるほど画面の前面に近い  The display layer of the picture. The further back it is, the closer it is to the front of the screen.
    enum Layer{
        BACK,
        FRONT
    };

    class Picture{
        private:
        pictures::Textures* m_textures;
        std::string m_path; // 画像ファイルのパス  Image file path
        SDL_Rect* m_srcrct; // SDL_RenderCopy source rect
        common::Vec2 m_xy; // 画像の座標  Image coordinates
        uint8_t m_position; // ピクチャの表示を合わせる位置  Position to align the picture display
        common::Vec2 m_scale; // 表示スケール  Display scale
        double m_angle_rad; // 画像の回転角度[rad]  Image rotation angle[rad]
        SDL_RendererFlip m_flip; // 画像の縦横反転  Flip image horizontally and vertically
        SDL_Color m_color; // 色調補正  Color modulation
        common::Vec2 m_num_of_segs; // 画像の分割数（アニメーション用）  Number of segmentation of image (for animation) 
        int32_t m_start_frame; // アニメーションの開始フレーム（範囲に含まれる 0始まり）  Animation start frame (included within range. zero-indexed)
        int32_t m_last_frame; // アニメーションの終了フレーム（範囲に含まれない 0始まり）  Animation final frame (not included within range. zero-indexed)
        double m_fpf; // ゲームの1フレームあたりのアニメーションのフレーム数  Frames of animation per frame of game

        public:
        // コンストラクタ  Constructor
        Picture(pictures::Textures* textures, std::string path, common::Vec2 xy);
        // デストラクタ  Destructor
        ~Picture();
        // 画像を画面に表示  Display picture on screen
        bool Display();
        // アニメーションを表示  Draw animation
        bool Animation(int64_t start_game_frame, int64_t current_game_frame);   

        // セッター  Setter
        void SetXY(common::Vec2 xy){m_xy = xy;}
        void SetPosition(uint8_t position){m_position = position;}
        void SetClipXYAndSize(common::Vec2 xy, common::Vec2 wh){
            m_srcrct->x = xy.m_x;
            m_srcrct->y = xy.m_y;
            m_srcrct->w = wh.m_x;
            m_srcrct->h = wh.m_y;
        }
        void SetClipEdge(int32_t right, int32_t bottom, int32_t left, int32_t top){
            m_srcrct->x = left;
            m_srcrct->y = top;
            m_srcrct->w = right - left;
            m_srcrct->h = bottom - top;
        }
        void SetScale(common::Vec2 scale){m_scale = scale;}
        void SetAngle(double angle_rad){m_angle_rad = angle_rad;}
        void SetFlip(SDL_RendererFlip flip){m_flip = flip;}
        void SetAnimation(common::Vec2 num_of_segs, int32_t start_frame, int32_t last_frame, double fpf){
            m_num_of_segs = num_of_segs;
            m_start_frame = start_frame;
            m_last_frame = last_frame;
            m_fpf = fpf;
        }
        void SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
            m_color.r = r;
            m_color.g = g;
            m_color.b = b;
            m_color.a = a;
        }
    };

    struct LayerAndNo{
        pictures::Layer m_layer;
        int32_t m_no;

        LayerAndNo();
        LayerAndNo(pictures::Layer layer, int32_t no);
    };

    struct AnimationOption{
        pictures::Picture* picture;
        bool in_animation;
        int64_t start_game_frame;
    };

    class Pictures{
        private:
        game::Game* m_game;
        std::map<pictures::Layer, std::map<int32_t, pictures::AnimationOption>> m_pictures; // 表示するピクチャをレイヤーとレイヤー内の番号で管理する  Manage pictures to be displayed by layer and number in the layer
        pictures::Textures* m_textures;
        bool changed; // 画面に変化がない場合にDisplayAll()が動かないようにするためのフラグ  Flag to prevent DisplayAll() from working if there is no change on the screen
        
        public:
        Pictures(game::Game* game, pictures::Textures* textures); // コンストラクタ  Constructor
        ~Pictures(); // デストラクタ  Destructor
        void Add(pictures::LayerAndNo layer_and_no, std::string path, common::Vec2 xy); // 表示するピクチャを追加する  Add picture on display
        void Delete(pictures::LayerAndNo layer_and_no); // ピクチャを削除  Delete a picture
        void StartAnimation(pictures::LayerAndNo layer_and_no); // 指定したピクチャのアニメーションを開始  Starts animation of the specified picture
        void StopAnimation(pictures::LayerAndNo layer_and_no);
        bool DisplayAll(); // 全てのピクチャを表示 毎フレームの最後に呼び出す  Displays all pictures. Called at the end of every frame.

        // セッター  Setter
        void SetXY(pictures::LayerAndNo layer_and_no, common::Vec2 xy){
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no].picture->SetXY(xy);
        }
        void SetPosition(pictures::LayerAndNo layer_and_no, uint8_t position){
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no].picture->SetPosition(position);
        }
        void SetClipXYAndSize(pictures::LayerAndNo layer_and_no, common::Vec2 xy, common::Vec2 wh){
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no].picture->SetClipXYAndSize(xy, wh);
        }
        void SetClipEdge(pictures::LayerAndNo layer_and_no, int32_t right, int32_t bottom, int32_t left, int32_t top){
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no].picture->SetClipEdge(right, bottom, left, top);
        }
        void SetScale(pictures::LayerAndNo layer_and_no, common::Vec2 scale){
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no].picture->SetScale(scale);
        }
        void SetAngle(pictures::LayerAndNo layer_and_no, double angle_rad){
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no].picture->SetAngle(angle_rad);
        }
        void SetFlip(pictures::LayerAndNo layer_and_no, SDL_RendererFlip flip){
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no].picture->SetFlip(flip);
        }
        void SetAnimation(pictures::LayerAndNo layer_and_no, common::Vec2 num_of_segs, int32_t start_frame, int32_t last_frame, double fpf){
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no].picture->SetAnimation(num_of_segs, start_frame, last_frame, fpf);
        }
        void SetRGBA(pictures::LayerAndNo layer_and_no, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no].picture->SetRGBA(r, g, b, a);
        }
    };
}