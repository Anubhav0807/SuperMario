#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Block.h"
#include "Player.h"
#include "Animator.h"
#include "Goomba.h"
#include "Global.h"

class MapManager
{
private:
	bool deathMusicPlaying;

public:
	enum Entity {
		NONE,
		BLOCK,
		PLAYER,
		ENEMY_L,
		ENEMY_R,
	};

	std::vector<std::vector<int>> mapMatrix;
	sf::Music themeMusic;

	std::vector<Goomba*> goombaVect;
	Frames goombaFrames;
	sf::Texture goombaTexture;

	Player* player;
	Frames playerFrames;
	sf::Texture playerTexture;

	std::vector<Block*> blockVect;
	Block* bouncingBlock;
	sf::Texture blockTexture;

	MapManager();
	~MapManager();
	void update(float deltaTime, float center_x);
	void draw(sf::RenderWindow& window);
	void restart();
	void setPosition();
	void toggleHitbox();
};

