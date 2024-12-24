//
// Created by Makoto Xu on 2024/12/14.
//

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <SFML/Graphics.hpp>
#include "particle.h"
#include <cmath>
#include <limits>

class Constraint {
public:
    Particle *p1;
    Particle *p2;
    float initial_length;
    bool active;

    Constraint(Particle *p1, Particle *p2) : p1(p1), p2(p2) {
        initial_length = std::hypot(p2->pos.x - p1->pos.x,
                                    p2->pos.y - p1->pos.y);
        active = true;
    }

    void satisfy() {
        if (!active) {
            return;
        }
        sf::Vector2f delta = p2->pos - p1->pos;
        float current_length = std::hypot(delta.x, delta.y);
        float difference = (current_length - initial_length) / current_length;
        sf::Vector2f correction = delta * 0.5f * difference;

        if (!p1->isPinned) p1->pos += correction;
        if (!p2->isPinned) p2->pos -= correction;
    }

    void deactivate() {
        active = false;
    }

    bool checkCollisionWithBounds(Particle* p1, Particle* p2, float width, float height) {
        sf::Vector2f delta = p2->pos - p1->pos;
        float length = std::hypot(delta.x, delta.y);
        float max_length = std::max(width, height);

        if (length > max_length) {
            return true; // 约束超出了边界，需要调整
        }

        return false;
    }
};

#endif //CONSTRAINT_H
