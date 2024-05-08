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
    inline Polygon transform(Transform2 transform, Vector2 center) const {
        std::vector<Vector2> new_points = points;
        std::cout << "transform: " << transform.translation << ", " << transform.transform(Vector2 {0, 0}) << std::endl;
        for (int i = 0; i < new_points.size(); i++) {
            new_points[i] = transform.transform(points[i], center) + transform.translation;
        }

        return Polygon {new_points};
    }

    std::vector<Vector2> derivatives() const {
        std::vector<Vector2> derivatives;
        for (int i = 0; i < points.size() - 1; i++) {
            derivatives.push_back(points[i + 1] - points[i]);
        }
        return derivatives;
    }

    Vector2 get_normal(Vector2 v, Vector2 direction) const;
};

class Rectangle : public PhysicsEntity {
   public:
    Rectangle(int width, int height, Scene& scene, Color color = Color{0, 0, 255, 255}) 
        : PhysicsEntity{scene}, width{width}, height{height} {
        unsigned char* data = (unsigned char*)calloc(width*height*3, sizeof(unsigned char));
        
        Image image = Image {data, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8};
        ImageDrawRectangle(&image, 0, 0, width, height, color);
        
        surf = LoadTextureFromImage(image);
        free(data);

        center = {(float)width/2, (float)height/2};
    }


    Rectangle(int width, int height, Scene& scene, Texture2D surf) 
        : PhysicsEntity{scene}, width{width}, height{height} {
        this->surf = surf;
        center = {(float)width/2, (float)height/2};
    }

    inline Vector2 get_x1y1(Transform2 trans) const {
        Vector2 p = Vector2{trans.translation.x, trans.translation.y};
        return transform.transform(p, center);
    }
    inline Vector2 get_x2y1(Transform2 trans) const {
        Vector2 p = Vector2{trans.translation.x + width, trans.translation.y};
        return transform.transform(p, center);
    }
    inline Vector2 get_x1y2(Transform2 trans) const {
        Vector2 p = Vector2{trans.translation.x, trans.translation.y + height};
        return transform.transform(p, center);
    }
    inline Vector2 get_x2y2(Transform2 trans) const {
        Vector2 p = Vector2{trans.translation.x + width, trans.translation.y + height};
        return transform.transform(p, center);
    }

    inline Polygon box() const {
        return Polygon {
            get_x1y1(transform),
            get_x2y1(transform),
            get_x2y2(transform),
            get_x1y2(transform),
        };
    }
    
    inline Polygon old_box() const {
        return Polygon {
            get_x1y1(old_transform),
            get_x2y1(old_transform),
            get_x2y2(old_transform),
            get_x1y2(old_transform),
        };
    }
    virtual Polygon get_polygon() const override {
        return box();
    };
    

    virtual Vector2 get_normal(Vector2 v, Vector2 direction) const override;
    
    virtual void handle_collision(PhysicsEntity* phys_other) override;

    bool intersects(const Rectangle& other) const;

    int width;
    int height;
};



class PolygonEntity : public PhysicsEntity {
   public:
    PolygonEntity(Polygon base, Scene& scene, Color color = Color{0, 255, 255, 255}) 
        : PhysicsEntity{scene}, base{base} {

        int width = 0;
        int height = 0;
        for (const auto& p : base.points) {
            if (p.x > width) {
                width = p.x;
            }
            if (p.y > height) {
                height = p.y;
            }
        }

        unsigned char* data = (unsigned char*)calloc(width*height*4, sizeof(unsigned char));
        
        Image image = Image {data, width, height+1, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
        
        image_draw_polygon(image, base.points, color);
        
        surf = LoadTextureFromImage(image);
        free(data);

        center = {(float)width/2, (float)height/2};
    }

    inline Polygon box() const {
        return base.transform(transform, center);
    }
    
    inline Polygon old_box() const {
        return base.transform(old_transform, center);
    }
    virtual Polygon get_polygon() const override {
        return box();
    };
    

    virtual Vector2 get_normal(Vector2 v, Vector2 direction) const override;
    

    bool intersects(const Rectangle& other) const;

    Polygon base;
};

bool line_intersects(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2, bool colinear_intersection = true);

}  // namespace vl