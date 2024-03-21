#pragma once
#include "raylib.h"
namespace vl {

struct Transform2 {
    Vector2 translation = Vector2 {0.0, 0.0};    // Translation
    float rot_rad = 0.0;    // Rotation radians
    float scale = 1.0;
};

}