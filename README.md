# Nameless Renderer
Physically Based Renderer with DirectX12 made for learning and trying rendering techniques

# Rendering Image
## IBL
![ibl rendering image](https://user-images.githubusercontent.com/83057130/200493259-90e801bd-6809-41f4-a060-72980351ae37.PNG)
## Cook-Torrance
![cook-torrance rendering image](https://user-images.githubusercontent.com/83057130/200494726-da110b85-6b7c-4043-90a4-14613881d5d5.PNG)
## Trowbridge-Reitz(GGX)
![ggx rendering image](https://user-images.githubusercontent.com/83057130/200494759-bb96bf09-fb69-43c5-9201-f47ba7526c27.PNG)
# Feature
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

# Future Work
- [ ] FBX Loader
- [ ] glTF Loader
- [ ] Specular BRDF like Unreal Engine 4
- [ ] Shadow Mapping

# References
- Direct3D 12 ゲームグラフィックス実践ガイド Pocol著 https://gihyo.jp/book/2021/978-4-297-12365-9
- DirectX 12の魔導書 3Dレンダリングの基礎からMMDモデルを踊らせるまで　川野 竜一 著 https://www.shoeisha.co.jp/book/detail/9784798161938

# API & Libraries
- DirectX12
- DirectXTex https://github.com/Microsoft/DirectXTex
- DirectXMath https://github.com/microsoft/DirectXMath
- Dear ImGUI https://github.com/ocornut/imgui

# Tools Used
- IBL Baker https://github.com/derkreature/IBLBaker
