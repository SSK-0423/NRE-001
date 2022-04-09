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
	VertexBuffer _vertexBuffer;			// 頂点バッファー
	IndexBuffer _indexBuffer;			// インデックスバッファー
	Shader _vertexShader;				// 頂点シェーダー
	Shader _pixelShader;				// ピクセルシェーダー
	RootSignature _rootSignature;		// ルートシグネチャ
	MyFrameWork::Polygon _triangle;		// 三角形ポリゴン
	MyFrameWork::Polygon _square;		// 四角形ポリゴン
	CD3DX12_VIEWPORT _viewport;			// ビューポート
	CD3DX12_RECT _scissorRect;			// シザー矩形

	// テクスチャマッピング実装用
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
	/// テクスチャ初期化
	/// </summary>
	/// <returns></returns>
	MYRESULT InitTexture();
	/// <summary>
	/// コンスタントバッファ―初期化
	/// </summary>
	/// <returns></returns>
	MYRESULT InitConstantBuffer();

	/// <summary>
	/// テクスチャマッピング・コンスタントバッファ―確認用
	/// </summary>
	void TextureMappingDraw();

	// マルチパスレンダリング実装用
private:
	OffScreenRender _firstPassRender;
	OffScreenRender _secondPassRender;

	/// <summary>
	/// オフスクリーンレンダー初期化
	/// </summary>
	/// <returns></returns>
	MYRESULT InitOffscreenRender();
	/// <summary>
	/// マルチパスレンダリング確認用
	/// </summary>
	void MultiPassRenderingDraw();
	
	// マルチレンダーターゲット
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
	/// マルチレンダーターゲット初期化
	/// </summary>
	/// <returns></returns>
	MYRESULT InitMultiRenderTarget();

	/// <summary>
	/// MRTポリゴン生成
	/// </summary>
	/// <returns></returns>
	MYRESULT CreateMRTPolygon();

	/// <summary>
	/// MRTレンダーターゲット生成
	/// </summary>
	/// <returns></returns>
	MYRESULT CreateMRTRenders();

	/// <summary>
	/// MRT用のディスクリプタヒープ生成
	/// </summary>
	/// <returns></returns>
	MYRESULT CreateMRTDescriptorHeap();

	/// <summary>
	/// マルチレンダーターゲット確認用
	/// </summary>
	void MultiRenderTargetDraw();
};

/// メモ
/// DirectXの初期化にウィンドウハンドル必要(HWND)
/// →シングルトン化してウィンドウサイズ取得できるようにしたい
/// →色んなクラスからAppicationクラスを呼び出す羽目になる
/// →アプリケーションクラスはアプリケーション固有の処理を記述するためのクラス
///	→システム側のクラスがアプリケーションクラスと関係を持つことは避ける
/// →main.cppでのみ呼び出すような設計にする→シングルトン化する必要なし