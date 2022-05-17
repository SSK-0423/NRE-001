#include "MultiRenderTargetSample.h"

MYRESULT MultiRenderTargetSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
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
	renderTargetData.depthStencilBufferData.width = window.GetWindowSize().cx;
	renderTargetData.depthStencilBufferData.height = window.GetWindowSize().cy;

	MYRESULT result = MYRESULT::FAILED;
	for (auto& render : _renderTargets)
	{
		result = render.Create(graphicsEngine.Device(), renderTargetData);
		if (result == MYRESULT::FAILED) return result;
	}

	// スプライト生成
	SpriteData spriteData;
	spriteData.vertexShaderData = ShaderData(L"src/MultiRenderTargetFirstVertex.hlsl", "MrtFirstVertex", "vs_5_0");
	spriteData.pixelShaderData = ShaderData(L"src/MultiRenderTargetFirstPixel.hlsl", "MrtFirstPixel", "ps_5_0");
	spriteData.colorFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;	// 2つ目のレンダーターゲットのフォーマットセット

	// ルートシグネチャ設定
	RootSignatureData rootSigData;
	rootSigData._descRangeData.srvDescriptorNum = 2;

	spriteData.rootSignatureData = rootSigData;

	// １パス目用のスプライト生成
	result = _firstSprite.Create(graphicsEngine, spriteData);
	if (result == MYRESULT::FAILED) return result;

	spriteData.vertexShaderData = ShaderData(L"src/MultiRenderTargetVertex.hlsl", "MrtVertex", "vs_5_0");
	spriteData.pixelShaderData = ShaderData(L"src/MultiRenderTargetPixel.hlsl", "MrtPixel", "ps_5_0");
	spriteData.colorFormats[1] = DXGI_FORMAT_UNKNOWN;


	// レンダーターゲットのテクスチャセット
	for (size_t idx = 0; idx < _countof(_renderTargets); idx++)
	{
		spriteData.textures[idx] = &_renderTargets[idx].GetRenderTargetTexture();
	}

	// 最終パス用のスプライト生成
	result = _finalSprite.Create(graphicsEngine, spriteData);
	if (result == MYRESULT::FAILED) return result;

	return result;
}

void MultiRenderTargetSample::Update(float deltaTime)
{
}

/*
 * 1パス目：2つのレンダーターゲットへ描画
 * 最終パス：スプライトによって2つのレンダーターゲットの描画結果を表示する
 */
void MultiRenderTargetSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	// １パス目のレンダリング
	RenderTarget::BeginMultiRendering(
		_renderTargets, _countof(_renderTargets), renderContext, _viewport, _scissorRect);
	{
		_firstSprite.Draw(renderContext);
	}
	RenderTarget::EndMultiRendering(
		_renderTargets, _countof(_renderTargets), renderContext, _viewport, _scissorRect);

	// フレームバッファーへレンダリング
	graphicsEngine.SetFrameRenderTarget(_viewport, _scissorRect);
	{
		_finalSprite.Draw(renderContext);
	}
}

void MultiRenderTargetSample::Final()
{
}
