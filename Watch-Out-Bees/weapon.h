#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Weapon {
public:
	RectangleShape sprite;
	Weapon(int is_fired) : is_fired_(is_fired) {}
	// getter
	int get_is_fired() {
		return is_fired_;
	}
	// setter
	void set_is_fired(int is_fired) {
		is_fired_ = is_fired;
	}
private:
	int is_fired_;// 발사 여부
};