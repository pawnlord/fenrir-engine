#include "input.h"

namespace vl {

using MapResult = InputManager::MapResult;

MapResult InputManager::add_functionality(std::string name, std::function<void(void)> functionality) {
    if (functionalities.find(name) != functionalities.end()) {
        return MapResult::FunctionalityExists;
    }
    functionalities.emplace(name, Func(KEY_NULL, functionality));
    return MapResult::Ok;
}

MapResult InputManager::map_button(KeyboardKey key, KeyState activation_state, std::string func_name) {
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


void InputManager::set_button(KeyboardKey key, KeyState activation_state, std::string func_name) {
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



void InputManager::unmap_button(KeyboardKey key) {
    InputInformation info = inputs.at(key);
    inputs.erase(key);
    functionalities.at(info.function_name).first = KEY_NULL;
}

void InputManager::unmap_function(std::string func_name) {
    auto& func_pair = functionalities.at(func_name);
    inputs.erase(func_pair.first);
    func_pair.first = KEY_NULL;
}
} // namespace vl