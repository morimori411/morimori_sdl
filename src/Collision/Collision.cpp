#include "Collision.h"

collision::Rectangle::Rectangle(){}

collision::Rectangle::Rectangle(game::Game* game, double right_display_x, double bottom_display_y, double left_display_x, double top_display_y){
    this->game = game;
    this->right_display_x = right_display_x;
    this->bottom_display_y = bottom_display_y;
    this->left_display_x = left_display_x;
    this->top_display_y = top_display_y;
}

collision::Circle::Circle(){}

collision::Circle::Circle(game::Game* game, double center_display_x, double center_display_y, double radius){
    this->game = game;
    this->center_display_x = center_display_x;
    this->center_display_y = center_display_y;
    this->radius = radius;
}

bool collision::isInCollision(collision::Rectangle* rectangle1, collision::Rectangle* rectangle2){
    if(rectangle1->right_display_x < rectangle2->left_display_x){
        return false;
    }
    if(rectangle1->left_display_x > rectangle2->right_display_x){
        return false;
    }
    if(rectangle1->top_display_y > rectangle2->bottom_display_y){
        return false;
    }
    if(rectangle1->bottom_display_y < rectangle2->top_display_y){
        return false;
    }
    return true;
}

float collision::isInCollision(collision::Rectangle* rectangle, collision::Circle* circle){
    // 円の右端の画面x座標
    double circle_right_display_x = circle->center_display_x + circle->radius;
    // 円の上端の画面y座標
    double circle_top_display_y = circle->center_display_y - circle->radius;
    // 円の左端の画面x座標
    double circle_left_display_x = circle->center_display_x - circle->radius;
    // 円の下端の画面y座標
    double circle_bottom_display_y = circle->center_display_y + circle->radius;
    // 円が矩形であると仮定して衝突判定
    Rectangle* virtual_rectangle = new Rectangle(nullptr, circle_right_display_x, circle_top_display_y, circle_left_display_x, circle_bottom_display_y);
    // 矩形で判定して衝突する場合
    if(isInCollision(rectangle, virtual_rectangle)){
        // 円形で判定すると衝突しない可能性がある
        // 円形の中心が矩形の斜めにある場合
        if(circle->center_display_x > rectangle->right_display_x && circle->center_display_y < rectangle->top_display_y || circle->center_display_x < rectangle->left_display_x && circle->center_display_y < rectangle->top_display_y || circle->center_display_x < rectangle->left_display_x && circle->center_display_y > rectangle->bottom_display_y || circle->center_display_x > rectangle->right_display_x && circle->center_display_y > rectangle->bottom_display_y){
            // 矩形の4頂点との距離が全て円形の半径より大きい場合
            if(common::dist(circle->center_display_x, circle->center_display_y, rectangle->right_display_x, rectangle->top_display_y) > circle->radius * circle->radius && common::dist(circle->center_display_x, circle->center_display_y, rectangle->left_display_x, rectangle->top_display_y) > circle->radius * circle->radius && common::dist(circle->center_display_x, circle->center_display_y, rectangle->left_display_x, rectangle->bottom_display_y) > circle->radius * circle->radius && common::dist(circle->center_display_x, circle->center_display_y, rectangle->right_display_x, rectangle->bottom_display_y) > circle->radius * circle->radius){
                // 衝突しない
                return -1;
            // そうでない、すなわち矩形の角と円形が衝突している場合
            }else{
                // 返り値
                float dir;
                // 右上の角
                if(circle->center_display_x > rectangle->right_display_x && circle->center_display_y < rectangle->top_display_y){
                    dir = atan2(circle->center_display_y - rectangle->top_display_y, circle->center_display_x - rectangle->right_display_x);
                // 左上の角
                }else if(circle->center_display_x < rectangle->left_display_x && circle->center_display_y < rectangle->top_display_y){
                    dir = atan2(circle->center_display_y - rectangle->top_display_y, circle->center_display_x - rectangle->left_display_x);
                // 左下の角
                }else if(circle->center_display_x < rectangle->left_display_x && circle->center_display_y > rectangle->bottom_display_y){
                    dir = atan2(circle->center_display_y - rectangle->bottom_display_y, circle->center_display_x - rectangle->left_display_x);
                // 右下の角
                }else{
                    dir = atan2(circle->center_display_y - rectangle->bottom_display_y, circle->center_display_x - rectangle->right_display_x);
                }
                // 値を返す
                if(dir < 0){
                    return dir + M_PI;
                }else{
                    return dir;   
                }
            }
        // 矩形の右にある場合
        }else if(circle->center_display_x > rectangle->right_display_x){
            return 0.000001;
        // 矩形の上にある場合
        }else if(circle->center_display_y < rectangle->top_display_y){
            return M_PI * 3 / 2;
        // 矩形の左にある場合
        }else if(circle->center_display_x < rectangle->left_display_x){
            return M_PI;
        // 矩形の下にある場合
        }else if(circle->center_display_y > rectangle->top_display_y){
            return M_PI / 2;
        // 中に入り込んでいる場合
        }else{
            // とりあえず0を返す
            return 0;
        }
    // 矩形で判定して衝突しない場合
    }else{
        // 円形で判定しても絶対に衝突しない
        return -1;
    }
}

float collision::isInCollision(collision::Circle* circle, collision::Rectangle* rectangle){
    return isInCollision(rectangle, circle);
}

float collision::isInCollision(collision::Circle* circle1, collision::Circle* circle2){
    // 2乗したまま比較
    // 半径の合計
    uint32_t radius_sum = (circle1->radius + circle2->radius) * (circle1->radius + circle2->radius);
    // 円の中心間の距離
    uint32_t dist = common::dist(circle1->center_display_x, circle1->center_display_y, circle2->center_display_x, circle2->center_display_y);
    // 衝突判定
    if(dist <= radius_sum){
        // 衝突する場合力積の方向を計算
        float dir = atan2f(circle1->center_display_y - circle2->center_display_y, circle1->center_display_x - circle2->center_display_x);
        if(dir < 0){
            return dir + M_PI * 2;
        }else{
            return dir;   
        }
    }else{
        return -1;
    }
}