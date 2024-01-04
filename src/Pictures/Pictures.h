#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "../Game.h"
#include "../Common/Vector.h"
#include "Fonts.h"
#include "Textures.h"

namespace pictures{
    // ピクチャの表示レイヤ 後ろにあるほど画面の前面に近い  The display layer of the picture. The further back it is, the closer it is to the front of the screen.
    enum Layer{
        BACK,
        FRONT
    };

    class Picture{
        private:
        game::Game *m_game;
        pictures::Textures* m_textures;
        pictures::TextTextures* m_text_textures;
        bool m_is_text; // ピクチャが画像かテキストか  Whether the picture is an image or text
        std::string m_path; // 画像ファイルのパス  Image file path
        std::string m_text; // (テキストのみ)表示テキスト  // (Only text) Display text
        uint16_t m_pt; // (テキストのみ)テキストのサイズ  // (Only text) Text size
        SDL_Rect* m_srcrct; // SDL_RenderCopy source rect
        common::Vec2 m_xy; // ピクチャの座標  Picture coordinates
        uint8_t m_position; // ピクチャの表示を合わせる位置  Position to align the picture display
        common::Vec2 m_scale; // 表示スケール  Display scale
        double m_angle_rad; // ピクチャの回転角度[rad]  Picture rotation angle[rad]
        SDL_RendererFlip m_flip; // ピクチャの縦横反転  Flip picture horizontally and vertically
        SDL_Color m_color; // 色調補正  Color modulation
        bool m_in_animation; // アニメーション中かどうか  Whether animation is in progress or not
        int64_t m_start_game_frame; // アニメーションのゲーム起動から数えた開始フレーム  Start frame of animation counted from game startup.
        common::Vec2 m_num_of_segs; // ピクチャの分割数（アニメーション用）  Number of segmentation of picture (for animation) 
        int32_t m_start_seg; // アニメーションの開始区画（範囲に含まれる 0始まり）  Animation start segment (included within range. zero-indexed)
        int32_t m_last_seg; // アニメーションの終了区画（範囲に含まれない 0始まり）  Animation final segment (not included within range. zero-indexed)
        double m_fpf; // ゲームの1フレームあたりのアニメーションのフレーム数  Frames of animation per frame of game

        public:
        // コンストラクタ  Constructor
        Picture(game::Game* game, pictures::Textures* textures, std::string path, common::Vec2 xy);
        Picture(game::Game* game, pictures::TextTextures* text_textures, std::string path, std::string text, uint16_t pt, common::Vec2 xy);
        // デストラクタ  Destructor
        ~Picture();
        // ピクチャを画面に表示  Display picture on screen
        bool Display();
        // アニメーションを表示  Draw animation
        bool Animation();   

