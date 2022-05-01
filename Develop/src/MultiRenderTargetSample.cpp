#include "MultiRenderTargetSample.h"

MYRESULT MultiRenderTargetSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
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

	MYRESULT result = MYRESULT::FAILED;
	for (auto& render : _renderTargets)
	{
		result = render.Create(graphicsEngine.Device(), renderTargetData);
		if (result == MYRESULT::FAILED) return result;
	}

	// �X�v���C�g����
	SpriteData spriteData;
	spriteData.vertexShaderData = ShaderData(L"src/MultiRenderTargetFirstVertex.hlsl", "MrtFirstVertex", "vs_5_0");
	spriteData.pixelShaderData = ShaderData(L"src/MultiRenderTargetFirstPixel.hlsl", "MrtFirstPixel", "ps_5_0");
	spriteData.colorFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;	// 2�ڂ̃����_�[�^�[�Q�b�g�̃t�H�[�}�b�g�Z�b�g

	// �P�p�X�ڗp�̃X�v���C�g����
	result = _firstSprite.Create(graphicsEngine, spriteData);
	if (result == MYRESULT::FAILED) return result;

	spriteData.vertexShaderData = ShaderData(L"src/MultiRenderTargetVertex.hlsl", "MrtVertex", "vs_5_0");
	spriteData.pixelShaderData = ShaderData(L"src/MultiRenderTargetPixel.hlsl", "MrtPixel", "ps_5_0");
	spriteData.colorFormats[1] = DXGI_FORMAT_UNKNOWN;

	// �����_�[�^�[�Q�b�g�̃e�N�X�`���Z�b�g
	for (size_t idx = 0; idx < _countof(_renderTargets); idx++)
	{
		spriteData.textures[idx] = &_renderTargets[idx].GetRenderTargetTexture();
	}

	// �ŏI�p�X�p�̃X�v���C�g����
	result = _finalSprite.Create(graphicsEngine, spriteData);
	if (result == MYRESULT::FAILED) return result;

	return result;
}

void MultiRenderTargetSample::Update(float deltaTime)
{
}

void MultiRenderTargetSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	// �����̃����_�[�^�[�Q�b�g�Z�b�g


	//graphicsEngine.SetFrameRenderTarget(_viewport, _scissorRect);
	//{
	//	_finalSprite.Draw(renderContext);
	//}

	/*
	* �ŏ��̕`��łQ�̃����_�[�^�[�Q�b�g�֕`��
	* �ŏI�p�X�łQ�̃����_�[�^�[�Q�b�g�̕`�挋�ʂ��e�N�X�`���Ƃ��Ď��X�v���C�g��`��
	*/
}

void MultiRenderTargetSample::Final()
{
}
