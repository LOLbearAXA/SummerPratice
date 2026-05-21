#ifndef SCENE1P_H
#define SCENE1P_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;

class Scene1p : public Scene {
private:
	Body* plane;

	Body* sphere;
	Body* sphere2;

	Shader* shader;
	Mesh* mesh;

	Mesh* sphereMesh; // shuold be in body

	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;

	Matrix4 sphereMatrix; // should be in body (each body has it's own)

	bool drawInWireMode;

	bool setOnlyOnce = false;
	bool setOnlyOnce2 = false;
public:
	explicit Scene1p();
	virtual ~Scene1p();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // SCENE1P_H