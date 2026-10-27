#pragma once
#include <SFML/Graphics.hpp>
#include<cmath>
#include "Monster.cpp"

using namespace std;
//--Lớp đạn--//
class Bullet {
public:
	sf::Vector2f position;
	sf::Vector2f velocity;
	float speed;
	float damage;
	bool isActive;
	Monster* targetMonster;

	Bullet(sf::Vector2f startPos, Monster* target, float bulletSpeed, float dmg) : position(startPos), speed(bulletSpeed), damage(dmg)
	, isActive(true), targetMonster(target){
		updateVelocity();
	}

	void updateVelocity() {
		if (targetMonster && targetMonster ->isAlive) {
			sf::Vector2f dir = targetMonster->position - position;
			float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
			if (dist > 0.f) {
				velocity = (dir / dist) * speed;
			}
		}
	}
	
	void update(float dt, float windowWidth, float windowHeight) {
		if (!isActive) return;

		if (targetMonster && targetMonster->isAlive) {
			sf::Vector2f dir = targetMonster->position - position;
			float distSq = dir.x * dir.x + dir.y * dir.y;
			if (distSq <= (speed * dt) * (speed * dt)) {
				targetMonster->HP -= damage;
				if (targetMonster->HP <= 0.f && targetMonster->isAlive) {
					targetMonster->isAlive = false;
				}
				isActive = false;
				return;
			}
			else {
				float dist = sqrt(distSq);
				velocity = (dir/dist) * speed;
			}
		}
		else {
			if (targetMonster && !targetMonster ->isAlive) {
				targetMonster = nullptr;
			}
		}

		position += velocity * dt;

		if (position.x < 0 || position.x > windowWidth || position.y < 0 || position.y > windowHeight ) {
			isActive = false;
		}
	}

	void draw(sf::RenderWindow& window) const {
		if (!isActive) {
			return;
		}
		sf::CircleShape bulletShape(4.f);
		bulletShape.setOrigin(4.f, 4.f);
		bulletShape.setPosition(position);
		bulletShape.setFillColor(sf::Color::Yellow);
		window.draw(bulletShape);
	}
};