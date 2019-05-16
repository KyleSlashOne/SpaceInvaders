#pragma once
#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "AssetManager.h"

class AssetManager;
class ColliderComponent;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void checkAlive();
	void clean();

	void resetFrameCount() { frameCount = 0; }
	int getFrameCount() { return frameCount; }

	static bool isRunning;
	bool running() { return isRunning; }

	static bool fire;
	bool firing() { return fire; }

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static AssetManager* assets;

	enum groupLabels : std::size_t
	{
		groupPlayer,
		groupEnemies,
		groupMap,
		groupColliders,
		groupProjectiles
	};

private:
	bool gameOver = false;
	bool gameEndCheck = false;
	int frameCount = 0;
	int enemySpeed = 60;
	int score = 0;
	int lives = 3;
	int randNum = 0;
	int round = 0;
	bool reverseDirection = false;
	bool reverseCheck = false;
	SDL_Window *window;
};

