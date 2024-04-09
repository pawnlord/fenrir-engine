#include "geometry.h"

namespace vl {


// Is q in the box with diagonal line p and r
bool in_diagonals_box(Vector2 q, Vector2 p, Vector2 r)  { 
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && 
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) 
       return true; 
  
    return false; 
} 

Orientation orientation(Vector2 p, Vector2 q, Vector2 r)  { 
    // Slopes multiplied by corresponding denominators

    // (y2 - y1)/(x2 - x1) < (y3 - y2)/(x3 - x2) => (y2 - y1)*(x3 - x2) < (y3 - y2)*(x2 - x1)
    int slope_pq = (q.y - p.y) * (r.x - q.x);
    int slope_qr = (r.y - q.y) * (q.x - p.x); 
  
    if (slope_pq == slope_qr) return Orientation::Colinear; 
    
    if (slope_pq > slope_qr) {
        return Orientation::CW;
    }
    return Orientation::CCW; 
} 

bool line_intersects(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2, bool colinear_intersection = true) {
    Orientation o1 = orientation(p1, q1, p2); 
    Orientation o2 = orientation(p1, q1, q2); 
    Orientation o3 = orientation(p2, q2, p1); 
    Orientation o4 = orientation(p2, q2, q1); 
    
    if (o1 != o2 && o3 != o4) {
        return true;
    }
    
    // Colinear checking is expesive, and not very important for some polygons.
    if (colinear_intersection) {
        return (o1 == Orientation::Colinear && in_diagonals_box(p1, p2, q1))
                || (o2 == Orientation::Colinear && in_diagonals_box(p1, q2, q1))
                || (o3 == Orientation::Colinear && in_diagonals_box(p2, p1, q2))
                || (o4 == Orientation::Colinear && in_diagonals_box(p2, q1, q2));
    } else {
        return false;
    }
}


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

bool Polygon::intersects(const Polygon& other) const {
    if (this->points.size() == 4 && other.points.size() == 4) {
        Box this_box {
            this->points[0],
            this->points[1],
            this->points[3],
            this->points[2],
        };
        Box other_box {
            other.points[0],
            other.points[1],
            other.points[3],
            other.points[2],
        };
        return this_box.intersects(other_box);
    }
    for (int i = 0; i < this->points.size() - 1; i++) { 
        for (int j = 0; j < other.points.size() - 1; j++) {
            if (line_intersects(this->points[i], this->points[i+1],
                                other.points[i], other.points[i+1])) {
                return true;
            }
        }
    }
    return false;
}

bool Rectangle::intersects(const Rectangle& other) const {
    Polygon this_box = this->box();
    Polygon other_box = other.box();

    return this_box.intersects(other_box);
}

Vector2 Rectangle::get_normal(Vector2 v, Vector2 direction) const {
    Polygon box = this->box();

    // Get counterclockwise lines
    std::vector<std::pair<Vector2, Vector2>> lines;
    for (int i = 0; i < box.points.size() - 1; i++) {
        lines.push_back(std::pair<Vector2, Vector2>(box.points[i], box.points[i + 1]));
    }

    while (true) {
        std::vector<std::pair<Vector2, Vector2>> new_lines;
        for (const auto& line : lines) {
            if (orientation(line.first, line.second, v) == Orientation::CW) {
                new_lines.push_back(line);
            }
        }
        if (new_lines.size() == 1) {
            Vector2 parallel = new_lines[0].second - new_lines[0].first;
            return normal(parallel);
        }

        if (new_lines.size() == 0) {
            Vector2 normal_vec = Vector2 {0, 0};
            for (const auto& line : lines) {
                Vector2 parallel = line.second - line.first;
                normal_vec = normal_vec + normal(parallel);
            }
            return Vector2Normalize(normal_vec);
        }
        v = v + direction;
        lines = new_lines;
    }

}

}  // namespace vl