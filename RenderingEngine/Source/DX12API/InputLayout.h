#pragma once
#include <d3d12.h>
#include <vector>

#pragma comment(lib,"d3d12.lib")

namespace NamelessEngine::DX12API {
	struct InputLayout {
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs;

		/// <summary>
		/// D3D12_INPUT_ELEMENT_DESC配列の先頭ポインタ取得
		/// </summary>
		/// <returns>配列の要素が0ならnullptrを返す</returns>
		D3D12_INPUT_ELEMENT_DESC* GetData() {
			if (inputElementDescs.size() == 0) {
				nullptr;
			}
			return inputElementDescs.data();
		}

		UINT GetSize() {
			return inputElementDescs.size();
		}

		void Append(
			const char* semanticName, UINT semanticIndex, DXGI_FORMAT format, UINT inputSlot,
			UINT alignedByteOffset, D3D12_INPUT_CLASSIFICATION inputSlotClass, UINT instanceDataStepRate) {
			D3D12_INPUT_ELEMENT_DESC desc = {
				semanticName, semanticIndex, format,inputSlot,
				alignedByteOffset,inputSlotClass, instanceDataStepRate };
			inputElementDescs.push_back(desc);
		}

		static InputLayout DefaultLayout() {
			InputLayout layout;
			layout.inputElementDescs.resize(4);
			layout.inputElementDescs[0] = {
				"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
			layout.inputElementDescs[1] = {
				"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
			layout.inputElementDescs[2] = {
				"TANGENT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
			layout.inputElementDescs[3] = {
				"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

			return layout;
		}
	};
}