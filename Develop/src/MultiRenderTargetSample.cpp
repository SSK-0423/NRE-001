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
	renderTargetData.depthStencilBufferData.width = window.GetWindowSize().cx;
	renderTargetData.depthStencilBufferData.height = window.GetWindowSize().cy;

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

	// ���[�g�V�O�l�`���ݒ�
	RootSignatureData rootSigData;
	rootSigData._descRangeData.srvDescriptorNum = 2;

	spriteData.rootSignatureData = rootSigData;

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

/*
 * 1�p�X�ځF2�̃����_�[�^�[�Q�b�g�֕`��
 * �ŏI�p�X�F�X�v���C�g�ɂ����2�̃����_�[�^�[�Q�b�g�̕`�挋�ʂ�\������
 */
void MultiRenderTargetSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

	// �P�p�X�ڂ̃����_�����O
	RenderTarget::BeginMultiRendering(
		_renderTargets, _countof(_renderTargets), renderContext, _viewport, _scissorRect);
	{
		_firstSprite.Draw(renderContext);
	}
	RenderTarget::EndMultiRendering(
		_renderTargets, _countof(_renderTargets), renderContext, _viewport, _scissorRect);

	// �t���[���o�b�t�@�[�փ����_�����O
	graphicsEngine.SetFrameRenderTarget(_viewport, _scissorRect);
	{
		_finalSprite.Draw(renderContext);
	}
}

void MultiRenderTargetSample::Final()
{
}
