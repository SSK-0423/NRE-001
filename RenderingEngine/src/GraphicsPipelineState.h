#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

class GraphicsPipelineState {
public:
	GraphicsPipelineState() = default;
	~GraphicsPipelineState() = default;
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState = nullptr;
public:
};