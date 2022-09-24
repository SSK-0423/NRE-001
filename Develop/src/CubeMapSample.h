#include "Dx12ApplicationImpl.h"

#include "SkySphere.h"
#include "SphereGeometry.h"
#include "CubeGeometry.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "Texture.h"

class CubeMapSample : public Dx12ApplicationImpl
{
public:
	MYRESULT Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window);
	void Update(float deltaTime);
	void Draw(Dx12GraphicsEngine& graphicsEngine);
	void Final();

private:
	SkySphere skySphere;
	SphereGeometry _sphere;
	CubeGeometry _cube;
	Texture _cubeTexture;
	
	CD3DX12_VIEWPORT _viewport;			        // ビューポート
	CD3DX12_RECT _scissorRect;			        // シザー矩形

	struct ConstBuff {
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldViewProj;
		DirectX::XMFLOAT3 eye;
	};

	ConstBuff _cbuffData;
	ConstantBuffer _constantBuffer;
	
	float _angle;

	DirectX::XMMATRIX _view;
	DirectX::XMMATRIX _proj;

	MYRESULT SetConstantBuffer(Dx12GraphicsEngine& graphicsEnigne, AppWindow& window);
};