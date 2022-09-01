#include "Dx12ApplicationImpl.h"
#include "SphereGeometry.h"

class SphereMapSample : public Dx12ApplicationImpl 
{
public:
	MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& appWindow);
	void Update(float deltaTime);
	void Draw(Dx12GraphicsEngine& graphicsEngine);
	void Final();
private:
	SphereGeometry sphere;

};