#pragma once

#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <string>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"dxguid.lib")

#include "Dx12GraphicsEngine.h"
#include "EngineUtility.h"
#include "RenderTargetBuffer.h"
#include "DepthStencilBuffer.h"

class Texture {
public:
	Texture() = default;
	~Texture() = default;

	Texture& operator=(const Texture& inst);
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _uploadBuffer = nullptr;		// 中間バッファー(アップロード元)
	Microsoft::WRL::ComPtr<ID3D12Resource> _textureBuffer = nullptr;	// テクスチャバッファー(アップロード先)

	std::vector<D3D12_SUBRESOURCE_DATA> _subresources; // キューブテクスチャ用サブリソース

	// ScratchImage::GetImageの戻り値にconstがついているため
	const DirectX::Image* _image = nullptr;	// テクスチャの生データ
	DirectX::TexMetadata _metaData;	        // テクスチャのメタ情報
	DirectX::ScratchImage _scratchImage;    // 

	/// <summary>
	/// WIC対応のファイル(bmp,png,jpeg,tiffなど)を読み込む
	/// </summary>
	/// <param name="texPath">テクスチャへのパス</param>
	/// <returns></returns>
	HRESULT LoadTextureFromWICFile(const std::wstring& texturePath);

	/// <summary>
	/// DDSファイル読み込み
	/// </summary>
	/// <param name="texturePath">テクスチャへのパス</param>
	/// <returns></returns>
	HRESULT LoadTextureFromDDSFile(const std::wstring& texturePath);

	/// <summary>
	/// 中間バッファーとテクスチャバッファー生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns></returns>
	HRESULT CreateUploadAndTextureBuffer(ID3D12Device& device);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	HRESULT CreateUploadAndCubeTextureBuffer(ID3D12Device& device);

	/// <summary>
	/// マップ処理
	/// </summary>
	/// <returns></returns>
	HRESULT MapTexture();

	/// <summary>
	/// アップロードバッファーの内容をテクスチャバッファーへコピー
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="graphicsEngine">グラフィクスエンジン</param>
	/// <returns></returns>
	HRESULT CopyTexture(ID3D12Device& device, Dx12GraphicsEngine& graphicsEngine);

	HRESULT CopyCubeTexture(Dx12GraphicsEngine& graphicsEngine);

	void SetTextureData(
		std::vector<ColorRGBA>& data, const size_t& width, const size_t& height, const DXGI_FORMAT& format);

public:
	/// <summary>
	/// WIC対応のファイル((bmp,png,jpeg,tiffなど)からテクスチャを生成
	/// </summary>
	/// <param name="graphicsEngine">グラフィクスエンジン</param>
	/// <param name="texturePath">テクスチャへのパス</param>
	/// <returns>成功：MYRESULT::SUCCESS 失敗：MYRESULT::FAILED</returns>
	MYRESULT CreateTextureFromWIC(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath);

	/// <summary>
	/// DDSファイルからテクスチャを生成
	/// </summary>
	/// <param name="graphicsEngine">グラフィクスエンジン</param>
	/// <param name="texturePath">テクスチャへのパス</param>
	/// <returns>成功：MYRESULT::SUCCESS 失敗：MYRESULT::FAILED</returns>
	MYRESULT CreateTextureFromDDS(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath);

	/// <summary>
	/// 用意したRGBAデータからテクスチャ生成
	/// </summary>
	/// <param name="graphicsEngine"></param>
	/// <param name="data"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	MYRESULT CreateTextureFromRGBAData(
		Dx12GraphicsEngine& graphicsEngine, std::vector<ColorRGBA>& data,
		const size_t& width, const size_t& height, const DXGI_FORMAT& format);

	/// <summary>
	/// レンダーターゲットからテクスチャ生成
	/// </summary>
	/// <param name="renderTargetBuffer">レンダーターゲットバッファー</param>
	void CreateTextureFromRenderTarget(RenderTargetBuffer& renderTargetBuffer);

	/// <summary>
	/// デプスステンシルバッファーからテクスチャ生成
	/// </summary>
	/// <param name="depthStencilBuffer">デプスステンシルバッファー</param>
	void CreateTextureFromDepthStencil(DepthStencilBuffer& depthStencilBuffer);

	MYRESULT CreateCubeTextureFromDDS(Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePath);

	/// <summary>
	/// テクスチャバッファー取得
	/// </summary>
	/// <returns>テクスチャバッファー</returns>
	ID3D12Resource& GetBuffer() const {
		return *_textureBuffer.Get();
	}

	/// <summary>
	/// テクスチャのメタデータ取得
	/// </summary>
	/// <returns>テクスチャのメタデータ</returns>
	DirectX::TexMetadata& GetTexMetadata() {
		return _metaData;
	}

	/// <summary>
	/// テクスチャの生データ取得
	/// </summary>
	/// <returns>テクスチャの生データ</returns>
	const DirectX::Image& GetImage() {
		return *_image;
	}
};