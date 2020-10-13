#pragma once

#include "DisplayObject.h"
#include "TextureManager.h"

class LootCrate final : public DisplayObject
{
public:
	LootCrate();
	~LootCrate();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void addForce(glm::vec2 Amount);
	void move();

	bool doesUpdate = false; 

	glm::vec2 Gravity = glm::vec2(0, 9.8f);

	bool calculateTheta;
	float Speed;
	float pixelsPerMeter;
	float throwAngle;

	float Rotation = 0.0f;

	glm::vec2 Force;

private:

	// steering behaviours
	float m_maxSpeed;
	float m_turnRate;

	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
};


