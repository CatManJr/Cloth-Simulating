#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "particle.h"
#include "constraint.h"
#include "input_handler.h"
constexpr int HEIGHT = 640;
constexpr int WIDTH = 1080;
constexpr float PARTICLE_R = 10.0f;
constexpr float GRAVITY = 10.0f;
constexpr float TIME_STEP = 0.1f;

constexpr int ROW = 100;
constexpr int COL = 150;
constexpr float REST_DISTANCE = 5.0f;

float start_x = 0.0f;
float start_y = 0.0f;
sf::Vector2i mousePos;
sf::Vector2u windowSize;

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cloth Simulation");
    window.setFramerateLimit(120);

    std::vector<Particle> particles;
    std::vector<Constraint> constraints;

    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            float x = col * REST_DISTANCE + 100;
            float y = row * REST_DISTANCE + 10;
            bool pinned = (row == 0);
            particles.emplace_back(x, y, pinned);
        }
    }

    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            if (col < COL - 1) {
                constraints.emplace_back(&particles[row * COL + col],
                                            &particles[row * COL + col + 1]);
            }
            if (row < ROW - 1) {
                constraints.emplace_back(&particles[row * COL + col],
                                            &particles[(row + 1) * COL + col]);
            }
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // handle mouse clicks
            InputHandler::handleMouseClick(event, particles, constraints);
            InputHandler::handleMouseDrag(event, particles, constraints, start_x, start_y);
        }

        window.clear(sf::Color::Black);

        // Apply gravity
        for (auto& particle : particles) {
            particle.applyForce(sf::Vector2f(0, GRAVITY));
            particle.update(TIME_STEP);
            particle.constrain_to_bounds(WIDTH, HEIGHT);
        }

        for (size_t i = 0; i < 5; i++) {
            for (auto& constraint : constraints) {
                constraint.satisfy();
            }
        }

        //Draw particles as balls
        /*
        for (const auto& particle : particles) {
            sf::CircleShape circle(PARTICLE_R);
            circle.setFillColor(sf::Color::White);
            circle.setPosition(particle.pos.x - PARTICLE_R,
                            particle.pos.y - PARTICLE_R);
            window.draw(circle);
        }
        */
        //Draw particles as points
        for (const auto& particle : particles) {
            sf::Vertex point(particle.pos, sf::Color::White);
            window.draw(&point, 1, sf::Points);
        }

        //Draw constraints as lines
        for (const auto& constraint : constraints) {
            if (!constraint.active) {
                continue;
            }
            sf::Vertex line[] = {
                sf::Vertex(constraint.p1->pos, sf::Color::White),
                sf::Vertex(constraint.p2->pos, sf::Color::White),
            };
            window.draw(line, 2, sf::Lines);
        }
        window.display();
    }
}
