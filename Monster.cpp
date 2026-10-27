#pragma once
#include<SFML/Graphics.hpp>
#include <vector>
using namespace std;
//--Lớp Quái vật--//
class Monster {
public:
	sf::Vector2f position;
	float HP;
	float maxHP;
	float speed;
	bool isAlive;
	bool isRewarded;
	size_t currentWayPointIndex;

	Monster(const vector<sf::Vector2f>& wayPoint, float hpBonus = 0.f, float speedBonus = 0.f) {
		if (!wayPoint.empty()) {
			position = wayPoint[0];
		}
		HP = 120.f + hpBonus;
		maxHP = HP;
		speed = 100.f + speedBonus;
		currentWayPointIndex = 1;
		isAlive = true;
		isRewarded = false;
	}

	void update(float dt, const vector<sf::Vector2f>& wayPoint) {
		if (!isAlive || currentWayPointIndex >= wayPoint.size()) return;

		sf::Vector2f target = wayPoint[currentWayPointIndex];
		sf::Vector2f dir = target - position;
		float distSq = dir.x * dir.x + dir.y * dir.y;
		float step = speed * dt;

		if (distSq <= step * step) {
			position = target;
			currentWayPointIndex++;
		}
		else {
			float dist = sqrt(distSq);
			position += (dir / dist) * step;
		}
	}

	bool hasReachedEnd(size_t totalWayPoint) const {
		return currentWayPointIndex >= totalWayPoint;
	}

	void draw(sf::RenderWindow& window) const {
		if (!isAlive) { return; }

		sf::CircleShape monsterShape(10.f);
		monsterShape.setOrigin(10.f, 10.f);
		monsterShape.setPosition(position);
		monsterShape.setFillColor(sf::Color::Red);
		window.draw(monsterShape);

		//Vẽ thanh máu
		sf::RectangleShape heartBarBg(sf::Vector2f(24.f, 4.f));
		heartBarBg.setFillColor(sf::Color::Black);
		heartBarBg.setPosition(position.x - 12.f, position.y - 18.f);
		window.draw(heartBarBg);

		float healthPct = max(0.0f, HP / maxHP);
		sf::RectangleShape healthBar(sf::Vector2f(24.f * healthPct, 4.f));
		healthBar.setFillColor(sf::Color::Green);
		healthBar.setPosition(position.x - 12.f, position.y - 18.f);
		window.draw(healthBar);
	}
};