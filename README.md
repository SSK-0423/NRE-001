# Nameless Renderer
Physically Based Renderer with DirectX12 made for learning and trying rendering techniques

## Gallery
### IBL
![Nameless Renderer 2023_06_11 19_49_32](https://github.com/SSK-0423/NamelessRenderer/assets/83057130/ab051d17-f2f5-4fc2-b804-2f3e5c3736de)
### Cook-Torrance
![Nameless Renderer 2023_06_11 19_50_14](https://github.com/SSK-0423/NamelessRenderer/assets/83057130/d2492d8f-26df-44c1-9c31-bef9b24b560e)
### Trowbridge-Reitz(GGX)
![Nameless Renderer 2023_06_11 19_50_28](https://github.com/SSK-0423/NamelessRenderer/assets/83057130/e59e2127-3821-4e28-a7b9-a42728b49329)
### Movie
https://github.com/SSK-0423/NamelessRenderer/assets/83057130/5726e31b-040d-4b33-bbd6-4566d0bc0d83

## Features
- Deferred Rendering
  1. GBuffer
  2. Directional Lighting
  3. Image Based Lighting
  4. Drawing SkyBox
  5. Blend Lighting result and SkyBox
- Image Based Lighting
- BRDF
  - Diffuse
    - Normalize Lambert
  - Specular
    - Cook-Torrance
    - Trowbridge-Reitz(GGX)
- Reinhard Simple ToneMapping
- Material Parameters
  - Base Color
  - Roughness
  - Metallic
- Light Source
  - Directional Light

## Future Work
- [x] glTF Loader
- [ ] Editor
- [ ] Shadow Mapping (Coming Soon)

## References
- Direct3D 12 ゲームグラフィックス実践ガイド Pocol著 https://gihyo.jp/book/2021/978-4-297-12365-9
- DirectX 12の魔導書 3Dレンダリングの基礎からMMDモデルを踊らせるまで　川野 竜一 著 https://www.shoeisha.co.jp/book/detail/9784798161938
- HLSLシェーダーの魔導書 シェーディングの基礎からレイトレーシングまで　清原 隆行 著 https://www.shoeisha.co.jp/book/detail/9784798164281

## API & Libraries
- DirectX12
- DirectXTex https://github.com/Microsoft/DirectXTex
- DirectXMath https://github.com/microsoft/DirectXMath
- Dear ImGUI https://github.com/ocornut/imgui
- tinygltf https://github.com/syoyo/tinygltf

## Tools Used
- IBL Baker https://github.com/derkreature/IBLBaker

## Assets
- glTF Sample Models https://github.com/KhronosGroup/glTF-Sample-Models
