# Projective Rendering TP

## About this Project

This project focuses on the fundamentals of projective rendering using a custom rasterization pipeline implemented in C++. The goal is to simulate the inner workings of the traditional OpenGL pipeline, from geometry transformation to pixel-level lighting.

It manually reproduces core stages of real-time rendering, providing an educational framework to better understand how 3D objects are transformed, lit, and rasterized into 2D images.

The project includes implementations of:

- Line and triangle rasterization
- Depth buffering (Z-buffer)
- Per-vertex and per-fragment Phong lighting
- Texture mapping
- Camera transformations and perspective projection

By reproducing these techniques entirely on the CPU, it helps demystify what happens behind the scenes in modern GPU-based rendering.

---

## Features

- **Line Drawing with Color Interpolation**  
  Implemented using Bresenham's algorithm across all octants with smooth interpolation between vertex colors.

- **Triangle Rasterization (Scanline Fill)**  
  Filling triangles by interpolating pixel attributes (color, depth, texture coordinates) across scanlines.

- **Z-buffering**  
  Manages pixel visibility by comparing depth values and updating the framebuffer accordingly.

- **Phong Illumination Model**  
  Includes ambient, diffuse, and specular lighting with light position in world space. Illumination is computed at the vertex and optionally at the fragment level.

- **Vertex and Fragment Shading Simulation**  
  Vertex shader handles model/view/projection transforms and lighting at vertex level. Fragment shader interpolates results across the surface for improved realism.

- **Perspective Projection**  
  Transformation of 3D geometry to 2D screen space using custom model-view-projection matrices.

- **Texture Mapping**  
  Triangles can be textured using UV coordinates with proper interpolation in projective space.

- **Mesh Support**  
  Includes example meshes like a dinosaur, demonstrating full shading and texturing.

- **Optional Fragment Shader Pipeline**  
  Improved lighting realism by performing Phong calculations at pixel level.

---

## Compilation 

### From the Makefile 

```sh
cd projet
make
./pgm
```

From the CMakeLists.txt

```sh 
mkdir build
cd build
cmake ..
make
cd ../projet
../build/pgm
```

or

```sh 
cd projet
mkdir ../build
cmake .. -B ../build
make -C ../build
../build/pgm
```

### From an IDE (VSCode, QtCreator, etc).


The execution directory must be inside projet/

That is, the data/ directory must be accessible.
