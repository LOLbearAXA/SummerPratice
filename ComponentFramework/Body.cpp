#include "Body.h"
#include <QMath.h>
#include <MMath.h>

Body::Body(): pos{}, vel{}, accel{}, mass{1.0f}, radius(1.0f), 
mesh{nullptr}, texture{nullptr}{
	// set inertia
}

Body::~Body() {}

void Body::Update(float deltaTime) {
	/// From 1st semester physics class
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;
	vel += accel * deltaTime;
}

void Body::ApplyForce(Vec3 force) {
	accel = force / mass;
}

bool Body::OnCreate() {
	return true;
}

void Body::OnDestroy() {

}

void Body::Render() const {

}

