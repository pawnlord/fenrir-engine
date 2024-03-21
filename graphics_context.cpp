#include "graphics_context.h"
#include <chrono>
namespace vl {

static bool window_exists = false;
static std::vector<Entity*> entities;


GraphicsContext::GraphicsContext(int width, int height, std::string name, int fps) {
    if (window_exists) {
        throw new std::exception("vl: Window already exists");
    }
    screen_width = width;
    screen_height = height;
    InitWindow(screen_width, screen_height, "raylib [core] example - basic window");

    SetTargetFPS(fps);

    window_exists = true;
}

void GraphicsContext::run() {
    scene.init(*this);
    while (!WindowShouldClose()) {
        time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        scene.draw(*this);
    }

    CloseWindow();
}
long long GraphicsContext::get_time_ms() {
    return time_ms;
}

void GraphicsContext::set_scene(Scene scene) {
    this->scene = scene;
}

void Scene::init(GraphicsContext& ctx) {
    for (auto& entity : entities) {
        for (auto& component : entity->components) {
            component->init(ctx);
        }
    }
}

void Scene::draw(GraphicsContext& ctx) {
    for (auto& entity : entities) {
        for (auto& component : entity->components) {
            component->update_time(ctx);
            component->run(ctx);
        }
    }
    BeginDrawing();
    ClearBackground(BLACK);
    
    for (auto& entity : entities) {
        DrawTextureEx(entity->surf, entity->transform.translation,
                    entity->transform.rot_rad,
                    entity->transform.scale,
                    Color {255, 255, 255, 255});
    }   

    EndDrawing();
}

void Scene::add_entity(Entity* entity) {
    entities.push_back(entity);
}

void Entity::add_component(std::unique_ptr<Component> component) {
    component->set_entity(this);
    this->components.push_back(std::move(component));
}


}  // namespace vl