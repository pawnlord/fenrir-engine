#pragma once
#include <cassert>
#include <functional>
#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include "vlmath.h"
#include "raylib.h"
#include "raymath.h"
namespace vl {

class Entity;
class GraphicsContext;

Image create_blank(int width, int height, Color initial = Color{0, 0, 0, 255});

class Scene {
   public:
    void add_entity(Entity* entity);
    void run(GraphicsContext& ctx);
    void draw(GraphicsContext& ctx);
    void init(GraphicsContext& ctx);
   private:
    std::vector<Entity*> entities;
};

class GraphicsContext {
   public:
    GraphicsContext(int width, int height, std::string name, int fps = 144);
    void run();
    void set_scene(Scene scene);
    float screen_width;
    float screen_height;
    long long time_ms = 0;
    long long get_time_ms();
    uint64_t id_counter = 0;

    Scene scene;
};

class Component {
   public:
    Component(Scene& scene) : scene{scene} {}
    virtual void init(GraphicsContext& ctx) = 0;
    virtual void run(GraphicsContext& ctx) = 0;
    virtual ~Component() = default;

    void update_time(GraphicsContext& ctx) {
        long long time_ms = ctx.get_time_ms();
        dt_s = (time_ms - last_time_ms) / 1000.0;
        last_time_ms = time_ms;
    }
    void set_entity(Entity* entity) {
        this->entity = entity;
    }
   protected:
    long long last_time_ms = 0;
    double dt_s;
    Scene& scene;
    Entity* entity;
};

class Entity {
   public:
    Entity(Scene& scene) : scene{scene} {
        scene.add_entity(this);
    };
    Transform2 transform;
    void add_component(std::unique_ptr<Component> component);
    // TODO: Should this be unique?
    std::vector<std::unique_ptr<Component>> components;
    Texture2D surf;
   private:
    Scene& scene;
};


}  // namespace vl