//Component used for keyboard input

#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	TransformComponent *transform;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_LEFT:
				transform->velocity.x = -1;
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				break;
			case SDLK_RIGHT:
				transform->velocity.x = 1;
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				break;
			case SDLK_SPACE:
				Game::fire = true;
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_LEFT:
				transform->velocity.x = 0;
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				break;
			case SDLK_RIGHT:
				transform->velocity.x = 0;
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
				break;
			default:
				break;
			}
		}
	}
};