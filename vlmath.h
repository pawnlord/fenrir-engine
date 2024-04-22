#pragma once
#include "raylib.h"
#include "raymath.h"
namespace vl {

template<class T>
inline Vector2 operator*(T const& scalar, Vector2 rhs) {
    return Vector2Scale(rhs, scalar);
}

template<class T>
inline Vector2 operator*(Vector2 rhs, T const& scalar) {
    return Vector2Scale(rhs, scalar);
}
template<class T>
inline Vector2 operator/(Vector2 rhs, T const& scalar) {
    return Vector2Scale(rhs, 1/scalar);
}

inline Vector2 operator+(Vector2 const& lhs, Vector2 rhs) {
    return Vector2Add(lhs, rhs);
}

inline Vector2 operator+=(Vector2& lhs, Vector2 rhs) {
    lhs = Vector2Add(lhs, rhs);
    return lhs;
}
inline Vector2 operator-(Vector2 const& vec) {
    return -1 * vec;
}

inline bool operator==(Vector2 const& lhs, Vector2 const& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline Vector2 operator-(Vector2 const& lhs, Vector2 rhs) {
    return lhs + -1 * rhs;
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

inline Vector2 rotate(float rot_rad, Vector2 v) {
    float curr_rot = atan2(v.y, v.x);
    float mag = sqrt(v.y*v.y + v.x*v.x);
    curr_rot += rot_rad;
    return Vector2 {mag * cos(curr_rot), mag * sin(curr_rot)};
}

inline float dot(const Vector2& us, const Vector2& other) {
    return (us.x * other.x) + (us.y * other.y);
}
// Normalize CCW
inline Vector2 vector2_get_normal(const Vector2& v) {
    return Vector2Normalize(Vector2 {v.y, -v.x});
}

template <typename T> int signum(T val) {
    return (T(0) < val) - (val < T(0));
}

struct Transform2 {
    Vector2 translation = Vector2 {0.0, 0.0};    // Translation
    float rot_rad = 0.0;    // Rotation radians
    float scale = 1.0;
    inline Vector2 transform(Vector2 p) const {
        Vector2 scaled = scale * (p - translation);
        Vector2 rotated = rotate(rot_rad, scaled);
        return translation + rotated;
    }
    
    inline Vector2 transform(Vector2 p, Vector2 center) const {
        Vector2 scaled = scale * (p - translation - center);
        Vector2 rotated = rotate(rot_rad, scaled);
        return translation + center + rotated;
    }    
};

}