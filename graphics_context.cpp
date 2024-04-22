#include "graphics_context.h"
#include "geometry.h"
#include <chrono>
namespace vl {

int Tag::next_value = 0;
Tag no_tag = Tag::next();
Tag PhysicsEntity::physics_tag = Tag::next();
static bool window_exists = false;
static std::vector<Entity*> entities;

Image create_blank(int width, int height, Color initial) {
    Color* data = (Color*)malloc(width * height * sizeof(Color));
    
    for (int i = 0; i < width * height; i++) {
        data[i] = initial;
    }

    // PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 is the same layout as color.
    Image img = Image {data, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    return img;
}

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
        
        input.run_input_handler(time_ms);

        scene.draw(*this);
        scene.run(*this);
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

void Scene::run(GraphicsContext& ctx) {
    for (auto& entity : entities) {
        for (auto& component : entity->components) {
            component->update_time(ctx);
            component->run(ctx);
        }
    }
}

void Scene::draw(GraphicsContext& ctx) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    for (auto& entity : entities) {
        draw_texture_centered(entity->surf, entity->transform.translation + entity->center, (entity->transform.rot_rad * 180) / PI,
                    entity->center, entity->transform.scale,
                    Color {255, 255, 255, 255});
// #ifdef NDEBUG
        PhysicsEntity *phys_entity = dynamic_cast<PhysicsEntity*>(entity);
        if (phys_entity != nullptr) {
            draw_arrow(entity->transform.translation, phys_entity->vel);
        }
// #endif

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

bool Entity::has_tag(const Tag& t) {
    for (const auto& tag : tags) {
        if (tag.value == t.value) {
            return true;
        }
    }
    return false;
}

bool Entity::has_enabled_tag(const Tag& t) {
    for (const auto& tag : tags) {
        if (tag.value == t.value) {
            if (!tag.enabled) {
                return false;
            }
            return true;
        }
    }
    return false;
}
void Entity::add_tag(const Tag& t) {
    tags.push_back(t);
}

Tag Entity::find_tag(const Tag& t) {
    for (const auto& tag : tags) {
        if (tag.value == t.value) {   
            return tag;
        }
    }
    return no_tag;
}

bool PhysicsEntity::intersects(const PhysicsEntity& other) const {
    return other.get_polygon().intersects(this->get_polygon());
}

void PhysicsEntity::Physics::run(vl::GraphicsContext& ctx) {

    PhysicsEntity* phys_entity = dynamic_cast<PhysicsEntity*>(entity);
    if (!phys_entity->floating) {
        // Gravity
        phys_entity->vel.y += 1000 * dt_s;
    }

    if (!phys_entity->floating) {
        for (Entity* other : scene.get_entities()) {
            if (other == entity) {
                continue;
            }
            PhysicsEntity *phys_other = dynamic_cast<PhysicsEntity*>(other);
            if (phys_other != nullptr) {
                if (phys_entity->intersects(*phys_other)) {
                    phys_entity->handle_collision(phys_other);
                }
            }
        }
    }
    entity->transform.translation.x += phys_entity->vel.x * dt_s;
    entity->transform.translation.y += phys_entity->vel.y * dt_s;
    entity->transform.rot_rad += phys_entity->vtheta * dt_s;
}

void draw_arrow(Vector2 v, Vector2 direction) {
    DrawLine(v.x, v.y,
              v.x + direction.x, v.y + direction.y,
              Color {255, 0, 0, 255});
    DrawTriangle(v + direction,
                 v + direction - (Vector2Normalize(direction) * 10) + (normal(direction) * 10),
                 v + direction - (Vector2Normalize(direction) * 10) - (normal(direction) * 10),
                 Color {255, 0, 0, 255});
}


}  // namespace vl


void draw_texture_centered(Texture2D texture, Vector2 position, float rotation, Vector2 rotCenter, float scale, Color tint) {
    Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle dest = { position.x, position.y, (float)texture.width*scale, (float)texture.height*scale };
    Vector2 origin = rotCenter;

    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}
