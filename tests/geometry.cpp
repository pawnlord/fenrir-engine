#include <iostream>
#include <vector>
#include "geometry.h"

#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(IntersectionTest, BasicAssertions) {
    vl::Box box1 {
        Vector2{11.981, 82.680},
        Vector2{98.020, 40.228},
        Vector2{40.733, 140.935},
        Vector2{126.772, 98.502}
    };
    vl::Box box2 {
        Vector2{53.794, 89.545},
        Vector2{139.832, 47.093},
        Vector2{82.545, 147.818},
        Vector2{168.584, 105.367}
    };

    vl::Box box3 {
        Vector2{103.013, 142.082},
        Vector2{167.170, 213.418},
        Vector2{54.689,185.534},
        Vector2{118, 256.870}
    };
    
    vl::Box box4 {
        Vector2{-2.084,100.505},
        Vector2{-66.240, 29.169},
        Vector2{46.231, 57.053},
        Vector2{-17.925, -14.283}
    };

    ASSERT_TRUE(box1.intersects(box2));
    ASSERT_TRUE(box2.intersects(box1));

    ASSERT_TRUE(box1.intersects(box4));
    ASSERT_TRUE(box4.intersects(box1));
    
    ASSERT_FALSE(box1.intersects(box3));
    ASSERT_FALSE(box3.intersects(box1));

    ASSERT_FALSE(box2.intersects(box3));
    ASSERT_FALSE(box3.intersects(box2));

    
    ASSERT_FALSE(box2.intersects(box4));
    ASSERT_FALSE(box4.intersects(box2));
}