#include "MultiPassRenderingSample.h"

MYRESULT MultiPassRendering::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// ビューポートセット
	_viewport = CD3DX12_VIEWPORT(0.f, 0.f,
		static_cast<FLOAT>(window.GetWindowSize().cx),
		static_cast<FLOAT>(window.GetWindowSize().cy));

	// シザー矩形セット
	_scissorRect = CD3DX12_RECT(0, 0, window.GetWindowSize().cx, window.GetWindowSize().cy);

	// レンダーターゲット生成
	RenderTargetData renderTargetData;
	renderTargetData.renderTargetBufferData 
		= RenderTargetBufferData(DXGI_FORMAT_R8G8B8A8_UNORM, 1280, 720, { 0.f,1.f,1.f,1.f });
	renderTargetData.depthStencilBufferData.width = window.GetWindowSize().cx;
	renderTargetData.depthStencilBufferData.height = window.GetWindowSize().cy;

	MYRESULT result = _renderTarget.Create(graphicsEngine.Device(), renderTargetData);
	if (result == MYRESULT::FAILED) { return result; }

	// テクスチャ生成
	result = _texture.CreateTextureFromWIC(graphicsEngine, L"Ramen.JPG");
	if (result == MYRESULT::FAILED) { return result; }

	SpriteData data;
	data.vertexShaderData = ShaderData(L"src/TextureMapSampleVertex.hlsl", "main", "vs_5_0");
	data.pixelShaderData = ShaderData(L"src/TextureMapSamplePixel.hlsl", "main", "ps_5_0");
	data.textures[0] = &_texture;

	result = _myself.Create(graphicsEngine, data);
	if (result == MYRESULT::FAILED) { return result; }

	data.vertexShaderData = ShaderData(L"src/OffscreenVertexShader.hlsl", "main", "vs_5_0");
	data.pixelShaderData = ShaderData(L"src/OffscreenPixelShader.hlsl", "main", "ps_5_0");
	data.textures[0] = &_renderTarget.GetRenderTargetTexture();

	result = _sprite.Create(graphicsEngine, data);
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

void MultiPassRendering::Update(float deltaTime)
{
}

void MultiPassRendering::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	// 1パス目のレンダリング
	_renderTarget.BeginRendering(renderContext, _viewport, _scissorRect);
	{
		_myself.Draw(renderContext);
	}
	_renderTarget.EndRendering(renderContext);

	// 最終パスレンダリング
	// フレームレンダーターゲットセット
	graphicsEngine.SetFrameRenderTarget(_viewport, _scissorRect);
	{
		_sprite.Draw(renderContext);
	}
}

void MultiPassRendering::Final()
{
}
