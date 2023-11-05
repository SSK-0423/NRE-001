# Nameless Renderer 
Physically Based Renderer with DirectX12 made for learning and trying rendering techniques

## Gallery
### IBL
![IBL](https://github.com/SSK-0423/NamelessRenderer/assets/83057130/6f065ffe-38f2-40be-8f9c-cc6fbb813a31)

### Cook-Torrance
![Cook-Torrance](https://github.com/SSK-0423/NamelessRenderer/assets/83057130/16845d9a-9981-4a12-b181-ff69a8a46448)

### Trowbridge-Reitz(GGX)
![GGX](https://github.com/SSK-0423/NamelessRenderer/assets/83057130/b1f66254-2c69-4df3-8fe4-9c95cf7aaba3)

### GLB Model/Shadow Mapping
![Shadow](https://github.com/SSK-0423/NamelessRenderer/assets/83057130/20273e4c-feb3-44e4-829e-338aec2c6335)

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
