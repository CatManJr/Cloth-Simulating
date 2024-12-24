//
// Created by Makoto Xu on 2024/12/14.
//

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "particle.h"
#include "constraint.h"

constexpr float CLICK_TOLERANCE = 2.0f;
constexpr float DRAG_TOLERANCE = 15.0f;

class InputHandler {
public:
    static void handleMouseClick(const sf::Event& event, std::vector<Particle>& particles,
                                std::vector<Constraint>& constraints) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            float mouse_x = static_cast<float>(event.mouseButton.x);
            float mouse_y = static_cast<float>(event.mouseButton.y);
            tear_cloth(mouse_x, mouse_y, particles, constraints); // 删除最近的约束
        }
    }

    static void handleMouseDrag(const sf::Event& event, std::vector<Particle>& particles,
                               std::vector<Constraint>& constraints, float& start_x, float& start_y) {
        if (event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            float mouse_x = static_cast<float>(event.mouseMove.x);
            float mouse_y = static_cast<float>(event.mouseMove.y);
            float distance = std::sqrt((mouse_x - start_x) * (mouse_x - start_x) + (mouse_y - start_y) * (mouse_y - start_y));
            if (distance > DRAG_TOLERANCE) {
                drag_cloth(start_x, start_y, mouse_x, mouse_y, particles, constraints); // 删除路径上的所有约束和粒子
            }
        }
    }
private:
    static float point_to_segment_distance(float px, float py, float x1, float y1, float x2, float y2) {
        float ABx = x2 - x1;
        float ABy = y2 - y1;

        float APx = px - x1;
        float APy = py - y1;

        float BPx = px - x2;
        float BPy = py - y2;

        float AB_AP = ABx * APx + ABy * APy;
        float AB_AB = ABx * ABx + ABy * ABy;
        float t = AB_AP / AB_AB;

        // Project point P on the line segment AB
        if (t < 0.0f) {
            // P is closer to A
            return std::sqrt(APx * APx + APy * APy);
        }
        else if (t > 1.0f) {
            // P is closer to B
            return std::sqrt(BPx * BPx + BPy * BPy);
        }
        else {
            // projection point is on the segment
            float proj_x = x1 + t * ABx;
            float proj_y = y1 + t * ABy;
            return std::sqrt((px - proj_x) * (px - proj_x) + (py - proj_y) * (py - proj_y));
        }

    }

    static Constraint* find_nearest_constraint(float mouse_x, float mouse_y,
                                        const std::vector<Constraint>& constraints) {
        Constraint *nearest_constraint = nullptr;
        float min_distance = CLICK_TOLERANCE;

        for (const auto& constraint : constraints) {
            float distance = point_to_segment_distance(mouse_x, mouse_y,
                             constraint.p1->pos.x, constraint.p1->pos.y,
                             constraint.p2->pos.x, constraint.p2->pos.y);
            if (distance < min_distance) {
                min_distance = distance;
                nearest_constraint = const_cast<Constraint*>(&constraint);
            }
        }
        return nearest_constraint;
    }

    static void tear_cloth(float mouse_x, float mouse_y, std::vector<Particle>& particles,
                            std::vector<Constraint>& constraints) {
        if (Constraint *nearest = find_nearest_constraint(mouse_x, mouse_y, constraints))
            nearest->deactivate();
    }

    static void drag_cloth(float start_x, float start_y, float end_x, float end_y,
                       std::vector<Particle>& particles, std::vector<Constraint>& constraints) {
        // 删除路径上的所有约束
        for (auto it = constraints.begin(); it != constraints.end(); ) {
            if (point_to_segment_distance(start_x, start_y, it->p1->pos.x, it->p1->pos.y, it->p2->pos.x, it->p2->pos.y) < CLICK_TOLERANCE ||
                point_to_segment_distance(end_x, end_y, it->p1->pos.x, it->p1->pos.y, it->p2->pos.x, it->p2->pos.y) < CLICK_TOLERANCE) {
                it = constraints.erase(it);
                } else {
                    ++it;
                }
        }

        // 删除路径上的所有粒子
        for (auto it = particles.begin(); it != particles.end(); ) {
            if (point_to_segment_distance(start_x, start_y, it->pos.x, it->pos.y, end_x, end_y) < CLICK_TOLERANCE) {
                it = particles.erase(it);
            } else {
                ++it;
            }
        }
    }
};

#endif //INPUT_HANDLER_H
