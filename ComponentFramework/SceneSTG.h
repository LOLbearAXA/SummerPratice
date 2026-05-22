#ifndef SCENESTG_H
#define SCENESTG_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class Body;
class Mesh;
class Shader;
class Texture;

class SceneSTG : public Scene {
private:
	Body* sphere;
	Body* sphere2;
	Body* plane1;
	Shader* shader;
	Mesh* mesh, * planeMesh;
	Mesh* skullMesh;
	Texture* earthTexture, * moonTexture, * marioTexture;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 earthModelMatrix, moonModelMatrix, marioModelMatrix;
	bool drawInWireMode;

	Vec3 lightPos;

public:
	explicit SceneSTG();
	virtual ~SceneSTG();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE0_H