#pragma once

#include <cstdint>
#include <math.h>

#include "../Game.h"
#include "../Common/Common.h"

// 当たり判定関連
namespace collision{
    // 矩形の当たり判定
    struct Rectangle{
        game::Game* game; // ゲームオブジェクト
        double right_display_x; // 右上の画面x座標
        double top_display_y; // 左上の画面y座標
        double left_display_x; // 左下の画面x座標
        double bottom_display_y; // 右下の画面y座標

        // コンストラクタ
        Rectangle();
        Rectangle(game::Game* game, double right_display_x, double bottom_display_y, double left_display_x, double top_display_y);
    };

    // 円形の当たり判定
    struct Circle{
        game::Game* game; // ゲームオブジェクト
        double center_display_x; // 中心の画面x座標
        double center_display_y; // 中心の画面y座標
        double radius; // 半径

        // コンストラクタ
        Circle();
        Circle(game::Game* game, double center_display_x, double center_display_y, double radius);
    };

    // 矩形同士の当たり判定
    bool isInCollision(Rectangle* rectangle1, Rectangle* rectangle2);
    // 矩形と円形の当たり判定 第1引数から第2引数への力積の角度[rad]を返す 当たっていない場合-1
    float isInCollision(Rectangle* rectangle, Circle* circle);
    // 円形と矩形の当たり判定 第2引数から第1引数への力積の角度[rad]を返す 当たっていない場合-1
    float isInCollision(Circle* circle, Rectangle* rectangle);
    // 円形と円形の当たり判定 第2引数から第1引数への力積の角度[rad]を返す 当たっていない場合-1
    float isInCollision(Circle* circle1, Circle* circle2);
}