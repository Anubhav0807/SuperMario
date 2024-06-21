#include "MapManager.h"

MapManager::MapManager()
{
	bouncingBlock = nullptr;
	player = nullptr;
	sf::Image mapImage;
	mapImage.loadFromFile(PATH.map_img);
	deathMusicPlaying = false;

	blockTexture.loadFromFile(PATH.block_img);
	sf::Vector2u blockImageCount(28, 16);
	sf::Vector2f blockSize(blockTexture.getSize().x / float(blockImageCount.x), blockTexture.getSize().y / float(blockImageCount.y));

	goombaFrames.walking = { sf::Vector2u(3, 0), sf::Vector2u(4, 0) };
	goombaFrames.dying = { sf::Vector2u(5, 0) };
	goombaTexture.loadFromFile(PATH.enemy_img);

	playerFrames.idle = { sf::Vector2u(0, 0) };
	playerFrames.walking = { sf::Vector2u(1, 0), sf::Vector2u(2, 0), sf::Vector2u(3, 0) };
	playerFrames.jumping = { sf::Vector2u(5, 0) };
	playerFrames.dragging = { sf::Vector2u(4, 0) };
	playerFrames.dying = { sf::Vector2u(6, 1) };
	playerTexture.loadFromFile(PATH.player_img);
	
	themeMusic.openFromFile(PATH.bg_music);
	themeMusic.setLoop(true);
	themeMusic.play();

	for (int x = 0; x < mapImage.getSize().x; x++) {
		std::vector<int> row;
		for (int y = 0; y < mapImage.getSize().y; y++) {
			if (mapImage.getPixel(x, y) == sf::Color::Black) {
				row.push_back(BLOCK);
			}
			else if (mapImage.getPixel(x, y) == sf::Color::Red) {
				row.push_back(PLAYER);
			}
			else if (mapImage.getPixel(x, y) == sf::Color::Blue) {
				row.push_back(ENEMY_L);
			}
			else if (mapImage.getPixel(x, y) == sf::Color::Cyan) {
				row.push_back(ENEMY_R);
			}
			else {
				row.push_back(NONE);
			}
		}
		mapMatrix.push_back(row);
	}

	for (int i = 0; i < mapMatrix.size(); i++) {
		for (int j = 0; j < mapMatrix[0].size(); j++) {
			if (mapMatrix[i][j] == BLOCK) {
				if (j > 0 && mapMatrix[i][j-1] == BLOCK) {
					blockVect.push_back(new Block(&blockTexture, blockSize, false)); // Can not Bounce
				}
				else {
					blockVect.push_back(new Block(&blockTexture, blockSize, true)); // Can Bounce
				}
			}
			else if (mapMatrix[i][j] == PLAYER) {
				player = new Player(&playerTexture, playerFrames, 120.0f, 69.0f);
			}
			else if (mapMatrix[i][j] == ENEMY_L || mapMatrix[i][j] == ENEMY_R) {
				goombaVect.push_back(new Goomba(&goombaTexture, goombaFrames));
			}
		}
	}

	setPosition();
}

MapManager::~MapManager()
{
	for (Block* block : blockVect) {
		delete block;
	}
	for (Goomba* Goomba : goombaVect) {
		delete Goomba;
	}
	delete player;
}

void MapManager::update(float deltaTime, float center_x)
{
	for (Goomba* goomba : goombaVect) {
		// offset is a little more than (width of goomba) + (1/2 width of player)
		if (not goomba->isActive && center_x + float(SCREEN_WIDTH) / 2.0f + 30.0f > goomba->getPosition().x) {
			goomba->isActive = true;			
		}
		if (not goomba->isDead) {
			goomba->update(deltaTime, blockVect, goombaVect, bouncingBlock);
		}
	}
	if (bouncingBlock) {
		if (bouncingBlock->isBouncing()) {
			bouncingBlock->update(deltaTime);
		}
		else {
			bouncingBlock = nullptr;
		}
	}
	player->update(deltaTime, goombaVect, blockVect, bouncingBlock);
	if (not (player->isAlive || deathMusicPlaying)) {
		themeMusic.openFromFile(PATH.mario_die_sound);
		themeMusic.setLoop(false);
		themeMusic.play();
		deathMusicPlaying = true;
	}
}

void MapManager::draw(sf::RenderWindow& window)
{
	for (Block* block : blockVect) {
		block->draw(window);
	}
	for (Goomba* goomba : goombaVect) {
		goomba->draw(window);
	}
	player->draw(window);
}

void MapManager::restart()
{
	setPosition();
	player->isAlive = true;
	player->isDead = false;
	player->setVelocity(sf::Vector2f(0.0f, 0.0f));
	themeMusic.openFromFile(PATH.bg_music);
	themeMusic.setLoop(true);
	themeMusic.play();
	deathMusicPlaying = false;
}

void MapManager::setPosition()
{
	int blockIdx = 0;
	int goombaIdx = 0;
	for (int i = 0; i < mapMatrix.size(); i++) {
		for (int j = 0; j < mapMatrix[0].size(); j++) {
			if (mapMatrix[i][j] == BLOCK) {
				blockVect[blockIdx++]->setPosition(i, j);
			}
			else if (mapMatrix[i][j] == PLAYER) {
				player->setPosition(i, j);
			}
			else if (mapMatrix[i][j] == ENEMY_L) {
				goombaVect[goombaIdx++]->setPosition(i, j, -1);
			}
			else if (mapMatrix[i][j] == ENEMY_R) {
				goombaVect[goombaIdx++]->setPosition(i, j, +1);
			}
		}
	}
}

void MapManager::toggleHitbox()
{
	for (Goomba* goomba : goombaVect) {
		goomba->toggleHitbox();
	}
	player->toggleHitbox();
}
