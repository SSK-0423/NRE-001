#pragma once
#include "NamelessEngine.h"
#include "Camera.h"

class PBRScene : public NamelessEngine::Scene::Scene {
public:
	PBRScene(unsigned int window_width, unsigned int window_height);
	~PBRScene();

	NamelessEngine::Utility::RESULT Init();
	void Update(float deltaTime);
	void Draw();
	void Final();
private:
	NamelessEngine::Scene::Camera _camera;
};