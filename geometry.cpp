#include "geometry.h"

namespace vl {

bool Box::intersects(const Box& other) const {
    // Project other onto this
    {
        Vector2 projx = Vector2Normalize(Vector2Subtract(this->x2y1, this->x1y1));
        float length = Vector2Distance(this->x1y1, this->x2y1);
        float projx1y1 = Vector2DotProduct(Vector2Subtract(other.x1y1, this->x1y1), projx);
        float projx2y1 = Vector2DotProduct(Vector2Subtract(other.x2y1, this->x1y1), projx);
        float projx1y2 = Vector2DotProduct(Vector2Subtract(other.x1y2, this->x1y1), projx);
        float projx2y2 = Vector2DotProduct(Vector2Subtract(other.x2y2, this->x1y1), projx);
        if ((projx1y1 <= 0 && projx2y1 <= 0 && projx1y2 <= 0 && projx2y2 <= 0)
            || (projx1y1 >= length && projx2y1 >= length && projx1y2 >= length && projx2y2 >= length)) {
            return false;
        }

        
        Vector2 projy = Vector2Normalize(Vector2Subtract(this->x1y2, this->x1y1));
        length = Vector2Distance(this->x1y1, this->x1y2);
        projx1y1 = Vector2DotProduct(Vector2Subtract(other.x1y1, this->x1y1), projy);
        projx2y1 = Vector2DotProduct(Vector2Subtract(other.x2y1, this->x1y1), projy);
        projx1y2 = Vector2DotProduct(Vector2Subtract(other.x1y2, this->x1y1), projy);
        projx2y2 = Vector2DotProduct(Vector2Subtract(other.x2y2, this->x1y1), projy);
        if ((projx1y1 <= 0 && projx2y1 <= 0 && projx1y2 <= 0 && projx2y2 <= 0)
            || (projx1y1 >= length && projx2y1 >= length && projx1y2 >= length && projx2y2 >= length)) {
            return false;
        }
    }
    
    // Project this onto other
    {
        Vector2 projx = Vector2Normalize(Vector2Subtract(other.x2y1, other.x1y1));
        float length = Vector2Distance(other.x1y1, other.x2y1);
        float projx1y1 = Vector2DotProduct(Vector2Subtract(this->x1y1, other.x1y1), projx);
        float projx2y1 = Vector2DotProduct(Vector2Subtract(this->x2y1, other.x1y1), projx);
        float projx1y2 = Vector2DotProduct(Vector2Subtract(this->x1y2, other.x1y1), projx);
        float projx2y2 = Vector2DotProduct(Vector2Subtract(this->x2y2, other.x1y1), projx);
        if ((projx1y1 <= 0 && projx2y1 <= 0 && projx1y2 <= 0 && projx2y2 <= 0)
            || (projx1y1 >= length && projx2y1 >= length && projx1y2 >= length && projx2y2 >= length)) {
            return false;
        }

        
        Vector2 projy = Vector2Normalize(Vector2Subtract(other.x1y2, other.x1y1));
        length = Vector2Distance(other.x1y1, other.x1y2);
        projx1y1 = Vector2DotProduct(Vector2Subtract(this->x1y1, other.x1y1), projy);
        projx2y1 = Vector2DotProduct(Vector2Subtract(this->x2y1, other.x1y1), projy);
        projx1y2 = Vector2DotProduct(Vector2Subtract(this->x1y2, other.x1y1), projy);
        projx2y2 = Vector2DotProduct(Vector2Subtract(this->x2y2, other.x1y1), projy);
        if ((projx1y1 <= 0 && projx2y1 <= 0 && projx1y2 <= 0 && projx2y2 <= 0)
            || (projx1y1 >= length && projx2y1 >= length && projx1y2 >= length && projx2y2 >= length)) {
            return false;
        }
    }

    return true;
}

bool Rectangle::intersects(const Rectangle& other) const {
    Box this_box = this->box();
    Box other_box = other.box();

    return this_box.intersects(other_box);
}

}  // namespace vl