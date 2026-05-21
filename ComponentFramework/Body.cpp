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

void Body::updateLinierVel(const Vec3& planeNormal_) {
	vel = VMath::cross(angularVel, radius * planeNormal_);
}

void Body::ApplyForce(Vec3 force) {
	accel = force / mass;
}

void Body::updateOrientation(float deltaTime) {

	// mag of the angular velocity is angle / time
	float anglularVelMag = VMath::mag(angularVel);
	float angleRadians = anglularVelMag * deltaTime;
	float angleDegrees = angleRadians * RADIANS_TO_DEGREES;

	// get outta here if angle is zero
	if (angleDegrees <= VERY_SMALL) return;

	// angular velocity points along the axis
	Vec3 axisOfRotation = VMath::normalize(angularVel);

	Quaternion rotation = QMath::angleAxisRotation(angleDegrees, axisOfRotation);

	// Combine rotations by multiplication
	orientation = orientation * rotation;
}

void Body::applyTorque(Vec3 torque)
{
	// angular acceleration = inverse(rotational inertia) * torque
	angularAcc = MMath::inverse(inertia) * torque;
}

void Body::updateAngularVel(float deltaTime)
{
	// This will look similar to update()
	// vel += accel + deltatime
	// But with angular velocity and angular accleration instead
	angularVel += angularAcc * deltaTime;
}

void Body::setInertia(const std::string& type)
{
	if (type == "SOLID_SPHERE") {
		setSolidSphereInertia();
	}
	else if (type == "HOLLOW_SPHERE") {
		setHollowSphereInertia();
	}
}

void Body::correctPlaneColliBox()
{
	// let the collision box move along with the orientation
	pointOnPlane = pos; // correct position of position is changed
	//planeNormal = QMath::rotate(planeNormal, orientation); // not working for some reason
	//planeNormal = VMath::normalize(planeNormal);
	identifyPlane(); // correct the plane as it rotates
}

void Body::correctSpherePositionOnCollision(const Body& another) {
	if(another.planeNormal.y >= 0)
		pos = pos + another.planeNormal * (radius - VMath::dot(pos - another.pointOnPlane, another.planeNormal));
	else // making sure the position of the object is on the "up" side
		pos = pos - another.planeNormal * (radius + VMath::dot(pos - another.pointOnPlane, another.planeNormal));
	correctModelMatrix();
}

void Body::correctModelMatrix() {
	modelMatrix.loadIdentity();
	modelMatrix = modelMatrix * MMath::translate(pos) * 
		MMath::toMatrix4(orientation) * 
		MMath::scale(Vec3(radius, radius, radius));
}

void Body::setSolidSphereInertia()
{
	inertia[0] = (2.0f / 5.0f) * mass * pow(radius, 2) + mass * pow(radius, 2);
	inertia[4] = (2.0f / 5.0f) * mass * pow(radius, 2) + mass * pow(radius, 2);
	inertia[8] = (2.0f / 5.0f) * mass * pow(radius, 2) + mass * pow(radius, 2);
}

void Body::setHollowSphereInertia()
{
	inertia[0] = (2.0f / 3.0f) * mass * pow(radius, 2) + mass * pow(radius, 2);
	inertia[4] = (2.0f / 3.0f) * mass * pow(radius, 2) + mass * pow(radius, 2);
	inertia[8] = (2.0f / 3.0f) * mass * pow(radius, 2) + mass * pow(radius, 2);
}

void Body::identifyPlane() {
	float D = VMath::dot(planeNormal, pointOnPlane);
	planeVec = Vec4(planeNormal, -D);
}

bool Body::OnCreate() {
	return true;
}

void Body::OnDestroy() {

}

void Body::Render() const {

}

