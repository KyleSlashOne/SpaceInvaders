#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include "Layout.h"
#include <sstream>
#include <ctime>
#include <cstdlib>

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
AssetManager* Game::assets = new AssetManager(&manager);

std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());
auto& label_S(manager.addEntity());
auto& label_L(manager.addEntity());
auto& label_E(manager.addEntity());
auto& label_Q(manager.addEntity());

bool Game::isRunning = false;
bool Game::fire = false;

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window Created!..." << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "Renderer Created!..." << std::endl;
		}

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error in SDL_TTF" << std::endl;
	}

	assets->AddTexture("player", "assets/player.png");
	assets->AddTexture("projectile", "assets/projectile.png");
	assets->AddFont("verdana", "assets/verdana.ttf", 16);

	player.addComponent<TransformComponent>(240.0f, 600.0f, 16, 16, 2);
	player.addComponent<SpriteComponent>("player");
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player");
	player.addGroup(Game::groupPlayer);

	SDL_Color white = { 255, 255, 255 };
	label_S.addComponent<UILabelComponent>(8, 8, ("Score: 0"), "verdana", white);
	label_L.addComponent<UILabelComponent>(408, 8, ("Lives: 3"), "verdana", white);

	Layout::Layout();

	srand(time(0));
	randNum = rand() % 240;
}

auto& players(manager.getGroup(Game::groupPlayer));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& tiles(manager.getGroup(Game::groupMap));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
	if (!gameOver)
	{
		//all enemies defeated, starting next round
		if (manager.groupSize(Game::groupEnemies) == 0)
		{
			round++;
			score += 100;
			enemySpeed = 60 - (5 * round);
			if (enemySpeed < 10)
			{
				enemySpeed = 10;
			}
			reverseCheck = false;
			reverseDirection = false;
			Layout::ResetLayout(round);
			std::cout << round << std::endl;
		}
		else
		{
			//have enemies reached bottom of screen?
			for (auto& e : enemies)
			{
				if (e->getComponent<TransformComponent>().position.y >= 568)
				{
					gameOver = true;
				}
			}
		}

		//firing enemy projectiles at random intervals
		if (frameCount >= randNum)
		{
			randNum += rand() % (240 - (round * 10));
			int chance = 20;
			bool fired = false;

			while (!fired && chance > 0)
			{
				for (auto& e : enemies)
				{
					if (rand() % chance == 0)
					{
						Vector2D location(e->getComponent<TransformComponent>().position.x + 15.0f, e->getComponent<TransformComponent>().position.y);
						assets->CreateProjectile(location, Vector2D(0.0f, 2.0f), "Projectile_E");
						fired = true;
						break;
					}
				}

				chance--;
			}
		}
	}
}

void Game::update()
{
	Vector2D playerPos(0.0f, 0.0f);

	if (!gameOver)
	{
		//player position before update for collision detection
		playerPos = player.getComponent<TransformComponent>().position;

		for (auto& e : enemies)
		{
			e->getComponent<SpriteComponent>().setFrameCount(frameCount);
			e->getComponent<SpriteComponent>().setSpeed(enemySpeed);
		}
	}

	manager.refresh();
	manager.update();

	if(!gameOver)
	{
		//prevents firing more than one porjectile at a time
		for (auto& p : projectiles)
		{
			if (p->getComponent<ColliderComponent>().tag == "Projectile_P")
			{
				fire = false;
			}
		}
		//fire projectile from player position
		if (fire)
		{
			Vector2D location(player.getComponent<TransformComponent>().position.x + 15.0f, player.getComponent<TransformComponent>().position.y);
			assets->CreateProjectile(location, Vector2D(0.0f, -3.0f), "Projectile_P");
			fire = false;
		}
		//enemy movement
		if (frameCount % enemySpeed == 0)
		{
			Vector2D vSide(8.0f, 0.0f);
			if (reverseDirection)
			{
				Vector2D vRev(-1.0f, 0.0f);
				vSide.Multiply(vRev);
			}
			//reverse direction if needed and speed up enemy movement + animation
			if (reverseCheck)
			{
				Vector2D vDown(0.0f, 48.0f);
				for (auto& e : enemies)
				{
					e->getComponent<TransformComponent>().position.Add(vDown);
				}

				if (enemySpeed > 20)
				{
					enemySpeed -= 10;
				}
				else if (enemySpeed > 10)
				{
					enemySpeed -= 5;
				}


				reverseDirection = !reverseDirection;
				reverseCheck = false;
			}
			else
			{
				for (auto& e : enemies)
				{
					e->getComponent<TransformComponent>().position.Add(vSide);

					if (e->getComponent<TransformComponent>().position.x <= 8 ||
						e->getComponent<TransformComponent>().position.x >= 438)
					{
						reverseCheck = true;
					}
				}
			}
		}
		//collision detection
		for (auto& c : colliders)
		{
			//player and wall detection
			if (c->tag == "Wall")
			{
				if (Collision::AABB(player.getComponent<ColliderComponent>(), *c))
				{
					player.getComponent<TransformComponent>().position = playerPos;
				}
			}
			//player and enemy projectile detection
			else if (c->tag == "Projectile_E" && !(c->entity->getComponent<ProjectileComponent>().hasCollided()))
			{
				if (Collision::AABB(player.getComponent<ColliderComponent>(), *c))
				{
					c->entity->destroy();
					c->entity->getComponent<ProjectileComponent>().setCollided();

					lives--;
					if (lives <= 0)
					{
						gameOver = true;
					}

					std::stringstream ss;
					ss << "Lives: " << lives;
					label_L.getComponent<UILabelComponent>().SetLabelText(ss.str(), "verdana");
				}
			}
			//player projectile and enemy detection
			else if (c->tag == "Projectile_P" && !(c->entity->getComponent<ProjectileComponent>().hasCollided()))
			{
				for (auto& e : enemies)
				{
					if (Collision::AABB(e->getComponent<ColliderComponent>(), *c))
					{
						c->entity->destroy();
						e->destroy();
						c->entity->getComponent<ProjectileComponent>().setCollided();

						score += 10;

						std::stringstream ss;
						ss << "Score: " << score;
						label_S.getComponent<UILabelComponent>().SetLabelText(ss.str(), "verdana");

						break;
					}
				}
			}
		}
	}

	frameCount++;
}

void Game::render()
{
	SDL_RenderClear(renderer);
	
	if (!gameOver)
	{
		for (auto& p : projectiles)
		{
			p->draw();
		}
		for (auto& p : players)
		{
			p->draw();
		}
		for (auto& e : enemies)
		{
			e->draw();
		}
		for (auto& t : tiles)
		{
			t->draw();
		}
	}

	label_S.draw();
	label_L.draw();

	if (gameOver)
	{
		label_E.draw();
		label_Q.draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::checkAlive()
{
	if (gameOver && !gameEndCheck)
	{
		for (auto& p : projectiles)
		{
			p->destroy();
		}
		for (auto& e : enemies)
		{
			e->destroy();
		}

		SDL_Color white = { 255, 255, 255 };
		label_E.addComponent<UILabelComponent>(190, 256, ("Game Over"), "verdana", white);
		label_Q.addComponent<UILabelComponent>(190, 272, ("Esc to Quit"), "verdana", white);

		gameEndCheck = true;
	}
}

void Game::clean()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Game Cleaned!..." << std::endl;
}