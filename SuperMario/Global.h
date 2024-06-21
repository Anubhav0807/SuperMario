#pragma once
#include <string>

const float SIZE_RATIO = 3.28f;
const bool CAM_FOLLOW = true;
const float GRAVITY = 1400.0f;
const float FRICTION = 400.0f;

// Classic 256x240
const int SCREEN_WIDTH = 896;
const int SCREEN_HEIGHT = 840;

// Assets Path
struct {
	std::string map_img = "../Assets/Sprites/map.png";
	std::string block_img = "../Assets/Sprites/blocks.png";
	std::string player_img = "../Assets/Sprites/mario.png";
	std::string enemy_img = "../Assets/Sprites/enemy.png";
	std::string bg_music = "../Assets/Audio/underground-theme.mp3";
	std::string jump_sound = "../Assets/Audio/mario-jump.wav";
	std::string flat_sound = "../Assets/Audio/flat.wav";
	std::string mario_block_hit_sound = "../Assets/Audio/mario-block-hit.wav";
	std::string mario_die_sound = "../Assets/Audio/mario-die.wav";
} PATH;