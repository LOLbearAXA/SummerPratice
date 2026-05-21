#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene2g.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"

#include "Texture.h"

Scene2g::Scene2g() :skullBody{nullptr}, shader{nullptr}, skullMesh{nullptr},
drawInWireMode{true} {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene2g::~Scene2g() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene2g::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	skullBody = new Body();
	skullBody->OnCreate();
	
	skullMesh = new Mesh("meshes/Skull.obj");
	skullMesh->OnCreate();

	leftEyeBody = new Body();
	leftEyeBody->OnCreate();

	rightEyeBody = new Body();
	rightEyeBody->OnCreate();

	eyeMesh = new Mesh("meshes/sphere.obj");
	eyeMesh->OnCreate();

	skullTexture = new Texture();
	if (!skullTexture->LoadImage("textures/skull_texture.jpg")) {
		std::cerr << "OH NO!\n";
		return false;
	}

	eyeTexture = new Texture();
	if (!eyeTexture->LoadImage("textures/evilEye.jpg")) {
		std::cerr << "OH NO!\n";
		return false;
	}

	shader = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	modelMatrix.loadIdentity();
	return true;

	lightPos = Vec3(0.0f, 0.0f, -5.0f);
}

void Scene2g::OnDestroy() {
	Debug::Info("Deleting assets Scene0g: ", __FILE__, __LINE__);
	skullBody->OnDestroy();
	delete skullBody;

	skullMesh->OnDestroy();
	delete skullMesh;

	leftEyeBody->OnDestroy();
	delete leftEyeBody;

	rightEyeBody->OnDestroy();
	delete rightEyeBody;

	eyeMesh->OnDestroy();
	delete eyeMesh;
	
	shader->OnDestroy();
	delete shader;

	delete skullTexture;
}

void Scene2g::HandleEvents(const SDL_Event& sdlEvent) {
	trackballD.HandleEvents(sdlEvent);
	switch (sdlEvent.type) {
	case SDL_EVENT_KEY_DOWN:
		switch (sdlEvent.key.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;

		case SDL_SCANCODE_Y: // stop turning the skull
			skullStopTurning = !skullStopTurning;
			break;

		case SDL_SCANCODE_E: // makes the eye follow cursor
			eyeFollowsCursor = !eyeFollowsCursor;
			eyeStaresAtOrigin = false;
			break;

		case SDL_SCANCODE_R: // makes the eye stare at origin
			eyeStaresAtOrigin = !eyeStaresAtOrigin;
			eyeFollowsCursor = false;
			break;

		case SDL_SCANCODE_T: // makes the eye stare at skull's front
			eyeFollowsCursor = false;
			eyeStaresAtOrigin = false;
			break;

		default:
			break;
		}
		break;
	}
}

void Scene2g::Update(const float deltaTime) {
	static float totalTime;
	totalTime += deltaTime;

	const float EYE_SCALE = 0.4f;

	Matrix4 trackBallRotation = MMath::toMatrix4(trackballD.getQuat());

	if (skullStopTurning) {
		skullModelMatrix.loadIdentity();
	}
	else {
		skullModelMatrix = trackBallRotation;
	}

	// if there is any translation in skull

	Matrix4 leftEyeTranslate = MMath::translate(Vec3(0.5f, 0.2f, 0.8f));
	Matrix4 leftEyeRotation = MMath::rotate(-90.0f, Vec3(1.0f, 0.0f, 0.0f));
	//Matrix4 leftEyeRotation2 = MMath::rotate(30.0f, Vec3(0.0f, 0.0f, -1.0f));
	//Matrix4 leftEyeRotation3 = MMath::rotate(20.0f, Vec3(-1.0f, 0.0f, 0.0f));
	Matrix4 leftEyeRotation4 = MMath::rotate(90.0f, Vec3(0.0f, -1.0f, 0.0f));

	Matrix4 rightEyeTranslate = MMath::translate(Vec3(-0.5f, 0.2f, 0.8f));
	Matrix4 rightEyeRotation = MMath::rotate(-90.0f, Vec3(1.0f, 0.0f, 0.0f));
/*	Matrix4 rightEyeRotation2 = MMath::rotate(30.0f, Vec3(0.0f, 0.0f, -1.0f));
	Matrix4 rightEyeRotation3 = MMath::rotate(20.0f, Vec3(-1.0f, 0.0f, 0.0f))*/;
	Matrix4 rightEyeRotation4 = MMath::rotate(107.0f, Vec3(0.0f, -1.0f, 0.0f));

	Matrix4 eyeScale = MMath::scale(Vec3(EYE_SCALE, EYE_SCALE, EYE_SCALE));

	// eyeballModelMatrix - bind to skull
	if (eyeFollowsCursor) {
		leftEyeballModelMatrix = skullModelMatrix * leftEyeTranslate * trackBallRotation * leftEyeRotation4 * leftEyeRotation * eyeScale;

		rightEyeballModelMatrix = skullModelMatrix * rightEyeTranslate * trackBallRotation * rightEyeRotation4 * rightEyeRotation * eyeScale;
	}
	else if (eyeStaresAtOrigin && !skullStopTurning) {
		leftEyeballModelMatrix = skullModelMatrix * leftEyeTranslate * MMath::inverse(trackBallRotation) * leftEyeRotation4 * leftEyeRotation * eyeScale;

		rightEyeballModelMatrix = skullModelMatrix * rightEyeTranslate * MMath::inverse(trackBallRotation) * rightEyeRotation4 * rightEyeRotation * eyeScale;
	}
	else {
		leftEyeballModelMatrix = skullModelMatrix * leftEyeTranslate * leftEyeRotation4 * leftEyeRotation * eyeScale;

		rightEyeballModelMatrix = skullModelMatrix * rightEyeTranslate * rightEyeRotation4 * rightEyeRotation * eyeScale;
	}
}

void Scene2g::Render() const {
	glEnable(GL_DEPTH_TEST);

	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(drawInWireMode){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniform3fv(shader->GetUniformID("lightPos"), 1, lightPos);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, skullModelMatrix);
	glBindTexture(GL_TEXTURE_2D, skullTexture->getTextureID()); // BIND TEXTURE
	skullMesh->Render(GL_TRIANGLES);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, leftEyeballModelMatrix);
	glBindTexture(GL_TEXTURE_2D, eyeTexture->getTextureID()); // BIND TEXTURE
	eyeMesh->Render(GL_TRIANGLES);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, rightEyeballModelMatrix);
	glBindTexture(GL_TEXTURE_2D, eyeTexture->getTextureID()); // BIND TEXTURE
	eyeMesh->Render(GL_TRIANGLES);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}



	
