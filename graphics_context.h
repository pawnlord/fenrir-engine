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
struct Polygon;

Image create_blank(int width, int height, Color initial = Color{0, 0, 0, 255});


class Scene {
   public:
    void add_entity(Entity* entity);
    void run(GraphicsContext& ctx);
    void draw(GraphicsContext& ctx);
    void init(GraphicsContext& ctx);
    const std::vector<Entity*>& get_entities() {
        return entities;
    }
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
        if (last_time_ms == 0) {
            dt_s = 0.016;
        } else {
            dt_s = (time_ms - last_time_ms) / 1000.0;
        }
        last_time_ms = time_ms;
    }
    void set_entity(Entity* entity) {
        this->entity = entity;
    }
   protected:
    long long last_time_ms = 0;
    float dt_s;
    Scene& scene;
    Entity* entity;
};

struct Tag {
    static int next_value;
    Tag() = delete;
    static const Tag next() {
        Tag tag{next_value};
        next_value++;
        return tag;
    }

    Tag(const Tag& tag) {
        value = tag.value;
    }
    void disable() {
        enabled = false;
    }
    void enable() {
        enabled = true;
    }
    void toggle() {
        enabled = !enabled;
    }
    bool operator==(const Tag& other) {
        return this->value == other.value;
    }
    int value;
    bool enabled = true;

   private:
    Tag(int value) {
        this->value = value;
    }
};

extern Tag no_tag;

class Entity {
   public:
    Entity(Scene& scene) : scene{scene} {
        scene.add_entity(this);
    };
    virtual ~Entity() = default;
    Transform2 transform;   
    void add_component(std::unique_ptr<Component> component);
    // TODO: Should this be unique?
    std::vector<std::unique_ptr<Component>> components;
    Texture2D surf;
    // Center, so that entities can determine were they rotate and draw from
    Vector2 center = Vector2{0.0, 0.0};
    std::vector<Tag> tags;
   
    bool has_tag(const Tag& t);
    bool has_enabled_tag(const Tag& t);
    void add_tag(const Tag& t);
    Tag find_tag(const Tag& t);

   protected:
    Scene& scene;
};


class PhysicsEntity : public Entity {
   public:
    PhysicsEntity(Scene& scene) : Entity{scene} {
        add_component(std::make_unique<Physics>(scene));
    }

    virtual Polygon get_polygon() const = 0;
    virtual Vector2 get_normal(Vector2 v, Vector2 direction) const = 0;
    
    virtual void handle_collision(PhysicsEntity* phys_other) {
        Vector2 normal = phys_other->get_normal(this->transform.translation, this->vel);
        while (this->intersects(*phys_other)) {
            this->transform.translation = this->transform.translation + (normal * 0.01); 
        }
        this->vel = this->vel - (normal * dot(this->vel, normal) * this->bounce);    
    }

    bool intersects(const PhysicsEntity& other) const;

    Vector2 vel{0, 0};
    float vtheta = 0.0;

    // Some niche dials to turn
    float bounce = 1.0;

    bool floating = false;
   protected:
    static Tag physics_tag;

    class Physics : public Component {
       public:
        Physics(vl::Scene& scene) : vl::Component{scene} {}
        virtual ~Physics() override = default;

        virtual void init(vl::GraphicsContext& ctx) override {
            entity->add_tag(physics_tag);
        }

        virtual void run(vl::GraphicsContext& ctx) override;
    };
};

void draw_arrow(Vector2 v, Vector2 direction);
}  // namespace vl

// Outside because it uses Raylib primitives which overlap with vl
void draw_texture_centered(Texture2D texture, Vector2 position, float rotation, Vector2 rotCenter, float scale, Color tint);
