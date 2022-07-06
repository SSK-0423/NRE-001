#pragma once

#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <string>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"DirectXTex.lib")

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
	HRESULT CopyTexture(ID3D12Device& device,Dx12GraphicsEngine& graphicsEngine);

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
	/// レンダーターゲットからテクスチャ生成
	/// </summary>
	/// <param name="renderTargetBuffer">レンダーターゲットバッファー</param>
	void CreateTextureFromRenderTarget(RenderTargetBuffer& renderTargetBuffer);

	/// <summary>
	/// デプスステンシルバッファーからテクスチャ生成
	/// </summary>
	/// <param name="depthStencilBuffer">デプスステンシルバッファー</param>
	void CreateTextureFromDepthStencil(DepthStencilBuffer& depthStencilBuffer);

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