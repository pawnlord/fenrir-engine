#pragma once

#include "graphics_context.h"
#include "vlmath.h"
namespace vl {

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

class Rectangle : public Entity {
   public:
    Rectangle(int width, int height, Scene& scene, Color color = Color{0, 0, 255, 255}) 
        : Entity{scene}, width{width}, height{height}, color{color} {
        unsigned char* data = (unsigned char*)calloc(width*height*3, sizeof(unsigned char));
        
        Image image = Image {data, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8};
        ImageDrawRectangle(&image, 0, 0, width, height, color);
        
        surf = LoadTextureFromImage(image);

        free(data);
    }

    inline Vector2 get_x1y1() const {
        Vector2 p = Vector2{transform.translation.x - (width / 2), transform.translation.y - (height / 2)};
        return transform.transform(p);
    }
    inline Vector2 get_x2y1() const {
        Vector2 p = Vector2{transform.translation.x + (width / 2), transform.translation.y - (height / 2)};
        return transform.transform(p);
    }
    inline Vector2 get_x1y2() const {
        Vector2 p = Vector2{transform.translation.x - (width / 2), transform.translation.y + (height / 2)};
        return transform.transform(p);
    }
    inline Vector2 get_x2y2() const {
        Vector2 p = Vector2{transform.translation.x + (width / 2), transform.translation.y + (height / 2)};
        return transform.transform(p);
    }

    inline Box box() const {
        return Box(
            get_x1y1(),
            get_x2y1(),
            get_x1y2(),
            get_x2y2()
        );
    }

    bool intersects(const Rectangle& other) const;

    int width;
    int height;
    Color color;
};

}  // namespace vl