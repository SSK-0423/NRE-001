#pragma once

#include <Windows.h>
#include <memory>

#include "EngineUtility.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "Polygon.h"

#include <DirectXTex.h>

#pragma comment(lib,"DirectXTex.lib")

class Dx12GraphicsEngine;
class AppWindow;

/// <summary>
/// レンダリング開発中のテスト用クラス
/// </summary>
class Dx12Application
{
public:
	Dx12Application();
	~Dx12Application();

	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <returns>成功：MYRESULT::SUCCESS 失敗：MYRESULT::FAILED</returns>
	MYRESULT Init();

	/// <summary>
	/// メインループ
	/// </summary>
	void Run();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

private:
	Dx12GraphicsEngine& _graphicsEngine;			    // 描画の基礎部分を担当するエンジン
	std::shared_ptr<AppWindow> _window = nullptr;		// アプリケーションのウィンドウ
// 仮
	
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

// 開発用
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

	// テクスチャマッピング実装用
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _textureBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _textureHeap = nullptr;

	const DirectX::Image* _image = nullptr;
	DirectX::TexMetadata _metaData;
	DirectX::ScratchImage _scratchImage;

	MYRESULT InitTexture();
	HRESULT LoadTextureFile(const wchar_t* path);
	HRESULT CreateTextureResource(ID3D12Device& device);

};

/// メモ
/// DirectXの初期化にウィンドウハンドル必要(HWND)
/// →シングルトン化してウィンドウサイズ取得できるようにしたい
/// →色んなクラスからAppicationクラスを呼び出す羽目になる
/// →アプリケーションクラスはアプリケーション固有の処理を記述するためのクラス
///	→システム側のクラスがアプリケーションクラスと関係を持つことは避ける
/// →main.cppでのみ呼び出すような設計にする→シングルトン化する必要なし