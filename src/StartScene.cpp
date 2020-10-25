#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	TextureManager::Instance()->draw("background", 300.0f, 150.0f, 0, 255, true);
	drawDisplayList();

	// DEBUG PURPOSES
	//TheGame::Instance()->changeSceneState(PLAY_SCENE);
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{
	// Create background
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "background");

	const SDL_Color green = { 0, 255, 0, 255 };
	const SDL_Color cyan = { 0, 255, 255, 255 };

	m_pStartLabel = new Label("Ramp Simulation", "Dock51", 60, cyan, glm::vec2(Config::SCREEN_WIDTH / 2, 250.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pGroupMemberLabel = new Label("Tamerlan Tahirli - 101055392 -- Alexander Barnes - 101086806", "Consolas", 20, cyan, glm::vec2(Config::SCREEN_WIDTH / 2, 300.0f));
	m_pGroupMemberLabel->setParent(this);
	addChild(m_pGroupMemberLabel);

	m_pInstructionsLabel = new Label("Press start or 1 to begin", "Consolas", 40, green, glm::vec2(Config::SCREEN_WIDTH / 2, 400.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2, 600.0f);


	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);
}

