#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
public:
	RectangleShape sprite;// sprite : �׸��� �Ǵ� �κ�
	Player() {}
	Player(int speed, int speed_max, int score, int life) : speed_(speed), speed_max_(speed_max), score_(score), life_(life) {}
	// getter
	int get_speed() {
		return speed_;
	}
	int get_speed_max() {
		return speed_max_;
	}
	int get_score() {
		return score_;
	}
	int get_life() {
		return life_;
	}
	float get_x() {
		return x_;
	}
	float get_y() {
		return y_;
	}
	// setter
	void set_speed(int speed) {
		speed_ = speed;
	}
	void set_speed_max(int speed_max) {
		speed_max_ = speed_max;
	}
	void set_score(int score) {
		score_ = score;
	}
	void set_life(int life) {
		life_ = life;
	}
	void set_x(float x) {
		x_ = x;
	}
	void set_y(float y) {
		y_ = y;
	}
private:
	int speed_;// �̼�
	int speed_max_;// �̼� �ִ�ġ
	int score_;
	int life_;
	float x_, y_;// player ��ǥ
};