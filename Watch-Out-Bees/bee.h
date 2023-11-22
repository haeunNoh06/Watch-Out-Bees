#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bee {
public:
	RectangleShape sprite;
	Bee(int speed, int life) : speed_(speed), life_(life) {}
	// getter
	int get_speed() {
		return speed_;
	}
	int get_life() {
		return life_;
	}
	// setter
	void set_speed(int speed) {
		speed_ = speed;
	}
	void set_life(int life) {
		life_ = life;
	}
private:
	int speed_;
	int life_;
};