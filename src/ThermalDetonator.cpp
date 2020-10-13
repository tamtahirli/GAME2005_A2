#include "ThermalDetonator.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"

ThermalDetonator::ThermalDetonator() : m_maxSpeed(10.0f)
{
	TextureManager::Instance()->load("../Assets/textures/detonator.png", "ThermalDetonator");

	auto size = TextureManager::Instance()->getTextureSize("ThermalDetonator");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(THERMALDETONATOR);

	m_currentHeading = 0.0f; // current facing angle
	m_currentDirection = glm::vec2(1.0f, 0.0f); // facing right
	m_turnRate = 5.0f; // 5 degrees per frame
}


ThermalDetonator::~ThermalDetonator()
= default;

void ThermalDetonator::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ThermalDetonator
	TextureManager::Instance()->draw("ThermalDetonator", x, y, m_currentHeading, 255, true);
}


void ThermalDetonator::update()
{
	if(doesUpdate) move();
}

void ThermalDetonator::clean()
{
}

void ThermalDetonator::turnRight()
{
	m_currentHeading += m_turnRate;
	if (m_currentHeading >= 360)
	{
		m_currentHeading -= 360.0f;
	}
	m_changeDirection();
}

void ThermalDetonator::turnLeft()
{
	m_currentHeading -= m_turnRate;
	if (m_currentHeading < 0)
	{
		m_currentHeading += 360.0f;
	}

	m_changeDirection();
}

void ThermalDetonator::moveForward()
{
	getRigidBody()->velocity = m_currentDirection * m_maxSpeed;
}

void ThermalDetonator::moveBack()
{
	getRigidBody()->velocity = m_currentDirection * -m_maxSpeed;
}

void ThermalDetonator::addForce(glm::vec2 Amount)
{
	Force += Amount;
}

void ThermalDetonator::move()
{
	float deltaTime = 1.0f / 60.0f;

	getRigidBody()->velocity += (getRigidBody()->acceleration + Gravity) * deltaTime;
	getTransform()->position += getRigidBody()->velocity * deltaTime * pixelsPerMeter;

	//getRigidBody()->acceleration = glm::vec2(0.0f, Gravity);
	//getRigidBody()->acceleration += Force / getRigidBody()->mass;
	//getRigidBody()->velocity += getRigidBody()->acceleration;
	//getTransform()->position += getRigidBody()->velocity * deltaTime;

	//Force = glm::vec2(0.0f, 0.0f);
	//getRigidBody()->velocity *= 0.9f;

	//deltaTime = 1.0f / 60.0f; // FPS
	/*
	accel = 0, 9.8
	vel = accel * deltaTime
	pos = velocity * deltaTime * pixelsPerMeter
	float pixelsPerMeter = 1.0f;*/
}

glm::vec2 ThermalDetonator::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 ThermalDetonator::getCurrentDirection() const
{
	return m_currentDirection;
}

float ThermalDetonator::getMaxSpeed() const
{
	return m_maxSpeed;
}

void ThermalDetonator::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;

}

void ThermalDetonator::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void ThermalDetonator::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void ThermalDetonator::m_checkBounds()
{

	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position = glm::vec2(800.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f);
	}

}

void ThermalDetonator::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

void ThermalDetonator::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);

	glm::vec2 size = TextureManager::Instance()->getTextureSize("ThermalDetonator");
}

