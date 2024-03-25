#include "geometry.h"

namespace vl {

bool check_extreme_projection(const Box& us, const Box& other) {
    Vector2 projx = Vector2Normalize(us.x2y1 - us.x1y1);
    float length = Vector2Distance(us.x1y1, us.x2y1);
    float projx1y1 = dot(other.x1y1 - us.x1y1, projx);
    float projx2y1 = dot(other.x2y1 - us.x1y1, projx);
    float projx1y2 = dot(other.x1y2 - us.x1y1, projx);
    float projx2y2 = dot(other.x2y2 - us.x1y1, projx);
    if ((projx1y1 <= 0 && projx2y1 <= 0 && projx1y2 <= 0 && projx2y2 <= 0)
        || (projx1y1 >= length && projx2y1 >= length && projx1y2 >= length && projx2y2 >= length)) {
        return false;
    }

    
    Vector2 projy = Vector2Normalize(us.x1y2 - us.x1y1);
    length = Vector2Distance(us.x1y1, us.x1y2);
    projx1y1 = dot(other.x1y1 - us.x1y1, projy);
    projx2y1 = dot(other.x2y1 - us.x1y1, projy);
    projx1y2 = dot(other.x1y2 - us.x1y1, projy);
    projx2y2 = dot(other.x2y2 - us.x1y1, projy);
    if ((projx1y1 <= 0 && projx2y1 <= 0 && projx1y2 <= 0 && projx2y2 <= 0)
        || (projx1y1 >= length && projx2y1 >= length && projx1y2 >= length && projx2y2 >= length)) {
        return false;
    }
    return true;
}
bool Box::intersects(const Box& other) const {
    // Project other onto this
    if (!check_extreme_projection(*this, other)) {
        return false;
    }
    
    if (!check_extreme_projection(other, *this)) {
        return false;
    }
    return true;
}

bool Rectangle::intersects(const Rectangle& other) const {
    Box this_box = this->box();
    Box other_box = other.box();

    return this_box.intersects(other_box);
}

}  // namespace vl