#include "MultiPassRenderingSample.h"

MYRESULT MultiPassRendering::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// �r���[�|�[�g�Z�b�g
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<FLOAT>(window.GetWindowSize().cx);
	_viewport.Height = static_cast<FLOAT>(window.GetWindowSize().cy);

	// �V�U�[��`�Z�b�g
	_scissorRect.left = 0;
	_scissorRect.top = 0;
	_scissorRect.right = window.GetWindowSize().cx;
	_scissorRect.bottom = window.GetWindowSize().cy;

	// �����_�[�^�[�Q�b�g����
	RenderTargetData renderTargetData;
	renderTargetData.renderTargetBufferData 
		= RenderTargetBufferData(DXGI_FORMAT_R8G8B8A8_UNORM, 1280, 720, { 0.f,1.f,1.f,1.f });

	MYRESULT result = _renderTarget.Create(graphicsEngine.Device(), renderTargetData);
	if (result == MYRESULT::FAILED) { return result; }

	// �e�N�X�`������
	result = _texture.CreateTextureFromWIC(graphicsEngine, L"��.JPG");
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

	// 1�p�X�ڂ̃����_�����O
	_renderTarget.BeginRendering(renderContext, _viewport, _scissorRect);
	{
		_myself.Draw(renderContext);
	}
	_renderTarget.EndRendering(renderContext);

	// �ŏI�p�X�����_�����O
	// �t���[�������_�[�^�[�Q�b�g�Z�b�g
	graphicsEngine.SetFrameRenderTarget(_viewport, _scissorRect);
	{
		_sprite.Draw(renderContext);
	}
}

void MultiPassRendering::Final()
{
}
