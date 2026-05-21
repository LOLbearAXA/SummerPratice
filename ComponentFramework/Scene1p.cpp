#include <glew.h>
#include <iostream>
#include <SDL.h>
#include <SDL3/SDL_events.h>
#include "Scene1p.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include <VMath.h>
#include "Collision.h"

Scene1p::Scene1p() :plane{nullptr}, shader{nullptr}, mesh{nullptr},
sphere(nullptr), sphereMesh(nullptr),
drawInWireMode{true} {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1p::~Scene1p() {
	Debug::Info("Deleted Scene1: ", __FILE__, __LINE__);
}

bool Scene1p::OnCreate() {
	Debug::Info("Loading assets Scene1: ", __FILE__, __LINE__);
	plane = new Body();
	plane->OnCreate();
	
	// set initial postion and plane normal
	plane->setPos(Vec3(0.0f, -2.0f, 0.0f));
	Vec3 pointOnPlane(plane->getPos()); // this will give the center of the plane as p0
	Vec3 planeNormal(0.0f, 0.0f, 1.0f);
	planeNormal = VMath::normalize(planeNormal);
	plane->setPlane(pointOnPlane, planeNormal);

	mesh = new Mesh("meshes/Plane.obj");
	mesh->OnCreate();

	sphere = new Body();
	sphere->OnCreate();

	// set initial values of sphere
	sphere->setAngleVel(Vec3(0.0f, 0.0f, 0.0f));
	sphere->setRadius(1.0f);
	sphere->setPos(Vec3(0.0f, sphere->getRadius() + 1.0f, 0.0f));
	sphere->setMass(1.0f);

	sphere2 = new Body();
	sphere2->OnCreate();

	sphere2->setAngleVel(Vec3(0.0f, 0.0f, 0.0f));
	sphere2->setRadius(1.0f);
	sphere2->setPos(Vec3(0.0f, sphere->getRadius() + 1.0f, 0.0f));
	sphere2->setMass(1.0f);
	sphere2->setPos(Vec3(3.0f, sphere2->getRadius(), 0.0f));
	sphere2->correctModelMatrix();

	// list of inertia that is available
	// 1. "SOLID_SPHERE"
	// 2. "HOLLOW_SPHERE"
	sphere->setInertia("SOLID_SPHERE");
	sphere2->setInertia("HOLLOW_SPHERE");

	shader = new Shader("shaders/defaultVert.glsl", "shaders/defaultFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}

	sphereMesh = new Mesh("meshes/Sphere.obj");
	sphereMesh->OnCreate();

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	modelMatrix.loadIdentity();

	// rotate (start off with a horizontal plane)
	Quaternion orientation = QMath::angleAxisRotation(90.0f, Vec3(-1.0f, 0.0f, 0.0f));
	plane->setOrientation(orientation);
	plane->correctModelMatrix();
	Matrix4 rotation = MMath::rotate(90.0f, Vec3(Vec3(-1.0f, 0.0f, 0.0f)));
	plane->setPlaneNormal(VMath::normalize(rotation * plane->getPlaneNormal()));
	//plane->correctPlaneColliBox(); // correct the plane normal as it rotates

	// set collision type for plane and sphere
	// 1 - sphere
	// 2 - rectangular (plane)
	plane->setCollisionType(COLLISION_TYPE::PLANE);
	sphere->setCollisionType(COLLISION_TYPE::SPHERE);
	sphere2->setCollisionType(COLLISION_TYPE::SPHERE);

	return true;
}

void Scene1p::OnDestroy() {
	Debug::Info("Deleting assets Scene1p: ", __FILE__, __LINE__);
	plane->OnDestroy();
	delete plane;

	sphere->OnDestroy();
	delete sphere;

	sphere2->OnDestroy();
	delete sphere2;

	mesh->OnDestroy();
	delete mesh;

	sphereMesh->OnDestroy();
	delete sphereMesh;

	shader->OnDestroy();
	delete shader;
}

void Scene1p::HandleEvents(const SDL_Event& sdlEvent) {
	const float angleDeg = 10.0f;
	Vec3 axis;
	Quaternion planeOrientation;
	Matrix4 rotation;
	Vec3 planeNormal;
	Vec3 normalRotationAxis;

	switch (sdlEvent.type) {
	case SDL_EVENT_KEY_DOWN:
		switch (sdlEvent.key.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;
		case SDL_SCANCODE_UP:
			// let the ball roll away from us
			axis = Vec3(-1.0f, 0.0f, 0.0f);
			planeOrientation = plane->getOrientation();
			planeOrientation = QMath::angleAxisRotation(angleDeg, axis) * planeOrientation;
			plane->setOrientation(planeOrientation);
			plane->correctModelMatrix();
			planeNormal = QMath::rotate(plane->getPlaneNormal(), QMath::angleAxisRotation(angleDeg, axis));
			planeNormal = VMath::normalize(planeNormal);
			plane->setPlaneNormal(planeNormal);

			break;
		case SDL_SCANCODE_DOWN:
			axis = Vec3(1.0f, 0.0f, 0.0f);
			planeOrientation = plane->getOrientation();
			planeOrientation = QMath::angleAxisRotation(angleDeg, axis) * planeOrientation;
			plane->setOrientation(planeOrientation);
			plane->correctModelMatrix();
			planeNormal = QMath::rotate(plane->getPlaneNormal(), QMath::angleAxisRotation(angleDeg, axis));
			planeNormal = VMath::normalize(planeNormal);
			plane->setPlaneNormal(planeNormal);

			break;
		case SDL_SCANCODE_LEFT:
			axis = Vec3(0.0f, 0.0f, 1.0f); //  use for orientation rotation
			planeOrientation = plane->getOrientation();
			planeOrientation = QMath::angleAxisRotation(angleDeg, axis) * planeOrientation;
			plane->setOrientation(planeOrientation);
			plane->correctModelMatrix();
			planeNormal = QMath::rotate(plane->getPlaneNormal(), QMath::angleAxisRotation(angleDeg, axis));
			planeNormal = VMath::normalize(planeNormal);
			plane->setPlaneNormal(planeNormal);
			
			break;
		case SDL_SCANCODE_RIGHT:
			axis = Vec3(0.0f, 0.0f, -1.0f);
			planeOrientation = plane->getOrientation();
			planeOrientation = QMath::angleAxisRotation(angleDeg, axis) * planeOrientation;
			plane->setOrientation(planeOrientation);
			plane->correctModelMatrix();
			planeNormal = QMath::rotate(plane->getPlaneNormal(), QMath::angleAxisRotation(angleDeg, axis));
			planeNormal = VMath::normalize(planeNormal);
			plane->setPlaneNormal(planeNormal);

			break;
		default:
			break;
		}
		break;
	}
}

void Scene1p::Update(const float deltaTime) {
	// make sure the position of the ball is included in hte translate matrix
	Collision co;

	// P1
	Vec3 UP_VECTOR(0.0F, 1.0F, 0.0F); // should be pointing up
	float GRAVITY = -9.8F;

	float angleRadOfPlane = acos(VMath::dot(UP_VECTOR, plane->getPlaneNormal())); // acos (up vector Dot plane normal)
	float distanceToPivot = sphere->getRadius() * sin(angleRadOfPlane); // radius * sin(angleRadOfPlane)
	float weight = sphere->getMass() * GRAVITY; // mass * gravitational acceleration (g)
	Vec3 directionOfTorque = VMath::cross(plane->getPlaneNormal(), UP_VECTOR); // up Vector cross plane normal
	Vec3 torque = distanceToPivot * weight * directionOfTorque; // distanceToPivot * weight * direction
	
	if ((VMath::mag(directionOfTorque) >= VERY_SMALL) && co.checkCollision(*sphere, *plane)) {
		sphere->applyTorque(torque);
		sphere->updateAngularVel(deltaTime);
		sphere->updateOrientation(deltaTime);
	}

	// convert angular velocity to liner velocity (cross product)
	// update position base on velocity
	if (co.checkCollision(*sphere, *plane)) { // apply torque
		if (setOnlyOnce) {
			// set accel and vel to 0 since there is no bounce in this case
			sphere->setAccel(Vec3(0.0f, 0.0f, 0.0f)); 
			sphere->setVel(Vec3(0.0f, 0.0f, 0.0f)); 
			setOnlyOnce = false;
		}
		sphere->correctSpherePositionOnCollision(*plane);
		sphere->updateLinierVel(plane->getPlaneNormal());
	}
	else if(!Collision::checkCollision(*sphere, *plane)){ // apply graity for falling;
		sphere->ApplyForce(Vec3(0.0f, GRAVITY, 0.0f));
		setOnlyOnce = true;
	}

	// sphere 2 test for hollow sphere inertia
	float angleRadOfPlane2 = acos(VMath::dot(UP_VECTOR, plane->getPlaneNormal())); // acos (up vector Dot plane normal)
	float distanceToPivot2 = sphere2->getRadius() * sin(angleRadOfPlane); // radius * sin(angleRadOfPlane)
	float weight2 = sphere2->getMass() * GRAVITY; // mass * gravitational acceleration (g)
	Vec3 directionOfTorque2 = VMath::cross(plane->getPlaneNormal(), UP_VECTOR); // up Vector cross plane normal
	Vec3 torque2 = distanceToPivot * weight * directionOfTorque; // distanceToPivot * weight * direction

	if ((VMath::mag(directionOfTorque2) >= VERY_SMALL) && co.checkCollision(*plane, *sphere2)) {
		sphere2->applyTorque(torque);
		sphere2->updateAngularVel(deltaTime);
		sphere2->updateOrientation(deltaTime);
	}

	if (co.checkCollision(*plane, *sphere2)) { // apply torque
		if (setOnlyOnce) {
			// set accel and vel to 0 since there is no bounce in this case
			sphere2->setAccel(Vec3(0.0f, 0.0f, 0.0f));
			sphere2->setVel(Vec3(0.0f, 0.0f, 0.0f));
			setOnlyOnce2 = false;
		}
		sphere2->correctSpherePositionOnCollision(*plane);
		sphere2->updateLinierVel(plane->getPlaneNormal());
	}
	else if (!co.checkCollision(*plane, *sphere2)) { // apply graity for falling
		sphere2->ApplyForce(Vec3(0.0f, GRAVITY, 0.0f));
		setOnlyOnce2 = true;
	}

	sphere->Update(deltaTime);
	sphere2->Update(deltaTime);
	//sphere->getPos().print();

	//plane->correctPlaneColliBox();
	//plane->getPointOnPlane().print();
	
	// order does matter
	// the transformation order is right to left (just the way the operator work)
	sphere->correctModelMatrix();
	sphere2->correctModelMatrix();
}

void Scene1p::Render() const {
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
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, plane->getModelMatrix());
	mesh->Render(GL_TRIANGLES);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphere->getModelMatrix());
	sphereMesh->Render(GL_TRIANGLES);

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, sphere2->getModelMatrix());
	sphereMesh->Render(GL_TRIANGLES);

	glUseProgram(0);
}



	
