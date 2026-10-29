# Endless Procedural Grassland in OpenGL
Repository for the Computer Graphics course project - performant Ghost of Tsushima style grass with different LOD techniques in a procedurally generated environment using OpenGL.

## Building
CMake is used as the build system and vcpkg for package management.

To make a Windows build, run the following commands 
```sh
cmake --preset build-config-win
cmake --build --preset win-build
``` 

To make a Unix (Linux) build, run the following commands 
```sh
cmake --preset build-config-unix
cmake --build --preset unix-build
``` 

The binary will be at `build/Endless_Grassland`