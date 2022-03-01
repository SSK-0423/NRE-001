#include "Dx12Application.h"
#include "Dx12GraphicsEngine.h"
#include "AppWindow.h"

#include <iostream>

const LONG WINDOW_WIDTH = 1280;
const LONG WINDOW_HEIGHT = 720;

Dx12Application::Dx12Application() : _graphicsEngine(Dx12GraphicsEngine::Instance())
{
}

Dx12Application::~Dx12Application()
{
}

MYRESULT Dx12Application::Init()
{
	// �E�B���h�E������
	_window = std::make_shared<AppWindow>();
	AppWindowInitData initData(TEXT("�����_�����O�G���W���J��"), WINDOW_WIDTH, WINDOW_HEIGHT);
	_window->CreateAppWindow(initData);

	// �O���t�B�N�X�G���W��������
	SIZE wndSize = _window->GetWindowSize();
	MYRESULT result = _graphicsEngine.Init(_window->GetHwnd(), wndSize.cx, wndSize.cy);

	// �|���S���֘A������(�e�X�g�p)
	std::vector<DirectX::XMFLOAT3> vertex;
	vertex.push_back({ -0.5f,-0.7f,0.f });	// ����
	vertex.push_back({ 0.f,0.7f,0.f });	    // �^��
	vertex.push_back({ 0.5f,-0.7f,0.f });	// �E��

	std::vector<PolygonVertex> polygonVertex;
	polygonVertex.push_back({ { -0.5f,-0.7f	,0.f }	,{0.f,1.f} });
	polygonVertex.push_back({ { 0.f  ,0.7f	,0.f }	,{0.5f,0.f} });
	polygonVertex.push_back({ { 0.5f ,-0.7f	,0.f}	,{1.f,1.f} });

	//result = _vertexBuffer.Create(_graphicsEngine.Device(), vertex);
	result = _vertexBuffer.Create(
		_graphicsEngine.Device(), (void*)&polygonVertex[0],
		SizeofVector<PolygonVertex>(polygonVertex), sizeof(PolygonVertex));

	std::vector<UINT> index;
	index.push_back(0); index.push_back(1); index.push_back(2);

	result = _indexBuffer.Create(_graphicsEngine.Device(), index);

	RootSignatureData data;
	result = _rootSignature.Create(_graphicsEngine.Device(), data);

	// �V�F�[�_�[
	result = _vertexShader.Create(L"src/PolygonVertexShader.hlsl", "PolygonVS", "vs_5_0");
	result = _pixelShader.Create(L"src/PolygonPixelShader.hlsl", "PolygonPS", "ps_5_0");

	// �|���S������
	PolygonData polygonData;
	polygonData._vertexBuffer = _vertexBuffer;
	polygonData._indexBuffer = _indexBuffer;
	polygonData._vertexShader = _vertexShader;
	polygonData._pixelShader = _pixelShader;
	polygonData._rootSignature = _rootSignature;
	polygonData._inputLayout.push_back
	(
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	);
	polygonData._inputLayout.push_back
	(
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	);

	result = _triangle.Create(_graphicsEngine.Device(), polygonData);

	// �l�p�`�|���S��
	//vertex.resize(4);
	//vertex[0] = { -1.f,-1.f,0.f };
	//vertex[1] = { -1.f,1.f,0.f };
	//vertex[2] = { 0.f,-1.f,0.f };
	//vertex[3] = { 0.f,1.f,0.f };
	//result = _vertexBuffer.Create(_graphicsEngine.Device(), vertex);

	//index.push_back(2);	index.push_back(1); index.push_back(3);
	//result = _indexBuffer.Create(_graphicsEngine.Device(), index);

	//polygonData._vertexBuffer = _vertexBuffer;
	//polygonData._indexBuffer = _indexBuffer;

	//result = _square.Create(_graphicsEngine.Device(), polygonData);

	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<FLOAT>(_window->GetWindowSize().cx);
	_viewport.Height = static_cast<FLOAT>(_window->GetWindowSize().cy);

	_scissorRect.left = 0;
	_scissorRect.top = 0;
	_scissorRect.right = _window->GetWindowSize().cx;
	_scissorRect.bottom = _window->GetWindowSize().cy;

	result = InitTexture();

	return result;
}

