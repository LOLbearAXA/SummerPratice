#ifndef BODY_H
#define BODY_H
#include <Vector.h> /// This is in GameDev
#include <Quaternion.h>
using namespace MATH; 

/// Just forward declair these classes so I can define a pointer to them
/// Used later in the course
class Mesh;
class Texture;

class Body {
public:
    Body();
    ~Body();
private: /// Physics stuff
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	float mass;

	// rotation stuff
	Quaternion orientation;
	Vec3 angularVel; // angle (radian / sec). also points along axis of rotation
 
	Matrix4 modelMatrix;

	// angular acceleration is change in angularVel with time
	Vec3 angularAcc;
	Matrix3 inertia;

	float radius; // for size / (collition?)

	// collision box (for plane)

	Vec3 pointOnPlane;
	Vec3 planeNormal;

	Vec4 planeVec;

	// check for types of collision
	// 1 - sphere
	// 2 - rectangular (plane)
	int collisionType;

	friend class Collision;
private:
	// Graphics stuff 
	Mesh *mesh;
	Texture *texture;

	void setSolidSphereInertia();
	void setHollowSphereInertia();
public:
	bool OnCreate();
	void OnDestroy();
	void Update(float deltaTime);
	void Render() const;
	void ApplyForce(Vec3 force);
	void setAccel(const Vec3 &accel_) { accel = accel_;}
	void setVel(const Vec3& vel_) { vel = vel_; }

	// rotation stuff
	void updateOrientation(float deltaTime);
	void applyTorque(Vec3 torque);
	void updateAngularVel(float deltaTime);

	void updateLinierVel(const Vec3& planeNormal_);

	// getter / setter
	void setModelMatrix(const Matrix4& newModelMatrix) { modelMatrix = newModelMatrix; }

	void setOrientation(Quaternion& orientation_) { orientation = orientation_; }
	void setAngleVel(const Vec3& angleVel) { angularVel = angleVel; }

	void setRadius(const float& newRadius) { radius = newRadius; }

	void setInertia(const Matrix3& newInertia) { inertia = newInertia; }

	// list of inertia that is available
	// 1. "SOLID_SPHERE"
	// 2. "HOLLOW_SPHERE"
	void setInertia(const std::string& type); // set inertia base on what kind of shape it is

	void setPos(const Vec3& pos_) { pos = pos_; }

	void setPlane(const Vec3& pointOnPlane_, const Vec3& planeNormal_) {
		pointOnPlane = pointOnPlane_; planeNormal = planeNormal_;
	}
	void setCollisionType(const int type) { collisionType = type; }

	void setMass(const float mass_) { mass = mass_; }
	void setPlaneNormal(const Vec3& planeNormal_) { planeNormal = planeNormal_; }

	// getter
	Vec3 getPlaneNormal() const { return planeNormal; }
	Vec3 getPos() const { return pos; }
	Matrix3 getInertia() const { return inertia; }
	Quaternion getOrientation() const { return orientation; }
	float getRadius() const { return radius; }
	Matrix4 getModelMatrix() const { return modelMatrix; }
	float getMass() const { return mass; }
	Vec3 getPointOnPlane() const { return pointOnPlane; }
	Vec3 getAngleVel() const { return angularVel; }

	// helper
	void correctPlaneColliBox();
	void correctModelMatrix(); // correct model matrix according to position
	void identifyPlane(); // helps identify the plane equation

	// correct the sphere position when collide with plane / another sphere
	void correctSpherePositionOnCollision(const Body& another);
};

#endif
