#pragma once

// Core
#include "Core/AppWindow.h"
#include "Core/Dx12GraphicsEngine.h"

// DX12API
#include "DX12API/Texture.h"
#include "DX12API/ConstantBuffer.h"
#include "DX12API/DescriptorHeapCBV_SRV_UAV.h"
#include "DX12API/ShaderResourceViewDesc.h"

#include "DX12API/DepthStencilBuffer.h"
#include "DX12API/DescriptorHeapDSV.h"

#include "DX12API/RenderTarget.h"
#include "DX12API/RenderTargetBuffer.h"
#include "DX12API/DescriptorHeapRTV.h"

#include "DX12API/VertexBuffer.h"
#include "DX12API/IndexBuffer.h"

#include "DX12API/RenderingContext.h"

#include "DX12API/Shader.h"

#include "DX12API/RootSignature.h"
#include "DX12API/GraphicsPipelineState.h"

// Graphics
#include "Graphics/Geometry.h"
#include "Graphics/GeometryImpl.h"
#include "Graphics/CubeMesh.h"
#include "Graphics/SphereMesh.h"
#include "Graphics/SkySphere.h"
#include "Graphics/SkyBox.h"
#include "Graphics/Polygon.h"
#include "Graphics/Sprite.h"

#include "Graphics/FBXLoader.h"
#include "Graphics/FBXMesh.h"

#include "Graphics/IMaterial.h"
#include "Graphics/PBRMaterial.h"
#include "Graphics/PhongMaterial.h"

#include "Graphics/IRenderer.h"
#include "Graphics/PBRRenderer.h"

// Component
#include "Component/Actor.h"
#include "Component/IComponent.h"
#include "Component/Transform.h"
#include "Component/Mesh.h"

// Utility
#include "Utility/Singleton.h"
#include "Utility/EngineUtility.h"

// Scene
#include "Scene/Scene.h"
#include "Scene/Camera.h"
