#include "Mesh.h"
#include "IMeshLoader.h"
Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

MYRESULT Mesh::LoadMesh(const char* meshPath, ID3D12Device& device)
{
    // ローダー用意
    IMeshLoader* loader = new FBXLoader();

    // FBX読み込み

    if (!loader->Load(meshPath))
    {
        return MYRESULT::FAILED;
    }
    
    // 頂点バッファ―生成
    MYRESULT result = CreateVertexBuffers(device);
    if (result == MYRESULT::FAILED) { return result; }

    // インデックスバッファー生成
    result = CreateIndexBuffers(device);
    if (result == MYRESULT::FAILED) { return result; }

    return MYRESULT::SUCCESS;
}

void Mesh::Draw(RenderingContext& renderContext)
{
}

MYRESULT Mesh::CreateVertexBuffers(ID3D12Device& device)
{
    return MYRESULT::SUCCESS;
}

MYRESULT Mesh::CreateIndexBuffers(ID3D12Device& device)
{
    return MYRESULT::SUCCESS;
}

//
// 実装案
// 1. Mesh側でバッファー生成
// メリット
// ・FBXLoaderがVertexBuffer/IndexBufferクラスに依存しなくなるので、独立性が増して色々な実装に対応できる
// デメリット
// ・Mesh側のFBXLoaderへの依存が強くなる
// ・他形式のファイルを読み込む際に対応できない
// 2. FBXLoader側でバッファー生成
// メリット
// ・メッシュ側の処理が簡素になる
// ・他形式を読み込むローダークラスへの切り替えが容易
// デメリット
// ・VertexBuffer/IndexBufferクラスに依存する形になる
// ・独立性が損なわれ、部品としての使いまわしがしづらくなる
//