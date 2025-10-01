# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is Homework 3 from the GAMES101 course - a 3D rasterizer implementing various shading techniques including Phong shading, texture mapping, bump mapping, and displacement mapping. The project loads 3D models (OBJ format) and renders them using a custom software rasterizer with MSAA (4x multi-sample anti-aliasing).

## Build System

**Build the project:**
```bash
cd build
cmake ..
make
```

**Run the rasterizer:**
```bash
cd build
./Rasterizer output.png [shader_type]
```

Shader types: `normal`, `phong`, `texture`, `bump`, `displacement`

Example: `./Rasterizer output.png phong`

## Architecture

### Core Components

- **`rasterizer.hpp/cpp`**: Main rasterizer class (`rst::rasterizer`)
  - Manages framebuffer, depth buffer, and MSAA sample buffers
  - Implements triangle rasterization with 4x MSAA
  - MVP transformation pipeline: vertex shader → MVP → clipping → perspective division → viewport → rasterization → fragment shader
  - Key methods: `draw()`, `rasterize_triangle()`, `set_model/view/projection()`

- **`main.cpp`**: Entry point and shader implementations
  - View, model, and projection matrix generation functions
  - Fragment shaders: `normal_fragment_shader`, `phong_fragment_shader`, `texture_fragment_shader`, `bump_fragment_shader`, `displacement_fragment_shader`
  - Blinn-Phong lighting model implementation (ambient + diffuse + specular)
  - Loads OBJ models using `OBJ_Loader.h`

- **`Triangle.hpp/cpp`**: Triangle primitive
  - Stores vertex positions (Vector4f), colors, normals (Vector3f), and texture coordinates (Vector2f)
  - Used for per-vertex attribute storage

- **`Shader.hpp`**: Shader payload structures
  - `fragment_shader_payload`: Contains color, normal, tex_coords, view_pos, texture pointer
  - `vertex_shader_payload`: Contains position

- **`Texture.hpp/cpp`**: Texture sampling
  - Uses OpenCV to load images
  - `getColor(u, v)` for texture coordinate sampling

### Rendering Pipeline

1. Load 3D model (OBJ) with vertices, normals, and texture coordinates
2. Set up view, model, projection matrices
3. Transform vertices through MVP pipeline
4. Rasterize triangles with 4x MSAA (4 samples per pixel at offsets 0.25, 0.75)
5. For each sample: depth test, barycentric interpolation (color, normal, texcoords, view position with perspective-correct interpolation)
6. Run fragment shader to compute final color (lighting calculations)
7. Average 4 samples for final pixel color

### Key Technical Details

- **MSAA Implementation**: 4 samples per pixel stored in `sample_color_buf` and `sample_depth_buf` (size = width × height × 4)
- **Barycentric Interpolation**: Perspective-correct interpolation using `w_reciprocal` for depth, colors, normals, and texture coordinates
- **Coordinate Systems**: View space for lighting calculations, screen space for rasterization
- **Depth Testing**: Per-sample depth testing with infinite initial depth

## Dependencies

- **Eigen3**: Linear algebra (vectors, matrices)
- **OpenCV**: Image I/O and display (`cv::imread`, `cv::imshow`, `cv::imwrite`)
- C++17

## Code Patterns

- Shaders are implemented as `std::function<Eigen::Vector3f(payload)>` callbacks
- Color values are in [0, 255] range
- Fragment shaders receive interpolated attributes and return RGB color (multiplied by 255)
- Model data stored in `../models/spot/` directory
