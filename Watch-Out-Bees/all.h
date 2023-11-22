#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>//SoundBuffer ���
#include <SFML/Audio/SoundSource.hpp>
#include <Windows.h>
#include "player.h"
#include "bee.h"
#include "weapon.h"
#include "textures.h"
#include "item.h"
#include "sbuffers.h"
#include "gameover.h"
//#include <Windows.h>// sleep() ���

using namespace sf;

//obj1�� obj2�� �浹 ���� �浹�ϸ� 1�� ��ȯ �ƴϸ� 0���� ��ȯ
int is_collide(RectangleShape obj1, RectangleShape obj2) {
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

// �����̴� �Լ�
void moving(Player* player)
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		player->sprite.move(-1 * player->get_speed(), 0);//���� �̵�
	}
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		player->sprite.move(0, -1 * player->get_speed());//���� �̵�
	}
	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		player->sprite.move(0, player->get_speed());//�Ʒ��� �̵�
	}
	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		player->sprite.move(player->get_speed(), 0);//������ �̵�
	}
}

// �������� - const�� ó���Ͽ� �߰��� ���� �ٲ� �� ���� �͸� ���������� ����
const int BEE_NUM = 10;// ���� �ִ� ����
const int WEAPON_NUM = 50;// �Ѿ� �ִ� ����
const int ITEM_NUM = 2;// �������� �ִ� ����
const int W_WIDTH = 1280, W_HEIGHT = 575;// â�� ũ��
const int GO_WIDTH = 880, GO_HEIGHT = 468;// ���ӿ��� �׸��� ũ��