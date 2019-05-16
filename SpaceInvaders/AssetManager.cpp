#include "AssetManager.h"
#include "Components.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::CreateWall(Vector2D pos, int width, int height, std::string id)
{
	auto& wall(manager->addEntity());
	wall.addComponent<TransformComponent>(pos.x, pos.y, width, height, 1);
	wall.addComponent<SpriteComponent>(id);
	wall.addComponent<ColliderComponent>("Wall");
	wall.addGroup(Game::groupMap);
}

void AssetManager::CreateEnemy(Vector2D pos, int width, int height, std::string id)
{
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, width, height, 2);
	enemy.addComponent<SpriteComponent>(id, 2, 60);
	enemy.addComponent<ColliderComponent>("Enemy");
	enemy.addGroup(Game::groupEnemies);
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D velocity, std::string tag)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 2, 8, 1);
	projectile.addComponent<SpriteComponent>("projectile");
	projectile.addComponent<ProjectileComponent>(velocity);
	projectile.addComponent<ColliderComponent>(tag);
	projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	return fonts[id];
}