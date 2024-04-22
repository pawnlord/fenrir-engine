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
    MapResult add_functionality(std::string name, std::function<void(void)> functionality) {
        if (functionalities.find(name) != functionalities.end()) {
            return MapResult::FunctionalityExists;
        }
        functionalities.emplace(name, Func(KEY_NULL, functionality));
        return MapResult::Ok;
    }

    MapResult map_button(KeyboardKey key, KeyState activation_state, std::string func_name) {
        auto func_pair = functionalities.find(func_name);
        if (func_pair == functionalities.end()) {
            return MapResult::FunctionalityDoesntExist;
        }

        auto& func = func_pair->second;
        if (func.first != KEY_NULL) {
            return MapResult::FuncAlreadyMapped;
        }
        if(inputs.find(key) != inputs.end()) {
            return MapResult::KeyAlreadyMapped;
        }

        func.first = key;
        inputs.emplace(key, InputInformation {key, activation_state, func.second, func_name});

        return MapResult::Ok;
    }

    void set_button(KeyboardKey key, KeyState activation_state, std::string func_name) {
        MapResult res = map_button(key, activation_state, func_name);
        if (res == MapResult::FunctionalityDoesntExist) {
            throw new std::exception(("Functionality " + func_name + "  does not exist.").c_str());
        }
        
        if (res == MapResult::FuncAlreadyMapped) {
            unmap_function(func_name);
            res = map_button(key, activation_state, func_name);
        }
        
        if (res == MapResult::KeyAlreadyMapped) {
            unmap_button(key);
            map_button(key, activation_state, func_name);
        }
    }

    void unmap_button(KeyboardKey key) {
        InputInformation info = inputs.at(key);
        inputs.erase(key);
        functionalities.at(info.function_name).first = KEY_NULL;
    }

    void unmap_function(std::string func_name) {
        auto& func_pair = functionalities.at(func_name);
        inputs.erase(func_pair.first);
        func_pair.first = KEY_NULL;
    }
};

}