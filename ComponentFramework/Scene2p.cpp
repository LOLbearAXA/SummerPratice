#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene2p.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Texture.h"
#include <QMath.h>

Scene2p::Scene2p() :sphere{nullptr}, shader{nullptr}, mesh{nullptr},
					drawInWireMode{false} {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene2p::~Scene2p() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene2p::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	sphere = new Body();
	sphere->OnCreate();
	
	sphere2 = new Body();
	sphere2->OnCreate();

	plane1 = new Body();
	plane1->OnCreate();

	sphere->setRadius(1.0f);
	sphere2->setRadius(1.0f);

	mesh = new Mesh("meshes/Sphere.obj");
	mesh->OnCreate();

	planeMesh = new Mesh("meshes/plane.obj");
	planeMesh->OnCreate();

	skullMesh = new Mesh("meshes/Mario.obj");
	skullMesh->OnCreate();

	shader = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 7.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	//viewMatrix = viewMatrix * MMath::rotate(85.0f, Vec3(0.0f, 1.0f, 0.0f));

	lightPos = Vec3(3.0f, 0.0f, 0.0f);

	earthTexture = new Texture();
	if (!earthTexture->LoadImage("textures/earthclouds.jpg")) {
		std::cerr << "OH NO!\n";
		return false;
	}

	moonTexture = new Texture();
	if (!moonTexture->LoadImage("textures/moon.jpg")) {
		std::cerr << "OH NO!\n";
		return false;
	}

	marioTexture = new Texture();
	if (!marioTexture->LoadImage("textures/mario_main.png")) {
		std::cerr << "OH NO!\n";
		return false;
	}

	sphere->setPos(Vec3(0.0f, 2.0f + sphere->getRadius(), 0.0f));
	sphere->correctModelMatrix();
	sphere->setAngleVel(Vec3(0.0f, 1.0f, 0.0f));

	sphere2->setPos(Vec3(100.0f, 0.0f, 0.0f));
	sphere2->correctModelMatrix();

	sphere->setCollisionType(1);
	sphere2->setCollisionType(1);
	plane1->setCollisionType(2);
	Vec3 pointOnPlane(0.0f, 0.0f, 0.0f);
	Vec3 planeNormal(0.0f, 0.0f, 1.0f);
	planeNormal = VMath::normalize(planeNormal);
	plane1->setPlane(pointOnPlane, planeNormal);
		
	Quaternion orientation = QMath::angleAxisRotation(90.0f, Vec3(-1.0f, 0.0f, 0.0f));
	plane1->setOrientation(orientation);
	plane1->correctModelMatrix();
	//plane1->correctPlaneColliBox();

	return true;
}

void Scene2p::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	sphere->OnDestroy();
	delete sphere;

	sphere2->OnDestroy();
	delete sphere2;

	plane1->OnDestroy();
	delete plane1;

	mesh->OnDestroy();
	delete mesh;

	planeMesh->OnDestroy();
	delete planeMesh;

	skullMesh->OnDestroy();
	delete skullMesh;

	shader->OnDestroy();
	delete shader;

	delete earthTexture;

	delete moonTexture;

	delete marioTexture;
}

void Scene2p::HandleEvents(const SDL_Event &sdlEvent) {
	trackball.HandleEvents(sdlEvent);
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

void Scene2p::Update(const float deltaTime) {
	static float totalTime = 0.0f;
	totalTime += deltaTime;
}

void Scene2p::Render() const {
	glEnable(GL_DEPTH_TEST);

	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(drawInWireMode){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glUseProgram(shader->GetProgram()); // turn on shader
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniform3fv(shader->GetUniformID("lightPos"), 1, lightPos);

	Matrix4 rotation = MMath::toMatrix4(trackball.getQuat()); // don't do this

	// Earth
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphere->getModelMatrix() * rotation);
	glBindTexture(GL_TEXTURE_2D, earthTexture->getTextureID()); // BIND TEXTURE
	mesh->Render(GL_TRIANGLES); // RENDER ON THE TRIANGLES

	// Moon
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphere2->getModelMatrix());
	glBindTexture(GL_TEXTURE_2D, moonTexture->getTextureID()); // BIND TEXTURE
	mesh->Render(GL_TRIANGLES); // RENDER ON THE TRIANGLES

	// Plane
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, plane1->getModelMatrix());
	glBindTexture(GL_TEXTURE_2D, moonTexture->getTextureID());
	planeMesh->Render(GL_TRIANGLES);

	glBindTexture(GL_TEXTURE_2D, 0); // UNDIND TEXTURE (Don't need to unbind everytime)
	glUseProgram(0); // TURN OFF THE SHADER
}



	
