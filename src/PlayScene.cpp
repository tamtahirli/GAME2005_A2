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
	TextureManager::Instance()->draw("background", 300.0f, 150.0f, 0, 255, true);

	Util::DrawLine(Triangle[0], Triangle[1]);
	Util::DrawLine(Triangle[1], Triangle[2]);
	Util::DrawLine(Triangle[2], Triangle[0]);

	// This makes the slope at the bottom, size of x position + 600
	Util::DrawLine(Triangle[0], glm::vec2(Triangle[0].x + 600.0f, Triangle[0].y));

	if (m_pLootCrate->doesUpdate)
		SetText();

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
}

void PlayScene::update()
{
	if (m_pLootCrate->doesUpdate)
	{
		if (!AddFriction) Friction = 0.0f;

		float xAcceleration =
			m_pLootCrate->Mass * m_pLootCrate->Gravity * cos(Theta);

		float yAcceleration =
			m_pLootCrate->Mass * m_pLootCrate->Gravity * sin(Theta);

		float length = sqrt(TriangleWidth * TriangleWidth + TriangleHeight * TriangleHeight);
		float finalVelocity = sqrt(2 * yAcceleration * length);

		/*
		
		Does not look accurate, removed:

		if (m_pLootCrate->getRigidBody()->velocity.x >= finalVelocity)
		{
			Theta = 0.0f;
			yAcceleration = 0.0f; // mg - n
			m_pLootCrate->Rotation = Theta;
			m_pLootCrate->getRigidBody()->velocity.y = 0.0f;

			xAcceleration = -Friction * m_pLootCrate->Mass * m_pLootCrate->Gravity; // -f * mg

			if (m_pLootCrate->getRigidBody()->velocity.x <= 0)
			{
				// Come to a complete stop
				xAcceleration = 0.0f;
				m_pLootCrate->getRigidBody()->velocity.x = 0.0f;
			}
		}*/

		if (m_pLootCrate->getTransform()->position.y >= Triangle[1].y - m_pLootCrate->getHeight() / 2)
		{
			Theta = 0.0f;
			yAcceleration = 0.0f; // mg - n
			m_pLootCrate->Rotation = Theta;
			m_pLootCrate->getRigidBody()->velocity.y = 0.0f;

			xAcceleration = -Friction * m_pLootCrate->Mass * m_pLootCrate->Gravity; // -f * mg

			if (m_pLootCrate->getRigidBody()->velocity.x <= 0)
			{
				// Come to a complete stop
				xAcceleration = 0.0f;
				m_pLootCrate->getRigidBody()->velocity.x = 0.0f;
			}
		}

		m_pLootCrate->getRigidBody()->acceleration = glm::vec2(xAcceleration, yAcceleration);
	}
	
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
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "background");

	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Default values for triangle width, height, X, & Y
	TriangleWidth = 400.0f;
	TriangleHeight = 300.0f;
	TrianglePosX = 150.0f;
	TrianglePosY = 600.0f;

	m_pLootCrate = new LootCrate();
	m_pLootCrate->Mass = 12.8f; // 12.8 kg mass
	m_pLootCrate->pixelsPerMeter = 1.0f; // 1.0 PPM
	m_pLootCrate->Gravity = 9.8f;
	addChild(m_pLootCrate);

	CreateLabels();

	SetTriangle();
	
	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2, 700.0f);
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

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the grave accent (`) to toggle simulation menu", "Consolas", 20.0f, { 0, 255, 0, 255 });
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2, 750.0f);

	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function()
{
	ImGui::NewFrame();
	
	ImGui::Begin("Edit Variables", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
	if (ImGui::Button("Play"))
	{
		m_pLootCrate->doesUpdate = true;
	}

	if (ImGui::Button("Reset"))
	{
		m_pLootCrate->doesUpdate = false;
		SetTriangle();
		m_pLootCrate->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pLootCrate->getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
		SetText();
	}

	if (ImGui::Button("Pause"))
	{
		m_pLootCrate->doesUpdate = false;
	}

	if (ImGui::SliderFloat("Position", &TrianglePosX, 0.0f, Config::SCREEN_WIDTH) && !m_pLootCrate->doesUpdate)
		SetTriangle();

	if (ImGui::SliderFloat("Ramp Height", &TriangleHeight, 0.0f, 300.0f) && !m_pLootCrate->doesUpdate)
		SetTriangle();

	if (ImGui::SliderFloat("Ramp Width", &TriangleWidth, 0.0f, 400.0f) && !m_pLootCrate->doesUpdate)
		SetTriangle();

	if (ImGui::SliderFloat("Mass (Kg)", &m_pLootCrate->Mass, 1.0f, 100.0f) && !m_pLootCrate->doesUpdate)
		SetText();

	ImGui::Checkbox("Add friction?", &AddFriction);

	if (AddFriction)
	{
		ImGui::SliderFloat("Friction", &Friction, 0.0f, 0.999f);
	}

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

	Theta = atan(TriangleHeight / TriangleWidth);

	m_pLootCrate->Rotation = glm::degrees(Theta);

	m_pLootCrate->getTransform()->position = 
		glm::vec2(Triangle[2].x + m_pLootCrate->getWidth() / 2.5f, Triangle[2].y - m_pLootCrate->getHeight() / 2.5f);

	SetText();
}

void PlayScene::SetText()
{
	std::string Text = "";
	Text = "Mass: " + std::to_string(m_pLootCrate->Mass);
	MassLabel->setText(Text);

	Text = "Position (x, y): (" + std::to_string(m_pLootCrate->getTransform()->position.x) + ", " + std::to_string(m_pLootCrate->getTransform()->position.y) + ")";
	PositionLabel->setText(Text);

	Text = "Velocity (x, y): (" + std::to_string(m_pLootCrate->getRigidBody()->velocity.x) + ", " + std::to_string(m_pLootCrate->getRigidBody()->velocity.y) + ")";
	VelocityLabel->setText(Text);

	Text = "Acceleration (x, y): (" + std::to_string(m_pLootCrate->getRigidBody()->acceleration.x) + ", " + std::to_string(m_pLootCrate->getRigidBody()->acceleration.y) + ")";
	AccelerationLabel->setText(Text);

	Text = "Force: " + std::to_string(m_pLootCrate->Mass * m_pLootCrate->getRigidBody()->acceleration.x) + " N";
	ForceLabel->setText(Text);

	Text = "Theta: " + std::to_string(glm::degrees(Theta));
	ThetaLabel->setText(Text);
}

void PlayScene::CreateLabels()
{
	const SDL_Color green = { 0, 255, 0, 255 };

	std::string Text = "";
	Text = "Mass: " + std::to_string(m_pLootCrate->Mass);
	MassLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 25.0f));
	MassLabel->setParent(this);
	addChild(MassLabel);

	Text = "Position (x, y): (" + std::to_string(m_pLootCrate->getTransform()->position.x) + ", " + std::to_string(m_pLootCrate->getTransform()->position.y) + ")";
	PositionLabel = new Label(Text, "Consolas", 15, green, glm::vec2(175.0f, 100.0f));
	PositionLabel->setParent(this);
	addChild(PositionLabel);

	Text = "Velocity (x, y): (" + std::to_string(m_pLootCrate->getRigidBody()->velocity.x) + ", " + std::to_string(m_pLootCrate->getRigidBody()->velocity.y) + ")";
	VelocityLabel = new Label(Text, "Consolas", 15, green, glm::vec2(175.0f, 50.0f));
	VelocityLabel->setParent(this);
	addChild(VelocityLabel);

	Text = "Acceleration (x, y): (" + std::to_string(m_pLootCrate->getRigidBody()->acceleration.x) + ", " + std::to_string(m_pLootCrate->getRigidBody()->acceleration.y) + ")";
	AccelerationLabel = new Label(Text, "Consolas", 15, green, glm::vec2(175.0f, 75.0f));
	AccelerationLabel->setParent(this);
	addChild(AccelerationLabel);

	Text = "Force: " + std::to_string(m_pLootCrate->Mass * m_pLootCrate->getRigidBody()->acceleration.x) + " N";
	ForceLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 125.0f));
	ForceLabel->setParent(this);
	addChild(ForceLabel);

	Text = "Theta: " + std::to_string(glm::degrees(Theta));
	ThetaLabel = new Label(Text, "Consolas", 15, green, glm::vec2(100.0f, 150.0f));
	ThetaLabel->setParent(this);
	addChild(ThetaLabel);
}