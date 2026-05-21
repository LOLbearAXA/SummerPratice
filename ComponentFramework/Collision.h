#pragma once

enum COLLISION_TYPE
	// for Body (each different kind of shape should have different detection formula)
{
	SPHERE = 1,
	PLANE,
	OBJ1,
	MAX_SHAPE,
};

class Body;

typedef bool (*CollisionFn)(const Body&, const Body&);
static CollisionFn collisionTable[MAX_SHAPE][MAX_SHAPE];

typedef bool (*CollisionFixFn)(Body&, Body&);

// different types of collision
bool sphereToSphere(const Body&, const Body&);
bool sphereToPlane(const Body&, const Body&);
bool planeToSphere(const Body&, const Body&);

class Collision {
private:
	void initialCollisionTable();
public:

	Collision();
	~Collision();

	static bool checkCollision(const Body& body1, const Body& body2);
};