# Nameless Renderer
Physically Based Renderer with DirectX12 for Studying Photorealistic Rendering

## Rendering Image
### IBL Only
![ibl rendering image](https://user-images.githubusercontent.com/83057130/200493259-90e801bd-6809-41f4-a060-72980351ae37.PNG)

## Feature
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
- [ ] FBX Loader
- [ ] glTF Loader
- [ ] Disney BRDF

## References
- Direct3D 12 ゲームグラフィックス実践ガイド Pocol著 https://gihyo.jp/book/2021/978-4-297-12365-9
- DirectX 12の魔導書 3Dレンダリングの基礎からMMDモデルを踊らせるまで　川野 竜一 著 https://www.shoeisha.co.jp/book/detail/9784798161938

## API & Library
- DirectX12
- DirectXTex
- DirectXMath
