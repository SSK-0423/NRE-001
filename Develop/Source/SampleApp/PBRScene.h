#pragma once
#include "NamelessEngine.h"
#include "Camera.h"

class PBRScene : public NamelessEngine::Scene::Scene {
public:
	PBRScene();
	~PBRScene();

	NamelessEngine::Utility::RESULT Init();
	void Update(float deltaTime);
	void Draw();
	void Final();
private:
	NamelessEngine::Scene::Camera _camera;

};