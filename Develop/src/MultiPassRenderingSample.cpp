#include "MultiPassRenderingSample.h"

MYRESULT MultiPassRendering::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// ビューポートセット
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<FLOAT>(window.GetWindowSize().cx);
	_viewport.Height = static_cast<FLOAT>(window.GetWindowSize().cy);

	// シザー矩形セット
	_scissorRect.left = 0;
	_scissorRect.top = 0;
	_scissorRect.right = window.GetWindowSize().cx;
	_scissorRect.bottom = window.GetWindowSize().cy;

	// レンダーターゲット生成
	RenderTargetData renderTargetData;
	renderTargetData.renderTargetBufferData 
		= RenderTargetBufferData(DXGI_FORMAT_R8G8B8A8_UNORM, 1280, 720, { 0.f,1.f,1.f,1.f });

	MYRESULT result = _renderTarget.Create(graphicsEngine.Device(), renderTargetData);
	if (result == MYRESULT::FAILED) { return result; }

	// テクスチャ生成
	result = _texture.CreateTextureFromWIC(graphicsEngine, L"草.JPG");
	if (result == MYRESULT::FAILED) { return result; }

	SpriteData data;
	data.vertexShaderData = ShaderData(L"src/TextureMapSampleVertex.hlsl", "TexMapVS", "vs_5_0");
	data.pixelShaderData = ShaderData(L"src/TextureMapSamplePixel.hlsl", "TexMapPS", "ps_5_0");
	data.textures[0] = &_texture;

	result = _myself.Create(graphicsEngine, data);
	if (result == MYRESULT::FAILED) { return result; }

	data.vertexShaderData = ShaderData(L"src/OffscreenVertexShader.hlsl", "OffscreenVS", "vs_5_0");
	data.pixelShaderData = ShaderData(L"src/OffscreenPixelShader.hlsl", "OffscreenPS", "ps_5_0");
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
