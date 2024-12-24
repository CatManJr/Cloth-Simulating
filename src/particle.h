#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

class Particle {
public:
    sf::Vector2f pos;
    sf::Vector2f previous_pos;
    sf::Vector2f acceleration;
    sf::Vector2f velocity; // 用于存储速度
    bool isPinned;
    float damping; // 阻尼系数

    Particle(float x, float y, bool pinned = false, float damping = 1.0f) :
        pos(x, y), previous_pos(x, y), acceleration(0, 0), velocity(0, 0), 
        isPinned(pinned), damping(damping) {}

    void applyForce(const sf::Vector2f& force) {
        if (!isPinned)
            acceleration += force;
    }

    void update(float time_step) {
        // Verlet integration
        if (!isPinned) {
            velocity = pos - previous_pos; // 更新速度
            previous_pos = pos;
            pos += velocity + acceleration * time_step * time_step; // 应用加速度
            acceleration = sf::Vector2f(0, 0); // 重置加速度
            velocity *= damping; // 应用阻尼
        }
    }

    void constrain_to_bounds(float width, float height) {
        if (pos.x < 0 || pos.x > width) {
            pos.x = std::max(0.0f, std::min(pos.x, width));
            velocity.x = -velocity.x * damping; // 反弹并应用阻尼
        }
        if (pos.y < 0 || pos.y > height) {
            pos.y = std::max(0.0f, std::min(pos.y, height));
            velocity.y = -velocity.y * damping; // 反弹并应用阻尼
        }
    }
};

#endif //PARTICLE_H