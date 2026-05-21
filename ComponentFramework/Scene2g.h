#ifndef SCENE2G_H
#define SCENE2G_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>

#include "Trackball.h"

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;
class Trackball;
class Texture;

class Scene2g : public Scene {
private:
	Body* leftEyeBody;
	Body* rightEyeBody;
	Body* skullBody;
	Shader* shader;
	Mesh* eyeMesh;
	Mesh* skullMesh;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
	bool drawInWireMode;

	Matrix4 skullModelMatrix;
	Matrix4 leftEyeballModelMatrix;
	Matrix4 rightEyeballModelMatrix;


	Trackball trackballD;
	Texture* skullTexture;
	Texture* eyeTexture;
	Vec3 lightPos;

	bool eyeFollowsCursor = false;
	bool skullStopTurning = false;
	bool eyeStaresAtOrigin = false;

public:
	explicit Scene2g();
	virtual ~Scene2g();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // SCENE0P_H