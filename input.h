#include <raylib.h>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
namespace vl {
class GraphicsContext;

enum KeyState {
    None = 0,
    Up = 1,
    Down = 2,
    Pressed = 4,
    Released = 8 
};

using KeyHandler = std::function<void(KeyState, float)>;

/*
 * Input TODO:
 * - Make serialization/deserialization
 * - Figure out how controllers work
 */

struct InputInformation {
    InputInformation(KeyboardKey key, KeyState activation_state, KeyHandler callback, std::string function_name)
        : key{key}, activation_state{activation_state}, callback{callback}, function_name{function_name} {}

    KeyboardKey key = KeyboardKey::KEY_NULL;
    KeyState activation_state;
    KeyHandler callback;
    std::string function_name;
    
    inline KeyState check_press() {
        if (activation_state & KeyState::Up) {
            if (IsKeyUp(key)) {
                return KeyState::Up;
            }
        } else if (activation_state & KeyState::Down) {
            if (IsKeyDown(key)) {
                return KeyState::Down;
            }
        } else if (activation_state & KeyState::Pressed) {
            if (IsKeyPressed(key)) {
                return KeyState::Pressed;
            }
        } else if (activation_state & KeyState::Released) {
            if (IsKeyReleased(key)) {
                return KeyState::Released;
            }
        }
        return KeyState::None;
    }
};

struct InputManager {
    enum class MapResult { Ok, FunctionalityExists, FunctionalityDoesntExist, FuncAlreadyMapped, KeyAlreadyMapped };
    using Func = std::pair<KeyboardKey, KeyHandler>;
    std::unordered_map<std::string, Func> functionalities;
    std::unordered_map<KeyboardKey, InputInformation> inputs;
    MapResult add_functionality(std::string name, KeyHandler functionality);
    MapResult map_button(KeyboardKey key, KeyState activation_state, std::string func_name);

    void set_button(KeyboardKey key, KeyState activation_state, std::string func_name);

    void unmap_button(KeyboardKey key);

    void unmap_function(std::string func_name);

    void run_input_handler(long long time_ms);

    long long last_time_ms = 0;
    float dt_s;
};

}  // namespace vl