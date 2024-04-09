#pragma once

#include "graphics_context.h"
#include "vlmath.h"
namespace vl {


enum class Orientation {
    Colinear,
    CW,
    CCW
};


/*
 * A box is the points of a rectangle
 * used for collision
 */
struct Box {
    Box(Vector2 x1y1, Vector2 x2y1, Vector2 x1y2, Vector2 x2y2) : 
            x1y1{x1y1}, x2y1{x2y1}, x1y2{x1y2}, x2y2{x2y2} {}

    Vector2 x1y1;
    Vector2 x2y1;
    Vector2 x1y2;
    Vector2 x2y2;
    bool intersects(const Box& other) const;
};

struct Polygon {
    // clockwise-points
    std::vector<Vector2> points;
    Polygon(std::vector<Vector2> points) : points{points} {}
    Polygon(std::initializer_list<Vector2> list) : points{list} {}

    bool intersects(const Polygon& other) const;

    std::vector<Vector2> derivatives() const {
        std::vector<Vector2> derivatives;
        for (int i = 0; i < points.size() - 1; i++) {
            derivatives.push_back(points[i + 1] - points[i]);
        }
        return derivatives;
    }
};

class Rectangle : public PhysicsEntity {
   public:
    Rectangle(int width, int height, Scene& scene, Color color = Color{0, 0, 255, 255}) 
        : PhysicsEntity{scene}, width{width}, height{height}, color{color} {
        unsigned char* data = (unsigned char*)calloc(width*height*3, sizeof(unsigned char));
        
        Image image = Image {data, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8};
        ImageDrawRectangle(&image, 0, 0, width, height, color);
        
        surf = LoadTextureFromImage(image);

        free(data);
    }

    inline Vector2 get_x1y1() const {
        Vector2 p = Vector2{transform.translation.x, transform.translation.y};
        return transform.transform(p);
    }
    inline Vector2 get_x2y1() const {
        Vector2 p = Vector2{transform.translation.x + width, transform.translation.y};
        return transform.transform(p);
    }
    inline Vector2 get_x1y2() const {
        Vector2 p = Vector2{transform.translation.x, transform.translation.y + height};
        return transform.transform(p);
    }
    inline Vector2 get_x2y2() const {
        Vector2 p = Vector2{transform.translation.x + width, transform.translation.y + height};
        return transform.transform(p);
    }

    inline Polygon box() const {
        return Polygon {
            get_x1y1(),
            get_x2y1(),
            get_x2y2(),
            get_x1y2(),
        };
    }
    virtual Polygon get_polygon() const override {
        return box();
    };

    virtual Vector2 get_normal(Vector2 v, Vector2 direction) const override;

    bool intersects(const Rectangle& other) const;

    int width;
    int height;
    Color color;
};



}  // namespace vl