void Dx12Application::Run()
{
	// �E�B���h�E���쐬����Ă��Ȃ��Ȃ炻�̎��_�ŏI��
	if (_window == nullptr) { return; }

	// �Q�[�����[�v
	while (_window->DispatchWindowMessage())
	{
		// ������G���W��(�t���[�����[�N��)�ɋz��������H�H�H�H
		Update();
		Draw();
	}
}

void Dx12Application::End()
{
}

void Dx12Application::Update()
{
}

void Dx12Application::Draw()
{
	// 1�t���[���̕`�揈��
	_graphicsEngine.BeginDraw();
	{
		// ���f���Ȃǂ̕`��
		_graphicsEngine.GetRenderingContext().SetGraphicsRootSignature(_rootSignature);
		_graphicsEngine.GetRenderingContext().SetViewport(_viewport);
		_graphicsEngine.GetRenderingContext().SetScissorRect(_scissorRect);

		// �e�N�X�`���p�̐ݒ�
		_graphicsEngine.GetRenderingContext().SetDescriptorHeap(_textureHeap.GetAddressOf());
		_graphicsEngine.GetRenderingContext().SetGraphicsRootDescriptorTable(
			0, _textureHeap->GetGPUDescriptorHandleForHeapStart());

		// �`��
		_triangle.Draw(_graphicsEngine.GetRenderingContext());
		//_square.Draw(_graphicsEngine.GetRenderingContext());
	}
	_graphicsEngine.EndDraw();
}















MYRESULT Dx12Application::InitTexture()
{
	ID3D12Device& device = _graphicsEngine.Device();

	std::wstring path(L"textest.png");
	_texture.CreateTextureFromWIC(_graphicsEngine, path);
	if (FAILED(CreateTextureHeap(device))) { return MYRESULT::FAILED; }
	CreateTextureSRV(device);

	//// �t�@�C���ǂݍ���
	//if (FAILED(LoadTextureFile(L"textest.png"))) { return MYRESULT::FAILED; }
	//// ���\�[�X����
	//if (FAILED(CreateTextureResource(device))) { return MYRESULT::FAILED; }
	//// �}�b�v
	//if (FAILED(MapTexture())) { return MYRESULT::FAILED; }
	//// ���ԃo�b�t�@�[�̓��e���e�N�X�`���o�b�t�@�[�ɃR�s�[
	//if (FAILED(CopyTexture(device, _graphicsEngine))) { return MYRESULT::FAILED; }
	//// �f�B�X�N���v�^�q�[�v����
	//if (FAILED(CreateTextureHeap(device))) { return MYRESULT::FAILED; }
	//// �r���[����
	//CreateTextureSRV(device);

	return MYRESULT::SUCCESS;
}

HRESULT Dx12Application::LoadTextureFile(const wchar_t* path)
{
	HRESULT result = DirectX::LoadFromWICFile(
		path, DirectX::WIC_FLAGS_NONE, &_metaData, _scratchImage);
	// �e�N�X�`���̐��f�[�^�擾
	_image = _scratchImage.GetImage(0, 0, 0);

	return result;
}