        // ゲッター  Getter
        bool GetIsText() const {return m_is_text;}
        std::string GetPath() const {return m_path;}
        std::string GetText() const {return m_text;}
        uint16_t GetPt() const {return m_pt;}
        bool GetInAnimation() const {return m_in_animation;}
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
        void SetInAnimation(bool in_animation){m_in_animation = in_animation;}
        void SetStartGameFrame(int64_t start_game_frame){m_start_game_frame = start_game_frame;}
        void SetAnimation(common::Vec2 num_of_segs, int32_t start_frame, int32_t last_frame, double fpf){
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

    struct LayerAndNo{
        pictures::Layer m_layer;
        int32_t m_no;

        LayerAndNo();
        LayerAndNo(pictures::Layer layer, int32_t no);
    };

    class Pictures{
        private:
        game::Game* m_game;
        std::map<pictures::Layer, std::map<int32_t, pictures::Picture*>> m_pictures; // 表示するピクチャをレイヤーとレイヤー内の番号で管理する  Manage pictures to be displayed by layer and number in the layer
        pictures::Textures* m_textures;
        pictures::TextTextures* m_text_textures;
        bool changed; // 画面に変化がない場合にDisplayAll()が動かないようにするためのフラグ  Flag to prevent DisplayAll() from working if there is no change on the screen
        
        public:
        // コンストラクタ  Constructor
        Pictures(game::Game* game, pictures::Textures* textures, pictures::TextTextures* text_textures);
        // デストラクタ  Destructor
        ~Pictures();
        // 表示するピクチャを追加する  Add picture on display
        bool Add(pictures::LayerAndNo layer_and_no, std::string path, common::Vec2 xy);
        // 表示するテキストを追加する  Add text on display
        bool Add(pictures::LayerAndNo layer_and_no, std::string path, std::string text, uint16_t pt, common::Vec2 xy);
        // ピクチャを削除  Delete a picture
        bool Delete(pictures::LayerAndNo layer_and_no);
        // 指定したピクチャのアニメーションを開始  Starts animation of the specified picture
        void StartAnimation(pictures::LayerAndNo layer_and_no);
        // 指定したピクチャのアニメーションを終了  Stop animation of the specified picture
        void StopAnimation(pictures::LayerAndNo layer_and_no);
        // 全てのピクチャを表示 毎フレームの最後に呼び出す  Displays all pictures. Called at the end of every frame.
        bool DisplayAll();

        // セッター  Setter
        bool SetXY(pictures::LayerAndNo layer_and_no, common::Vec2 xy){
            if(!m_pictures[layer_and_no.m_layer].count(layer_and_no.m_no)) return 1;
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no]->SetXY(xy);
            return 0;
        }
        bool SetPosition(pictures::LayerAndNo layer_and_no, uint8_t position){
            if(!m_pictures[layer_and_no.m_layer].count(layer_and_no.m_no)) return 1;
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no]->SetPosition(position);
            return 0;
        }
        bool SetClipXYAndSize(pictures::LayerAndNo layer_and_no, common::Vec2 xy, common::Vec2 wh){
            if(!m_pictures[layer_and_no.m_layer].count(layer_and_no.m_no)) return 1;
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no]->SetClipXYAndSize(xy, wh);
            return 0;
        }
        bool SetClipEdge(pictures::LayerAndNo layer_and_no, int32_t right, int32_t bottom, int32_t left, int32_t top){
            if(!m_pictures[layer_and_no.m_layer].count(layer_and_no.m_no)) return 1;
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no]->SetClipEdge(right, bottom, left, top);
            return 0;
        }
        bool SetScale(pictures::LayerAndNo layer_and_no, common::Vec2 scale){
            if(!m_pictures[layer_and_no.m_layer].count(layer_and_no.m_no)) return 1;
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no]->SetScale(scale);
            return 0;
        }
        bool SetAngle(pictures::LayerAndNo layer_and_no, double angle_rad){
            if(!m_pictures[layer_and_no.m_layer].count(layer_and_no.m_no)) return 1;
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no]->SetAngle(angle_rad);
            return 0;
        }
        bool SetFlip(pictures::LayerAndNo layer_and_no, SDL_RendererFlip flip){
            if(!m_pictures[layer_and_no.m_layer].count(layer_and_no.m_no)) return 1;
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no]->SetFlip(flip);
            return 0;
        }
        bool SetAnimation(pictures::LayerAndNo layer_and_no, common::Vec2 num_of_segs, int32_t start_frame, int32_t last_frame, double fpf){
            if(!m_pictures[layer_and_no.m_layer].count(layer_and_no.m_no)) return 1;
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no]->SetAnimation(num_of_segs, start_frame, last_frame, fpf);
            return 0;
        }
        bool SetRGBA(pictures::LayerAndNo layer_and_no, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
            if(!m_pictures[layer_and_no.m_layer].count(layer_and_no.m_no)) return 1;
            m_pictures[layer_and_no.m_layer][layer_and_no.m_no]->SetRGBA(r, g, b, a);
            return 0;
        }
    };
}