# Nameless Renderer
Physically Based Renderer with DirectX12 made for learning and trying rendering techniques

## Gallery
### IBL
![Nameless Renderer 2023_06_28 1_50_44](https://github.com/SSK-0423/NamelessRenderer/assets/83057130/a8913e53-eeb4-4a9d-a4f5-78502130e8ef)

### Cook-Torrance
![Nameless Renderer 2023_06_28 1_53_11](https://github.com/SSK-0423/NamelessRenderer/assets/83057130/a8e389c4-476c-4db0-9eb0-730a0b8a418d)

### Trowbridge-Reitz(GGX)
![Nameless Renderer 2023_06_28 1_53_19](https://github.com/SSK-0423/NamelessRenderer/assets/83057130/19251bdc-d5a3-4255-b6fa-c158f0cb022a)

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
