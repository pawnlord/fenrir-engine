#pragma once
#include "raylib.h"
namespace vl {

inline Vector2 add(Vector2 v1, Vector2 v2) {
    return Vector2 {v1.x + v2.x, v1.y + v2.y};
}

inline Vector2 sub(Vector2 v1, Vector2 v2) {
    return Vector2 {v1.x - v2.x, v1.y - v2.y};
}

inline Vector2 neg(Vector2 v) {
    return Vector2 {-v.x, -v.y};
}

inline Vector2 mul(float lambda, Vector2 v) {
    return Vector2 {lambda * v.x, lambda * v.y};
}

inline Vector2 rotate(float rot_rad, Vector2 v) {
    float curr_rot = atan2(v.y, v.x);
    float mag = sqrt(v.y*v.y + v.x*v.x);
    curr_rot += rot_rad;
    return Vector2 {mag * cos(curr_rot), mag * sin(curr_rot)};
}


struct Transform2 {


    Vector2 translation = Vector2 {0.0, 0.0};    // Translation
    float rot_rad = 0.0;    // Rotation radians
    float scale = 1.0;
    inline Vector2 transform(Vector2 p) const {
        Vector2 scaled = (mul(scale, sub(p, translation)));
        Vector2 rotated = rotate(rot_rad, scaled);
        return add(translation, rotated);
    }
};

}