HRESULT Dx12Application::CreateTextureResource(ID3D12Device& device)
{
	// �A�b�v���[�h�p�̒��ԃo�b�t�@�[����
	CD3DX12_HEAP_PROPERTIES uploadHeapProp(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC uploadResDesc = CD3DX12_RESOURCE_DESC::Buffer(
		AlignmentedSize(_image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * _image->height);

	HRESULT result = device.CreateCommittedResource(
		&uploadHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&uploadResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(_uploadBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	// �A�b�v���[�h��̃o�b�t�@�[����
	CD3DX12_HEAP_PROPERTIES texHeapProp(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC texResDesc;
	texResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(_metaData.dimension);
	texResDesc.Alignment = 0;
	texResDesc.Width = _metaData.width;
	texResDesc.Height = _metaData.height;
	texResDesc.DepthOrArraySize = _metaData.arraySize;
	texResDesc.MipLevels = _metaData.mipLevels;
	texResDesc.Format = _metaData.format;
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 0;
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	result = device.CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(_textureBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	return result;
}

HRESULT Dx12Application::MapTexture()
{
	// �}�b�v
	uint8_t* texMap = nullptr;
	HRESULT result = _uploadBuffer->Map(0, nullptr, (void**)&texMap);
	if (FAILED(result)) { return result; }

	// �f�[�^�R�s�[
	uint8_t* srcAddress = _image->pixels;;
	size_t alignRowPitch = AlignmentedSize(_image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

	for (size_t y = 0; y < _image->height; y++) {
		std::copy_n(srcAddress, alignRowPitch, texMap);
		srcAddress += _image->rowPitch;
		texMap += alignRowPitch;
	}
	_uploadBuffer->Unmap(0, nullptr);

	return result;
}

HRESULT Dx12Application::CopyTexture(
	ID3D12Device& device, Dx12GraphicsEngine& graphicsEngine)
{
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();
	ID3D12CommandAllocator& cmdAllocator = graphicsEngine.CmdAllocator();
	ID3D12CommandQueue& cmdQueue = graphicsEngine.CmdQueue();

	// �R�s�[��
	D3D12_TEXTURE_COPY_LOCATION src = {};
	src.pResource = _uploadBuffer.Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint.Offset = 0;
	src.PlacedFootprint.Footprint.Width = _metaData.width;
	src.PlacedFootprint.Footprint.Height = _metaData.height;
	src.PlacedFootprint.Footprint.Depth = _metaData.depth;
	src.PlacedFootprint.Footprint.RowPitch =
		AlignmentedSize(_image->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
	src.PlacedFootprint.Footprint.Format = _image->format;

	// �R�s�[��
	D3D12_TEXTURE_COPY_LOCATION dst = {};
	dst.pResource = _textureBuffer.Get();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;

	// �t�F���X����
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	UINT fenceVal = 0;
	HRESULT result = device.CreateFence(
		fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	// ���ԃo�b�t�@�[�̓��e���R�s�[��o�b�t�@�[�փR�s�[
	{
		renderContext.CopyTextureRegion(src, dst);
		renderContext.TransitionResourceState(
			*_textureBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_PRESENT);

		renderContext.Close();

		// �R�}���h���X�g���s
		ID3D12CommandList* cmdLists[] = { &graphicsEngine.CmdList() };
		cmdQueue.ExecuteCommandLists(_countof(cmdLists), cmdLists);
		cmdQueue.Signal(fence.Get(), ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal) {
			// �C�x���g�n���h���擾
			auto event = CreateEvent(nullptr, false, false, nullptr);

			fence->SetEventOnCompletion(fenceVal, event);

			// �C�x���g����������܂ő҂�������
			WaitForSingleObject(event, INFINITE);

			// �C�x���g�n���h�������
			CloseHandle(event);
		}

		cmdAllocator.Reset();
		renderContext.Reset(cmdAllocator);
	}

	return result;
}

HRESULT Dx12Application::CreateTextureHeap(ID3D12Device& device)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = 1;
	heapDesc.NodeMask = 0;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	HRESULT result = device.CreateDescriptorHeap(
		&heapDesc, IID_PPV_ARGS(_textureHeap.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	return result;
}

void Dx12Application::CreateTextureSRV(ID3D12Device& device)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _texture.GetImage().format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = _texture.GetTexMetadata().mipLevels;

	device.CreateShaderResourceView(
		&_texture.GetBuffer(), &srvDesc, _textureHeap->GetCPUDescriptorHandleForHeapStart());

	//device.CreateShaderResourceView(
	//	_textureBuffer.Get(), &srvDesc, _textureHeap->GetCPUDescriptorHandleForHeapStart());
}
