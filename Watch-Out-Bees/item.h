#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

enum item_type {
	HEAL,// 0
	DELAY// 1
};

class Item {
public:
	RectangleShape sprite;
	Item(int delay, int is_presented, long presented_time, item_type type)
		: delay_(delay), is_presented_(is_presented), presented_time_(presented_time), type_(type) {}
	// getter
	int get_delay() {
		return delay_;
	}
	int get_is_presented() {
		return is_presented_;
	}
	long get_presented_time() {
		return presented_time_;
	}
	item_type get_type() {
		return type_;
	}
	// setter
	void set_delay(int delay) {
		delay_ = delay;
	}
	void set_is_presented(int is_presented) {
		is_presented_ = is_presented;
	}
	void set_presented_time(long presented_time) {
		presented_time_ = presented_time;
	}
	void set_type(item_type type) {
		type_ = type;
	}
private:
	int delay_;// 쿨타임
	int is_presented_;//아이템이 나타났는가
	long presented_time_;//나타난 시간
	enum item_type type_;// 아이템 타입
};