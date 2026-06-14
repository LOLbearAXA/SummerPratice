#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "SceneMuntasir.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Texture.h"
#include <QMath.h>

// imgui
#include "imgui.h"

// Constructor
SceneMuntasir::SceneMuntasir() : playerShip{ nullptr }, shader{ nullptr }, AlphaWingMesh{ nullptr }, audioPlayer{ nullptr },
drawInWireMode{ false }, playerSpeed{ 5.0f }, bulletSpeed{ 10.0f }, bulletMesh{ nullptr }, Bot01Mesh{ nullptr }, Bot01Speed{ 3.0f }, spawnTimer{ 0.0f }, spawnInterval{ 2.0f } {
	Debug::Info("Created SceneMuntasir: ", __FILE__, __LINE__);
}

// Destructor
SceneMuntasir::~SceneMuntasir() {
	Debug::Info("Deleted SceneMuntasir: ", __FILE__, __LINE__);
}

// OnCreate
bool SceneMuntasir::OnCreate() {
	Debug::Info("Loading assets SceneMuntasir: ", __FILE__, __LINE__);

	// Load Alpha wing Model
	AlphaWingMesh = new Mesh("meshes/Temp_AlphaWingEX.obj");
	if (AlphaWingMesh->OnCreate() == false) {
		std::cout << "Mesh faild to load!\n";
		return false;
	}

	// Bullet Mesh
	bulletMesh = new Mesh("meshes/Temp_AlphaWing_Bullet.obj");
	if (bulletMesh->OnCreate() == false) {
		std::cout << "Bullet Mesh not found!\n";
		return false;
	}

	// Enemy Bot01 Mesh
	Bot01Mesh = new Mesh("meshes/Temp_AlphaWing_Enemy_Bot01.obj");
	if (Bot01Mesh->OnCreate() == false) {
		std::cout << "Enemy mesh not in there!\n";
		return false;
	}

	// Create the body with physics 
	playerShip = new Body();
	playerShip->OnCreate();

	// Load shader
	shader = new Shader("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed!\n";
		return false;
	}

	// Player Position
	playerPos = Vec3(0.0f, 0.0f, -10.0f);

	// Placement of Alpha Wing
	playerModelMatrix = MMath::translate(playerPos);

	// Camera looking forward
	viewMatrix = MMath::lookAt(
		Vec3(0.0f, 0.0f, 0.0f),
		Vec3(0.0f, 0.0f, -1.0f),
		Vec3(0.0f, 1.0f, 0.0f)
	);

	// Perspective (this belongs to projectionMatrix, not playerModelMatrix)
	projectionMatrix = MMath::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);

	// SDL_Mixer with own my tuning
	SDL_AudioSpec defaultSpec;
	defaultSpec.freq = 44100;
	defaultSpec.channels = 2;
	defaultSpec.format = SDL_AUDIO_S16;

	audioPlayer = SDL_OpenAudioDeviceStream(
		SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
		&defaultSpec,
		nullptr,
		nullptr
	);

	if (!audioPlayer) {
		std::cout << "Failed to create audio player\n";
	}

	SDL_ResumeAudioStreamDevice(audioPlayer);

	audioTest = new Sound("audio/music/deadmou5-gg.wav");
	audioTest->OnCreate();

	audioTest->Play(audioPlayer);

	int queued = SDL_GetAudioStreamQueued(audioPlayer);

	SDL_Log("Queued bytes: %d", queued);

	return true;
}

// OnDestroy
void SceneMuntasir::OnDestroy() {
	//Debug
	Debug::Info("Deleting assets SceneMuntasir: ", __FILE__, __LINE__);

	// Player Ship
	playerShip->OnDestroy();
	delete playerShip;

	// Alpha Wing Mesh
	AlphaWingMesh->OnDestroy();
	delete AlphaWingMesh;

	// Bullet Mesh
	bulletMesh->OnDestroy();
	delete bulletMesh;
	bulletPositions.clear();

	// Bot01 Mesh
	Bot01Mesh->OnDestroy();
	delete Bot01Mesh;
	Bot01Positions.clear();

	// Shader
	shader->OnDestroy();
	delete shader;

	// Audio player
	SDL_DestroyAudioStream(audioPlayer);
	audioTest->OnDestroy();
	delete audioTest;
}

