#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>//SoundBuffer 사용
#include <SFML/Audio/SoundSource.hpp>
#include <Windows.h>
#include "player.h"
#include "bee.h"
#include "weapon.h"
#include "textures.h"
#include "item.h"
#include "sbuffers.h"
#include "gameover.h"
//#include <Windows.h>// sleep() 사용

using namespace sf;

//obj1과 obj2의 충돌 여부 충돌하면 1로 반환 아니면 0으로 반환
int is_collide(RectangleShape obj1, RectangleShape obj2) {
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

// 움직이는 함수
void moving(Player* player)
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		player->sprite.move(-1 * player->get_speed(), 0);//왼쪽 이동
	}
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		player->sprite.move(0, -1 * player->get_speed());//위쪽 이동
	}
	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		player->sprite.move(0, player->get_speed());//아래쪽 이동
	}
	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		player->sprite.move(player->get_speed(), 0);//오른쪽 이동
	}
}

// 전역변수 - const로 처리하여 중간에 값을 바꿀 수 없는 것만 전역변수로 세팅
const int BEE_NUM = 10;// 적의 최대 갯수
const int WEAPON_NUM = 50;// 총알 최대 갯수
const int ITEM_NUM = 2;// 아이템의 최대 종류
const int W_WIDTH = 1280, W_HEIGHT = 575;// 창의 크기
const int GO_WIDTH = 880, GO_HEIGHT = 468;// 게임오버 그림의 크기