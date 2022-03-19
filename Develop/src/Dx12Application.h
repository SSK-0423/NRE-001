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

#include <DirectXMath.h>
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

	DescriptorHeapCBV_SRV_UAV _polygonHeap;
	Texture _texture;
	ConstantBuffer _constantBuffer;

	struct PolygonConstantBuffer {
		DirectX::XMMATRIX _worldViewProj;
		DirectX::XMMATRIX _matrix;
	};

	PolygonConstantBuffer _polygonConstantBuffer;

	float _angle = 0.f;

	// マルチパスレンダリング
	DescriptorHeapCBV_SRV_UAV _offscreenHeap;	        // オフスクリーンテクスチャ用ヒープ
	Texture _offscreenTexture;	                        // オフスクリーンテクスチャ
	
	DescriptorHeapRTV _offscreenRTVHeap;	            // オフスクリーンレンダー用ヒープ
	RenderTargetBuffer _offscreenRender;	            // オフスクリーンレンダー
	
	RootSignature _offscreenRootSignature;	            // オフスクリーン用ルートシグネチャ(今は未使用)
	GraphicsPipelineState _offscreenGraphicsPipeline;	// オフスクリーン用パイプライン

	VertexBuffer _offscreenPolygonVB;					// オフスクリーン用頂点バッファー
	IndexBuffer _offscreenPolygonIB;					// オフスクリーン用インデックスバッファー
	MyFrameWork::Polygon _offscreenPolygon;				// オフスクリーンポリゴン
														// これに前パスのレンダリング結果をテクスチャマップする
	Shader _offscreenVS;	                            // オフスクリーンポリゴン用頂点シェーダー
	Shader _offscreenPS;	                            // オフスクリーンポリゴン用ピクセルシェーダー

	// テクスチャマッピング実装用
private:
	MYRESULT InitTexture();
	MYRESULT InitConstantBuffer();
	void TextureMappingDraw();		// テクスチャマッピング・定数バッファー確認用描画関数

	// マルチパスレンダリング実装用
private:
	MYRESULT InitOffscreenRender();
	void MultiPassRenderingDraw();
};

/// メモ
/// DirectXの初期化にウィンドウハンドル必要(HWND)
/// →シングルトン化してウィンドウサイズ取得できるようにしたい
/// →色んなクラスからAppicationクラスを呼び出す羽目になる
/// →アプリケーションクラスはアプリケーション固有の処理を記述するためのクラス
///	→システム側のクラスがアプリケーションクラスと関係を持つことは避ける
/// →main.cppでのみ呼び出すような設計にする→シングルトン化する必要なし