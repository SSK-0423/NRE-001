#pragma once
#include "../NamelessEngine.h"

class PBRScene : public NamelessEngine::Scene::Scene {
public:
	PBRScene();
	~PBRScene();

	NamelessEngine::Utility::RESULT ImplInit() override;
	void Update(float deltaTime) override;
	void Final() override;
};