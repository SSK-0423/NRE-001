#pragma once
#include <string>
#include <DirectXMath.h>

#include "IMaterial.h"
#include "ConstantBuffer.h"
#include "Texture.h"

#include "Dx12GraphicsEngine.h"

namespace NamelessEngine::Graphics
{
	/// <summary>
/// フォンの反射モデル用マテリアルクラス
/// </summary>
	class PhongMaterial : public IMaterial {
	public:
		PhongMaterial();
		~PhongMaterial();
		PhongMaterial(const PhongMaterial& inst);

	private:
		std::wstring _textureName;
		std::wstring _textureFolderPath;

		struct PhongMaterialBuff
		{
			DirectX::XMFLOAT4 ambient;
			DirectX::XMFLOAT4 diffuse;
			DirectX::XMFLOAT4 specular;
			float shininess;
		};
		PhongMaterialBuff _material;

		DX12API::ConstantBuffer _materialCBuffer;
		Utility::RESULT CreateMaterialConstantBuffer(ID3D12Device& device);

		DX12API::Texture _texture;
		Utility::RESULT CreateTexture(Core::Dx12GraphicsEngine& graphicsEngine);

	public:
		/// <summary>
		/// マテリアルを利用可能な状態にする
		/// </summary>
		/// <returns></returns>
		Utility::RESULT Commit();

		void ApplyMaterial(FBXMesh& mesh) override;

		inline void SetAmbient(float r, float g, float b, float factor)
		{
			DirectX::XMStoreFloat4(&_material.ambient, DirectX::XMVectorSet(r, g, b, factor));
		}

		inline void SetDiffuse(float r, float g, float b, float factor)
		{
			DirectX::XMStoreFloat4(&_material.diffuse, DirectX::XMVectorSet(r, g, b, factor));
		}

		inline void SetSpecular(float r, float g, float b, float factor)
		{
			DirectX::XMStoreFloat4(&_material.specular, DirectX::XMVectorSet(r, g, b, factor));
		}

		inline void SetShininess(float s)
		{
			_material.shininess = s;
		}

		inline void SetTextureName(std::wstring name)
		{
			_textureName = name;
		}

		inline void SetTextureFolderPath(std::wstring path)
		{
			_textureFolderPath = path;
		}
	};
}