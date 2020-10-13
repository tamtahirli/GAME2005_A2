#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 300.0f, 400.0f, 0, 255, true);

	Util::DrawLine(Triangle[0], Triangle[1]);
	Util::DrawLine(Triangle[1], Triangle[2]);
	Util::DrawLine(Triangle[2], Triangle[0]);

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
}

void PlayScene::update()
{
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{

	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.png", "background");

	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Default values for triangle width, height, X, & Y
	TriangleWidth = 100.0f;
	TriangleHeight = 100.0f;
	TrianglePosX = 300.0f;
	TrianglePosY = 400.0f;

	m_pLootCrate = new LootCrate();
	m_pLootCrate->Mass = 12.8f; // 12.8 kg mass
	addChild(m_pLootCrate);

	SetTriangle();
	
	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 500.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 500.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas", 20.0f, { 255, 255, 255, 255 });
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);

	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function()
{
	ImGui::NewFrame();
	
	ImGui::Begin("Edit Variables", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
	if (ImGui::Button("Play"))
	{
		std::cout << "You clicked play!";
	}

	if (ImGui::SliderFloat("Position", &TrianglePosX, 0.0f, Config::SCREEN_WIDTH))
		SetTriangle();

	if (ImGui::SliderFloat("Ramp Width", &TriangleWidth, 0.0f, 400.0f))
		SetTriangle();

	if (ImGui::SliderFloat("Ramp Height", &TriangleHeight, 0.0f, 300.0f))
		SetTriangle();

	/*if (ImGui::Button("Play"))
	{
		glm::vec2 DegreeToVector;
		if (m_pDetonator->calculateTheta)
		{
			glm::vec2 distance = StormTrooperPos.x - m_pDetonator->getTransform()->position;

			float gravityByDistance = m_pDetonator->Gravity.y * distance.x;
			float Equation = gravityByDistance / (Speed * Speed);
			float theta = 0.5 * glm::degrees(asin(Equation));

			std::cout << "DIST: " << distance.x << " THETA: " << theta << "\n";

			float cosTheta = cos(glm::radians(theta));
			float sinTheta = sin(glm::radians(theta));

			DegreeToVector = glm::vec2(cosTheta, -sinTheta);
		}
		else DegreeToVector = glm::vec2(cos(m_pDetonator->throwAngle), -sin(m_pDetonator->throwAngle));

		std::cout << "Degree to vector: " << DegreeToVector.x << " y: " << DegreeToVector.y << "\n";
		m_pDetonator->getRigidBody()->velocity = DegreeToVector * Speed;
		m_pDetonator->doesUpdate = true;
		
	}

	if (ImGui::Button("Reset"))
	{
		m_pDetonator->doesUpdate = false;
		m_pDetonator->getTransform()->position = glm::vec2(125.0f, 400.0f);
		m_pDetonator->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pDetonator->getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);

		SetText();
	}*/

	ImGui::Separator();

	ImGui::End();
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::SetTriangle()
{
	Triangle[0] = glm::vec2(TrianglePosX, TrianglePosY);
	Triangle[1] = glm::vec2(TrianglePosX + TriangleWidth, TrianglePosY);
	Triangle[2] = glm::vec2(TrianglePosX, TrianglePosY - TriangleHeight);

	float c = sqrt(TriangleWidth * TriangleWidth + TriangleHeight + TriangleHeight);
	float theta = atan(TriangleHeight / c);
	m_pLootCrate->Rotation = glm::degrees(theta);
	m_pLootCrate->getTransform()->position = glm::vec2(Triangle[2].x, Triangle[2].y - 35.0f);
	std::cout << "THETA: " << glm::degrees(theta) << "\n";
}