#pragma once

#include "DisplayObject.h"
#include "TextureManager.h"

class ThermalDetonator final : public DisplayObject
{
public:
	ThermalDetonator();
	~ThermalDetonator();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();

	void addForce(glm::vec2 Amount);

	void move();

	bool doesUpdate = false; 

	glm::vec2 Gravity = glm::vec2(0, 9.8f);

	bool calculateTheta;
	float Speed;
	float pixelsPerMeter;
	float throwAngle;

	glm::vec2 Force;

	// getters
	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;
	float getMaxSpeed() const;

	// setters
	void setTargetPosition(glm::vec2 newPosition);
	void setCurrentDirection(glm::vec2 newDirection);
	void setMaxSpeed(float newSpeed);

private:
	void m_checkBounds();
	void m_reset();

	// steering behaviours
	float m_maxSpeed;
	float m_turnRate;

	void m_changeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
};


