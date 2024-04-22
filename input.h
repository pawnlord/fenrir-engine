#include <iostream>
#include <raylib.h>
#include <functional>
#include <string>
#include <map>
#include <unordered_map>
namespace vl {
enum class KeyState {
    Up,
    Down,
    Pressed,
    Unpressed
};

/*
 * Input TODO:
 * - Make serialization/deserialization
 * - Figure out how controllers work
 */

struct InputInformation {
    InputInformation(KeyboardKey key, KeyState activation_state, std::function<void(void)> callback, std::string function_name)
        : key{key}, activation_state{activation_state}, callback{callback}, function_name{function_name} {}
    
    KeyboardKey key = KeyboardKey::KEY_NULL;
    KeyState activation_state;
    std::function<void(void)> callback;
    std::string function_name;
};


struct InputManager {
    enum class MapResult {
        Ok,
        FunctionalityExists,
        FunctionalityDoesntExist,
        FuncAlreadyMapped,
        KeyAlreadyMapped
    };
    using Func = std::pair<KeyboardKey, std::function<void(void)>>;
    std::unordered_map<std::string, Func> functionalities;
    std::unordered_map<KeyboardKey, InputInformation> inputs;
    MapResult add_functionality(std::string name, std::function<void(void)> functionality);
    MapResult map_button(KeyboardKey key, KeyState activation_state, std::string func_name);

    void set_button(KeyboardKey key, KeyState activation_state, std::string func_name);

    void unmap_button(KeyboardKey key);

    void unmap_function(std::string func_name);
};

}