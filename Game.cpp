#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "ResourceManage.cpp"
#include "Monster.cpp"
#include "Tower.cpp"
#include "Bullet.cpp"

using namespace std;
class Game {
private:
    const int WINDOW_WIDTH = 1000;
    const int WINDOW_HEIGHT = 700;

    ResourceManage economy;
    vector<sf::Vector2f> waypoints;
    vector<Monster> monsters;
    vector<Tower> towers;
    vector<Bullet> bullets;

    sf::Clock deltaClock;
    sf::Clock spawnClock;
    bool isGameOver;

    sf::RenderWindow window;
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (isGameOver) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    resetGame();
                }
                continue;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f clickPos(static_cast<float> (event.mouseButton.x), static_cast<float> (event.mouseButton.y));

                    bool canPlace = true;
                    for (size_t i = 0; i < waypoints.size() - 1; i++) {
                        float dx = clickPos.x - waypoints[i].x;
                        float dy = clickPos.y - waypoints[i].y;

                        if (sqrt(dx * dx + dy * dy) < 30.f) {
                            canPlace = false;
                            break;
                        }
                    }
                    if (canPlace && economy.spendGold(100)) {
                        towers.emplace_back(clickPos);
                    }
                }
            }
        }
    }

    void update() {
        float dt = deltaClock.restart().asSeconds();
        if (isGameOver) return;

        economy.update(dt);

        float currentSpawm = max(0.5f, 2.0f - (economy.getScore()/ 150.f) * 0.25f);
        if (spawnClock.getElapsedTime().asSeconds() >= currentSpawm) {
            int spawnBatch = 1 + (economy.getScore() / 120);
            float hpBonus = (economy.getScore() / 100.f) * 20.f;
            float speedBonus = (economy.getScore() / 250.f) * 6.f;

            for (int i = 0; i < spawnBatch; i++) {
                monsters.emplace_back(waypoints, hpBonus, speedBonus);
            }
            spawnClock.restart();
        }

        for (auto& m : monsters) {
            m.update(dt, waypoints);
        }

        for (auto& t : towers) {
            t.update(monsters, bullets);
        }

        for (auto& b : bullets) {
            b.update(dt, static_cast<float> (WINDOW_WIDTH), static_cast<float> (WINDOW_HEIGHT));
        }

        for (auto& m : monsters) {
            if (!m.isAlive && !m.isRewarded && m.HP <= 0) {
                economy.addGold(30);
                economy.addScore(15);
                m.isRewarded = true;
            }
        }

        monsters.erase(
            remove_if(monsters.begin(), monsters.end(), [](const Monster& m) {return !m.isAlive; }),
            monsters.end()
        );

        bullets.erase(
            remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {return !b.isActive; }),
            bullets.end()
        );
       

        for (auto& m : monsters) {
            if (m.isAlive && m.hasReachedEnd(waypoints.size())) {
                economy.takeDamage(50);
                m.isAlive = false;
            }
        }

        if (economy.getPlayerHP() <= 0) {
            isGameOver = true;
        }

        updateWindowTitle();
    }

    void resetGame() {
        economy.reset(300, 50, 100);
        isGameOver = false;
        bullets.clear();
        monsters.clear();
        towers.clear();
        spawnClock.restart();
        deltaClock.restart();
    }

    void updateWindowTitle() {
        if (!isGameOver) {
            string title = "Tower Defense | HP: " + to_string(economy.getPlayerHP())
                + " | Score: " + to_string(economy.getScore()) +
                " | Gold: " + to_string(economy.getGold()) +
                " | Energy: " + to_string(economy.getEnergy())
                ;
            window.setTitle(title);
        }
        else {
            window.setTitle("GAME OVER! Final Score: " + to_string(economy.getScore()) + " - Press 'R' to restart");
        }
    }

    void render() {
        window.clear(sf::Color(20,20,25));

        for (size_t i = 0; i < waypoints.size() - 1; i++) {
            sf::Vertex line[] = {
                sf::Vertex(waypoints[i], sf::Color(60,60,75)),
                sf::Vertex(waypoints[i + 1], sf::Color(60,60,75))
            };
            window.draw(line, 2, sf::Lines);
        }

        for (auto& m : monsters) {
            m.draw(window);
        }
        for (auto& t : towers) {
            t.draw(window);
        }
        for (auto& b : bullets) {
            b.draw(window);
        }
        window.display();
    }
public:
    Game() :window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense"), economy(300, 50, 100), isGameOver(false){
        window.setFramerateLimit(60);

        waypoints = {
            {50, 100},
            {350, 100},
            {350, 350},
            {650, 350},
            {650, 150},
            {850, 150},
            {850, 550},
            {950, 550},
        };
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }
};