// HandleEvents
void SceneMuntasir::HandleEvents(const SDL_Event& sdlEvent) {
	// Keyboard
	switch (sdlEvent.type) {
	case SDL_EVENT_KEY_DOWN:
		switch (sdlEvent.key.scancode) {
		case SDL_SCANCODE_F12:
			drawInWireMode = !drawInWireMode;
			break;
		case SDL_SCANCODE_W:
			playerPos.y += playerSpeed * 0.1f;
			break;
		case SDL_SCANCODE_S:
			playerPos.y -= playerSpeed * 0.1f;
			break;
		case SDL_SCANCODE_A:
			playerPos.x -= playerSpeed * 0.1f;
			break;
		case SDL_SCANCODE_D:
			playerPos.x += playerSpeed * 0.1f;
			break;
		case SDL_SCANCODE_SPACE:
			bulletPositions.push_back(playerPos);
			break;
		default:
			break;
		}
		break;

	// Mouse
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			bulletPositions.push_back(playerPos);
		}
		break;
	}
}

// Update
void SceneMuntasir::Update(const float deltaTime) {
	static float totalTime = 0.0f;
	totalTime += deltaTime;

	// Player Matrix
	playerModelMatrix = MMath::translate(playerPos) *
						MMath::scale(0.3f, 0.3f, 0.3f);

	// Move every bullet forward
	for (int i = 0; i < bulletPositions.size(); i++) {
		bulletPositions[i].x += bulletSpeed * deltaTime;
	}

	// Remove bullets that have gone off screen (above y = 15)
	for (int i = bulletPositions.size() - 1; i > +0; i--) {
		if (bulletPositions[i].y > 15.0f) {
			bulletPositions.erase(bulletPositions.begin() + i);
		}
	}

	// Spawn timer
	spawnTimer += deltaTime;
	if (spawnTimer >= spawnInterval) {

		spawnTimer = 0.0f;

		// Spawn Bot01 on the right side, random Y height
		float randomY = ((rand() % 10) - 5) * 0.5f;
		Bot01Positions.push_back(Vec3(15.0f, randomY, -10.0f));
	}

	// Move Bot01 to the left towards player
	for (int i = 0; i < Bot01Positions.size(); i++) {
		Bot01Positions[i].x -= Bot01Speed * deltaTime;
	}

	// Remove enemies that passed player
	for (int i = Bot01Positions.size() - 1; i >= 0; i--) {
		if (Bot01Positions[i].x < -15.0f) {
			Bot01Positions.erase(Bot01Positions.begin() + i);
		}
	}
}

// Render
void SceneMuntasir::Render() const {
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glUseProgram(shader->GetProgram()); // turn on shader
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, playerModelMatrix);

	// Draw Alpha WIng
	AlphaWingMesh->Render();

	// Draw Every Bullet
	for (int i = 0; i < bulletPositions.size(); i++) {
		Matrix4 bulletMatrix = MMath::translate(bulletPositions[i]);
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, bulletMatrix);
		bulletMesh->Render();
	}

	// Draw Bot01
	for (int i = 0; i < Bot01Positions.size(); i++) {
		Matrix4 enemyMatrix = MMath::translate(Bot01Positions[i]) *
							  MMath::rotate(180.0f, Vec3(0.0f, 1.0f, 0.0f)) *
							  MMath::scale(0.3f, 0.3f, 0.3f);
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, enemyMatrix);
		Bot01Mesh->Render();
	}

	glUseProgram(0); // TURN OFF THE SHADER
}

// imgui
void SceneMuntasir::DrawGui() {
	// Optional tiny debug window
	ImGui::Begin("Scene3p Debug");
	ImColor textColor(255, 255, 255);
	ImGui::Text("Yay, ImGui is working!");
	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	// Center of screen
	ImVec2 center(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

	// Crosshair settings
	float crosshairSize = 20.0f;
	float crosshairThickness = 5.0f;

	// Horizontal line
	drawList->AddLine(
		ImVec2(center.x - crosshairSize, center.y),
		ImVec2(center.x + crosshairSize, center.y),
		IM_COL32(255, 255, 255, 255),
		crosshairThickness
	);

	// Vertical line
	drawList->AddLine(
		ImVec2(center.x, center.y - crosshairSize),
		ImVec2(center.x, center.y + crosshairSize),
		IM_COL32(255, 255, 255, 255),
		crosshairThickness
	);
}