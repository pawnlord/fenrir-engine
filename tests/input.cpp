#include <input.h>

#include <raylib.h>
#include <gtest/gtest.h>


TEST(InputTest, BasicAssertions) {
    vl::InputManager im;
    int x = 0;
    int y = 0;

    im.add_functionality("Up", [&](vl::KeyState, float){
        y += 1;
    });
    
    im.add_functionality("Left", [&](vl::KeyState, float){
        x -= 1;
    });

    im.set_button(KEY_W, vl::KeyState::Up, "Up");
    ASSERT_EQ(im.functionalities.at("Up").first, KEY_W);
    ASSERT_EQ(im.inputs.at(KEY_W).function_name, "Up");
    
    im.inputs.at(KEY_W).callback(vl::KeyState::Up, 1);
    ASSERT_EQ(y, 1);

    im.set_button(KEY_W, vl::KeyState::Up, "Left");
    ASSERT_EQ(im.functionalities.at("Up").first, KEY_NULL);
    ASSERT_EQ(im.functionalities.at("Left").first, KEY_W);
    ASSERT_EQ(im.inputs.at(KEY_W).function_name, "Left");

    im.inputs.at(KEY_W).callback(vl::KeyState::Up, 1);
    ASSERT_EQ(x, -1);

    im.set_button(KEY_A, vl::KeyState::Up, "Left");
    ASSERT_EQ(im.inputs.find(KEY_W), im.inputs.end());
    ASSERT_EQ(im.functionalities.at("Left").first, KEY_A);
    ASSERT_EQ(im.inputs.at(KEY_A).function_name, "Left");

}