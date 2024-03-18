#pragma once
#include <cassert>
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include "raylib.h"

namespace vl {

class GraphicsContext {
    static uint64_t* current_id;
    static int get_id();

   public:
    GraphicsContext(int width, int height, std::string name, int fps = 144);
    void run();
    float screen_width;
    float screen_height;
    float time_s = 0;
    float get_time_s();
    uint64_t id_counter = 0;

   private:
};
uint64_t* GraphicsContext::current_id = nullptr;


template <class T>
class Entity {
   public:
    uint64_t id = 0;
    std::shared_ptr<T> value;
    Entity(std::shared_ptr<T> value) : value{value} {
        id = GraphicsContext::get_id(); 
    }
    
    T* operator->() { return value.get(); }

    T& operator*() { return *value; }
};

class Behavior {
   public:
    virtual void init(GraphicsContext& ctx, int id);
    virtual void run(GraphicsContext& ctx, int id);
    void update_time(GraphicsContext& ctx) {
        float time_s = ctx.get_time_s();
        dt_s = time_s - last_time_s;
        last_time_s = time_s;
    }

   protected:
    float last_time_s = 0;
    float dt_s;
};

class Object {
   public:
    std::vector<Entity<Behavior>> behaviors;
    void init(GraphicsContext& ctx) {
        for (auto& b : behaviors) {
            b->init(ctx, b.id);
        }
    }

    void run(GraphicsContext& ctx) {
        for (auto& b : behaviors) {
            b->update_time(ctx);
            b->run(ctx, b.id);
        }
    }
};

}  // namespace vl