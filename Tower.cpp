#pragma once
#include <SFML/Graphics.hpp>
#include<cmath>
#include<vector>
#include "Monster.cpp"
#include "Bullet.cpp"
using namespace std;
//--Lớp tháp phòng thủ--//
class Tower {
private:
	sf::Vector2f position;
	float range;
	float damage;
	float fireRate;
	sf::Clock fireClock;

public:
	Tower(sf::Vector2f pos) : position(pos), range(150.f), damage(30.f), fireRate(2.5f){}

	void update(vector<Monster>& monsters, vector<Bullet>& bullets ) {
		if (fireClock.getElapsedTime().asSeconds() < (1.0f/ fireRate)) {
			return;
		}

		Monster* target = nullptr;
		float minDstSq = range * range;

		for (auto&m : monsters) {
			if (!m.isAlive) {
				continue;
			}
			float dx = m.position.x - position.x;
			float dy = m.position.y - position.y;

			float dstSq = dx * dx + dy * dy;

			if (dstSq <= minDstSq) {
				minDstSq = dstSq;
				target = &m;
			}
		}
		if (target != nullptr) {
			bullets.emplace_back(position, target, 480.f, damage);
			fireClock.restart();
		}
	}

	void draw(sf::RenderWindow& window) const {
		// Vẽ radar tầm bắn
		sf::CircleShape rangeCircle(range);
		rangeCircle.setOrigin(range, range);
		rangeCircle.setPosition(position);
		rangeCircle.setFillColor(sf::Color(255, 255, 255, 10));
		
		rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 45));
		rangeCircle.setOutlineThickness(1.f);
		window.draw(rangeCircle);

		//Vẽ tháp
		sf::CircleShape towerShape(16.f);
		towerShape.setOrigin(16.f, 16.f);
		towerShape.setPosition(position);
		towerShape.setFillColor(sf::Color::Cyan);
		window.draw(towerShape);
	}
};