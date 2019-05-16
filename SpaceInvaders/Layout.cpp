#include "Layout.h"
#include "Components.h"

extern Manager manager;

Layout::Layout()
{
	auto& wallT(manager.addEntity());
	auto& wallB(manager.addEntity());
	auto& wallL(manager.addEntity());
	auto& wallR(manager.addEntity());

	Game::assets->AddTexture("wall_T", "assets/wall_T.png");
	Game::assets->AddTexture("wall_B", "assets/wall_B.png");
	Game::assets->AddTexture("wall_L", "assets/wall_L.png");
	Game::assets->AddTexture("wall_R", "assets/wall_R.png");
	Game::assets->AddTexture("enemy_R", "assets/enemy_R.png");
	Game::assets->AddTexture("enemy_Y", "assets/enemy_Y.png");
	Game::assets->AddTexture("enemy_G", "assets/enemy_G.png");

	Game::assets->CreateWall(Vector2D(0.0f, 0.0f), 480, 2, "wall_T");
	Game::assets->CreateWall(Vector2D(0.0f, 638.0f), 480, 2, "wall_B");
	Game::assets->CreateWall(Vector2D(0.0f, 0.0f), 2, 640, "wall_L");
	Game::assets->CreateWall(Vector2D(478.0f, 0.0f), 2, 640, "wall_R");

	ResetLayout(0);
}

void Layout::ResetLayout(int round)
{
	if (round > 8)
	{
		round = 8;
	}

	for (int k = 0; k < 5; k++)
	{
		for (int i = 0; i < 8; i++)
		{
			float x = static_cast<float>(16 + (i * 32 + i * 8));
			float y = static_cast<float>(48 + (round * 40) + (k * 32 + k * 8));

			switch (k)
			{
			case 0:
				Game::assets->CreateEnemy(Vector2D(x, y), 16, 16, "enemy_R");
				break;
			case 1:
				Game::assets->CreateEnemy(Vector2D(x, y), 16, 16, "enemy_Y");
				break;
			case 2:
				Game::assets->CreateEnemy(Vector2D(x, y), 16, 16, "enemy_Y");
				break;
			case 3:
				Game::assets->CreateEnemy(Vector2D(x, y), 16, 16, "enemy_G");
				break;
			case 4:
				Game::assets->CreateEnemy(Vector2D(x, y), 16, 16, "enemy_G");
				break;
			default:
				break;
			}
		}
	}
}