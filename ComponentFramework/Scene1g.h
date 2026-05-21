#ifndef Scene1g_H
#define Scene1g_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
#include <Vector>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;

#define MAX_NUMBER_OF_LIGHTS 10

class Scene1g : public Scene {
private:
	Body* virous;
	Shader* shader;
	Mesh* virousMesh;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	bool drawInWireMode;

	Vec3 lightPos[MAX_NUMBER_OF_LIGHTS];
	unsigned int numberOfLightPos;

	Vec4 diffuseColor[MAX_NUMBER_OF_LIGHTS];
	unsigned int numberOfDiffuseColor;

	Vec4 spectularColor[MAX_NUMBER_OF_LIGHTS];
	unsigned int numberOfSpectularColor;

	float brightness[MAX_NUMBER_OF_LIGHTS];
	unsigned int numberOfBrightness;

public:
	explicit Scene1g();
	virtual ~Scene1g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // Scene1g_H