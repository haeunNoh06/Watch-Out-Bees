#pragma once
#include <SFML/Audio.hpp>//SoundBuffer ���
#include <SFML/Audio/SoundSource.hpp>

using namespace sf;

struct SBuffers {
	SoundBuffer BGM;
	SoundBuffer rumble;
	SoundBuffer item;
	SoundBuffer gameover;
};