#pragma once
#ifndef SceneMuntasir_H
#define SceneMuntasir_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include "Sound.h"

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;
class Texture;

class SceneMuntasir : public Scene {
private:
	Body* sphere;
	Shader* shader;
	Mesh* mesh;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 earthModelMatrix, moonModelMatrix, marioModelMatrix;
	bool drawInWireMode;

	Vec3 lightPos;

	SDL_AudioStream* audioPlayer;
	Sound* audioTest;

public:
	explicit SceneMuntasir();
	virtual ~SceneMuntasir();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;



	// ImGui
	virtual void DrawGui() override;
};


#endif // SCENE0_H