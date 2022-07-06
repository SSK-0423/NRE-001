#pragma once
#include <string>

#include "IMaterial.h"

class PBRMaterial : public IMaterial {
public:
	PBRMaterial();
	~PBRMaterial();
private:
	std::wstring baseColorName;
	std::wstring metallicName;
	std::wstring roughnessName;
	std::wstring normalName;
	std::wstring occlusionName;

public:
	
};