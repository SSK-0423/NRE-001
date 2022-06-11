#pragma once
#include <fbxsdk.h>

#include "Singleton.h"
#include "EngineUtility.h"

class FBXLoader : public Singleton<FBXLoader>{
	friend Singleton<FBXLoader>;
private:
	FBXLoader();
	~FBXLoader();

public:
	MYRESULT Init(const char* meshPath);
};