#include "Dx12ApplicationImpl.h"

#include "SkySphere.h"
#include "SphereGeometry.h"

class SphereMapSample : public Dx12ApplicationImpl
{
public:
	MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window);
	void Update(float deltaTime);
	void Draw(Dx12GraphicsEngine& graphicsEngine);
	void Final();
private:
	SkySphere skySphere;
	SphereGeometry sphere;
	CD3DX12_VIEWPORT _viewport;			        // ビューポート
	CD3DX12_RECT _scissorRect;			        // シザー矩形
};