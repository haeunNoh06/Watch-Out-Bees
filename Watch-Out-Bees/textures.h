#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

struct Textures {
	Texture background; // 배경 이미지
	Texture bee;// 벌 이미지
	Texture gameover;// 게임 오버 이미지
	Texture item_delay;// 파리채 발사 속도 아이템 이미지
	Texture item_heal;// 생명 증가 아이템 이미지
	Texture player;// 플레이어 이미지
	Texture weapon;// 총알 이미지
};