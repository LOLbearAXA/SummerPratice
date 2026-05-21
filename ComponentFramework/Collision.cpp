#include "Collision.h"
#include "Body.h"
#include <VMath.h>

Collision::Collision() {
	initialCollisionTable();
}

Collision::~Collision() {}

// HELP FUNCTONS
bool sphereToSphere(const Body& body1, const Body& body2)
{
	bool result = pow((body1.getPos().x - body2.getPos().x), 2) +
		pow((body1.getPos().y - body2.getPos().y), 2) +
		pow((body1.getPos().z - body2.getPos().z), 2)
		<= pow(body1.getRadius() + body2.getRadius(), 2);
	return result;
}

bool planeToSphere(const Body& body1, const Body& body2)
{
	bool result = VMath::dot(body2.getPos() - body1.getPointOnPlane(), body1.getPlaneNormal()) <= body2.getRadius();
	return result;
}

bool sphereToPlane(const Body& body1, const Body& body2)
{
	bool result = planeToSphere(body2, body1);
	return result;
}

// COLLISION STUFF
void Collision::initialCollisionTable() {
	for (int i = 0; i < MAX_SHAPE; i++) {
		for (int j = 0; j < MAX_SHAPE; j++) {
			collisionTable[i][j] = nullptr;
		}
	}

	// all collision types below
	collisionTable[SPHERE][SPHERE] = sphereToSphere;
	collisionTable[SPHERE][PLANE] = sphereToPlane;
	collisionTable[PLANE][SPHERE] = planeToSphere;
}

bool Collision::checkCollision(const Body& body1, const Body& body2) {
	CollisionFn check = collisionTable[body1.collisionType][body2.collisionType];

	if (check) {
		return check(body1, body2);
	}
	else {
		return false; // no collision
	}
}