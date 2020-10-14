#include "LootCrate.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"

LootCrate::LootCrate()
{
	TextureManager::Instance()->load("../Assets/textures/lootcrate.png", "LootCrate");

	auto size = TextureManager::Instance()->getTextureSize("LootCrate");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(LOOTCRATE);
}


LootCrate::~LootCrate()
= default;

void LootCrate::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the LootCrate
	TextureManager::Instance()->draw("LootCrate", x, y, Rotation, 255, true);
}


void LootCrate::update()
{
	if (doesUpdate) move();
}

void LootCrate::clean()
{

}

void LootCrate::addForce(glm::vec2 Amount)
{
	Force += Amount;
}

void LootCrate::move()
{
	float deltaTime = 1.0f / 60.0f;

	getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime;
	getTransform()->position += getRigidBody()->velocity * deltaTime * pixelsPerMeter;

	//getRigidBody()->acceleration = glm::vec2(0.0f, Gravity);
	//getRigidBody()->acceleration += Force / getRigidBody()->mass;
	//getRigidBody()->velocity += getRigidBody()->acceleration;
	//getTransform()->position += getRigidBody()->velocity * deltaTime;

	//Force = glm::vec2(0.0f, 0.0f);
	//getRigidBody()->velocity *= 0.9f;
}

