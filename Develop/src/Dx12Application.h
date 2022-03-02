#pragma once

#include <Windows.h>
#include <memory>

#include "EngineUtility.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "Polygon.h"

#include "Texture.h"
#include "DescriptorHeapCBV_SRV_UAV.h"

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
	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	Shader _vertexShader;
	Shader _pixelShader;
	RootSignature _rootSignature;
	MyFrameWork::Polygon _triangle;
	MyFrameWork::Polygon _square;
	CD3DX12_VIEWPORT _viewport;
	CD3DX12_RECT _scissorRect;

	DescriptorHeapCBV_SRV_UAV _textureHeap;
	Texture _texture;

	// �e�N�X�`���}�b�s���O�����p
private:
	MYRESULT InitTexture();
};

/// ����
/// DirectX�̏������ɃE�B���h�E�n���h���K�v(HWND)
/// ���V���O���g�������ăE�B���h�E�T�C�Y�擾�ł���悤�ɂ�����
/// ���F��ȃN���X����Appication�N���X���Ăяo���H�ڂɂȂ�
/// ���A�v���P�[�V�����N���X�̓A�v���P�[�V�����ŗL�̏������L�q���邽�߂̃N���X
///	���V�X�e�����̃N���X���A�v���P�[�V�����N���X�Ɗ֌W�������Ƃ͔�����
/// ��main.cpp�ł̂݌Ăяo���悤�Ȑ݌v�ɂ��遨�V���O���g��������K�v�Ȃ