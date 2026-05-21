#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene1g.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"

Scene1g::Scene1g() :virous{ nullptr }, shader{ nullptr }, virousMesh{ nullptr },
drawInWireMode{ true }, numberOfSpectularColor( 0 ), numberOfLightPos( 0 ), numberOfDiffuseColor( 0 ),
numberOfBrightness ( 0 )
{
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene1g::~Scene1g() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene1g::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	virous = new Body();
	virous->OnCreate();

	virousMesh = new Mesh("meshes/CoronaVirus.obj");
	virousMesh->OnCreate();

	shader = new Shader("shaders/multiPhongVert.glsl", "shaders/multiPhongFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	modelMatrix.loadIdentity();
	virous->setRadius(3.0f); // make it larger
	virous->setModelMatrix(modelMatrix);
	virous->correctModelMatrix(); 

	// first light 
	lightPos[0] = Vec3(6.0f, -3.0f, -2.0f);
	numberOfLightPos++;
	brightness[0] = 0.3f;
	numberOfBrightness++;

	diffuseColor[0] = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
	numberOfDiffuseColor++;

	spectularColor[0] = Vec4(0.3f, 0.3f, 0.3f, 0.0f);
	numberOfSpectularColor++;

	// second light
	lightPos[1] = Vec3(-6.0f, -3.0f, -2.0f);
	numberOfLightPos++;
	brightness[1] = 0.1f;
	numberOfBrightness++;

	diffuseColor[1] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
	numberOfDiffuseColor++;

	spectularColor[1] = Vec4(0.6f, 0.6f, 0.6f, 0.0f);
	numberOfSpectularColor++;

	// third light
	lightPos[2] = Vec3(0.0f, 6.0f, -2.0f);
	numberOfLightPos++;
	brightness[2] = 0.7f;
	numberOfBrightness++;

	diffuseColor[2] = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
	numberOfDiffuseColor++;

	spectularColor[2] = Vec4(0.1f, 0.1f, 0.1f, 0.0f);
	numberOfSpectularColor++;

	return true;
}

void Scene1g::OnDestroy() {
	Debug::Info("Deleting assets Scene0g: ", __FILE__, __LINE__);
	virous->OnDestroy();
	delete virous;

	virousMesh->OnDestroy();
	delete virousMesh;

	shader->OnDestroy();
	delete shader;

}

void Scene1g::HandleEvents(const SDL_Event& sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_EVENT_KEY_DOWN:
		switch (sdlEvent.key.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;
		default:
			break;
		}
		break;
	}
}

void Scene1g::Update(const float deltaTime) {
	static float totalTime;
	totalTime += deltaTime;

	float rotateAngle = 1.0f;
	Vec3 rotateAxis(0.0f, 1.0f, 0.0f);

	// make the virous spin
	Matrix4 rotation = MMath::rotate(rotateAngle, rotateAxis);
	modelMatrix = virous->getModelMatrix();

	modelMatrix = modelMatrix * rotation;

	virous->setModelMatrix(modelMatrix);
}

void Scene1g::Render() const {
	glEnable(GL_DEPTH_TEST);

	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);

	glUniform3fv(shader->GetUniformID("lightPos[0]"), numberOfLightPos, lightPos[0]);
	glUniform4fv(shader->GetUniformID("diffuseLight[0]"), numberOfDiffuseColor, diffuseColor[0]);
	glUniform4fv(shader->GetUniformID("spectularLight[0]"), numberOfSpectularColor, spectularColor[0]);

	glUniform1i(shader->GetUniformID("numberOfDiffuseLight"), numberOfDiffuseColor);
	glUniform1i(shader->GetUniformID("numberOfSpectularLight"), numberOfSpectularColor);
	glUniform1i(shader->GetUniformID("numberOfLight"), numberOfLightPos);
	glUniform1i(shader->GetUniformID("numberOfBrightness"), numberOfBrightness);
	glUniform2f(shader->GetUniformID("brightness"),  numberOfBrightness , brightness[0]);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, virous->getModelMatrix());
	virousMesh->Render(GL_TRIANGLES);
	glUseProgram(0);
}




