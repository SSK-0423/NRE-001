#include "Texture.h"

HRESULT Texture::LoadTextureFromWICFile(const std::wstring& texturePath)
{
	// �t�@�C���ǂݍ���
	HRESULT result = DirectX::LoadFromWICFile(
		texturePath.c_str(), DirectX::WIC_FLAGS_NONE, &_metaData, _scratchImage);
	if (FAILED(result)) { return result; }

	// �e�N�X�`���̐��f�[�^�擾
	_image = _scratchImage.GetImage(0, 0, 0);

	return result;
}

HRESULT Texture::LoadTextureFromDDSFile(const std::wstring& texturePath)
{
	// �t�@�C���ǂݍ��݁@
	HRESULT result = DirectX::LoadFromDDSFile(
		texturePath.c_str(), DirectX::DDS_FLAGS_NONE, &_metaData, _scratchImage);
	if (FAILED(result)) { return result; }

	// �e�N�X�`���̐��f�[�^�擾
	_image = _scratchImage.GetImage(0, 0, 0);

	return E_NOTIMPL;
}

HRESULT Texture::CreateUploadAndTextureBuffer(ID3D12Device& device)
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

HRESULT Texture::MapTexture()
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

HRESULT Texture::CopyTexture(ID3D12Device& device,Dx12GraphicsEngine& graphicsEngine)
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

MYRESULT Texture::CreateTextureFromWIC(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath)
{
	ID3D12Device& device = graphicsEngine.Device();

	// �t�@�C���ǂݍ���
	if (FAILED(LoadTextureFromWICFile(texturePath))) { return MYRESULT::FAILED; }
	// �o�b�t�@�[����
	if (FAILED(CreateUploadAndTextureBuffer(device))) { return MYRESULT::FAILED; }
	// �}�b�v����
	if (FAILED(MapTexture())) { return MYRESULT::FAILED; }
	// �A�b�v���[�h�o�b�t�@�[�̓��e���e�N�X�`���o�b�t�@�[�փR�s�[
	if (FAILED(CopyTexture(device, graphicsEngine))) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

MYRESULT Texture::CreateTextureFromDDS(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath)
{
	ID3D12Device& device = graphicsEngine.Device();

	// �t�@�C���ǂݍ���
	if (FAILED(LoadTextureFromDDSFile(texturePath))) { return MYRESULT::FAILED; }
	// �o�b�t�@�[����
	if (FAILED(CreateUploadAndTextureBuffer(device))) { return MYRESULT::FAILED; }
	// �}�b�v����
	if (FAILED(MapTexture())) { return MYRESULT::FAILED; }
	// �A�b�v���[�h�o�b�t�@�[�̓��e���e�N�X�`���o�b�t�@�[�փR�s�[
	if (FAILED(CopyTexture(device, graphicsEngine))) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

void Texture::CreateTextureFromRenderTarget(RenderTargetBuffer& renderTargetBuffer)
{
	auto textureBuffer = _textureBuffer.Get();
	textureBuffer = &renderTargetBuffer.GetBuffer();

	// �V�F�[�_�[���\�[�X�Ƃ��ēo�^����ۂɕK�v�ȏ����擾
	DirectX::Image* img = new DirectX::Image();
	img->format = renderTargetBuffer.GetBuffer().GetDesc().Format;
	_image = img;

	_metaData.mipLevels = renderTargetBuffer.GetBuffer().GetDesc().MipLevels;
}
