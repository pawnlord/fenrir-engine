#include "graphics_context.h"
#include <chrono>
namespace vl {

static bool window_exists = false;
int GraphicsContext::get_id() {
    
    assert(GraphicsContext::current_id != NULL);
    int id = *GraphicsContext::current_id;
    *GraphicsContext::current_id++;

    return id;
}

GraphicsContext::GraphicsContext(int width, int height, std::string name, int fps) {
    if (window_exists) {
        throw new std::exception("vl: Window already exists");
    }
    screen_width = width;
    screen_height = height;
    InitWindow(screen_width, screen_height, "raylib [core] example - basic window");

    SetTargetFPS(144);

    window_exists = true;
    current_id = &id_counter;
}

void GraphicsContext::run() {
    while (!WindowShouldClose()) {
        time_s = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()/1000;
    }

    CloseWindow();
}
float GraphicsContext::get_time_s() {
    return time_s;
}


}  // namespace vl