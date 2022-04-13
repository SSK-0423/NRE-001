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

	// テクスチャ生成
	MYRESULT result = _texture.CreateTextureFromWIC(graphicsEngine, L"Ramen.JPG");
	if (result == MYRESULT::FAILED) { return result; }

	SpriteData data;
	data.vertexShaderData = ShaderData(L"src/TextureMapSampleVertex.hlsl", "TexMapVS", "vs_5_0");
	data.pixelShaderData = ShaderData(L"src/TextureMapSamplePixel.hlsl", "TexMapPS", "ps_5_0");
	data.textures[0] = &_texture;
	data.rootSignature = _rootSignature;

	result = _sprite.Create(graphicsEngine, data);
	if (result == MYRESULT::FAILED) { return result; }

	return result;
}

void MultiPassRendering::Update()
{
}

void MultiPassRendering::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	renderContext.SetGraphicsRootSignature(_rootSignature);
	renderContext.SetViewport(_viewport);
	renderContext.SetScissorRect(_scissorRect);

	_sprite.Draw(renderContext);
}

void MultiPassRendering::Final()
{
}
