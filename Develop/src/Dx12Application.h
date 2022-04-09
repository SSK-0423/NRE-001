#pragma once

#include <Windows.h>
#include <memory>

#include "EngineUtility.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "Polygon.h"

#include "Texture.h"
#include "ConstantBuffer.h"
#include "DescriptorHeapCBV_SRV_UAV.h"

#include "RenderTargetBuffer.h"
#include "OffScreenRender.h"

#include <DirectXMath.h>
#include <DirectXTex.h>

#pragma comment(lib,"DirectXTex.lib")

class Dx12GraphicsEngine;
class AppWindow;

/// <summary>
/// �����_�����O�J�����̃e�X�g�p�N���X
/// </summary>
class Dx12Application
{
public:
	Dx12Application();
	~Dx12Application();

	/// <summary>
	/// �A�v���P�[�V�����̏�����
	/// </summary>
	/// <returns>�����FMYRESULT::SUCCESS ���s�FMYRESULT::FAILED</returns>
	MYRESULT Init();

	/// <summary>
	/// ���C�����[�v
	/// </summary>
	void Run();

	/// <summary>
	/// �I������
	/// </summary>
	void End();

private:
	Dx12GraphicsEngine& _graphicsEngine;			    // �`��̊�b������S������G���W��
	std::shared_ptr<AppWindow> _window = nullptr;		// �A�v���P�[�V�����̃E�B���h�E
// ��

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	// �J���p
private:
	VertexBuffer _vertexBuffer;			// ���_�o�b�t�@�[
	IndexBuffer _indexBuffer;			// �C���f�b�N�X�o�b�t�@�[
	Shader _vertexShader;				// ���_�V�F�[�_�[
	Shader _pixelShader;				// �s�N�Z���V�F�[�_�[
	RootSignature _rootSignature;		// ���[�g�V�O�l�`��
	MyFrameWork::Polygon _triangle;		// �O�p�`�|���S��
	MyFrameWork::Polygon _square;		// �l�p�`�|���S��
	CD3DX12_VIEWPORT _viewport;			// �r���[�|�[�g
	CD3DX12_RECT _scissorRect;			// �V�U�[��`

	// �e�N�X�`���}�b�s���O�����p
private:
	DescriptorHeapCBV_SRV_UAV _polygonHeap;
	Texture _texture;
	ConstantBuffer _constantBuffer;

	struct PolygonConstantBuffer {
		DirectX::XMMATRIX _worldViewProj;
		DirectX::XMMATRIX _matrix;
	};

	PolygonConstantBuffer _polygonConstantBuffer;

	float _angle = 0.f;
	
	/// <summary>
	/// �e�N�X�`��������
	/// </summary>
	/// <returns></returns>
	MYRESULT InitTexture();
	/// <summary>
	/// �R���X�^���g�o�b�t�@�\������
	/// </summary>
	/// <returns></returns>
	MYRESULT InitConstantBuffer();

	/// <summary>
	/// �e�N�X�`���}�b�s���O�E�R���X�^���g�o�b�t�@�\�m�F�p
	/// </summary>
	void TextureMappingDraw();

	// �}���`�p�X�����_�����O�����p
private:
	OffScreenRender _firstPassRender;
	OffScreenRender _secondPassRender;

	/// <summary>
	/// �I�t�X�N���[�������_�[������
	/// </summary>
	/// <returns></returns>
	MYRESULT InitOffscreenRender();
	/// <summary>
	/// �}���`�p�X�����_�����O�m�F�p
	/// </summary>
	void MultiPassRenderingDraw();
	
	// �}���`�����_�[�^�[�Q�b�g
private:
	MyFrameWork::Polygon _mrtPolygon;
	VertexBuffer _mrtPolygonVB;					
	IndexBuffer _mrtPolygonIB;					
	Shader _mrtPolygonVS;	                    
	Shader _mrtPolygonPS;

	OffScreenRender _mrtRenders[2];
	OffScreenRender _mrtSecondRender;

	DescriptorHeapCBV_SRV_UAV _mrtDescriptorHeap;

	/// <summary>
	/// �}���`�����_�[�^�[�Q�b�g������
	/// </summary>
	/// <returns></returns>
	MYRESULT InitMultiRenderTarget();

	/// <summary>
	/// MRT�|���S������
	/// </summary>
	/// <returns></returns>
	MYRESULT CreateMRTPolygon();

	/// <summary>
	/// MRT�����_�[�^�[�Q�b�g����
	/// </summary>
	/// <returns></returns>
	MYRESULT CreateMRTRenders();

	/// <summary>
	/// MRT�p�̃f�B�X�N���v�^�q�[�v����
	/// </summary>
	/// <returns></returns>
	MYRESULT CreateMRTDescriptorHeap();

	/// <summary>
	/// �}���`�����_�[�^�[�Q�b�g�m�F�p
	/// </summary>
	void MultiRenderTargetDraw();
};

/// ����
/// DirectX�̏������ɃE�B���h�E�n���h���K�v(HWND)
/// ���V���O���g�������ăE�B���h�E�T�C�Y�擾�ł���悤�ɂ�����
/// ���F��ȃN���X����Appication�N���X���Ăяo���H�ڂɂȂ�
/// ���A�v���P�[�V�����N���X�̓A�v���P�[�V�����ŗL�̏������L�q���邽�߂̃N���X
///	���V�X�e�����̃N���X���A�v���P�[�V�����N���X�Ɗ֌W�������Ƃ͔�����
/// ��main.cpp�ł̂݌Ăяo���悤�Ȑ݌v�ɂ��遨�V���O���g��������K�v�Ȃ