# Ray Tracer Application

This is a simple, portable ray tracing engine written in C++17. It renders a scene to an image file.

# Structure

    include/  contains header files (.h)
    src/      contains implementation files (.cpp)
    libs/     contains external libraries used by the application
    examples/ contains example files, not used by the application

## Prerequisites
- A Unix-like system (Linux, macOS)
- GNU Make
- g++ (with C++17 support)

## Building
1. Clone or download this repository.
2. In the project root directory, run:
   ```
   make
   ```
   This will produce the executable `rt`.

## Running
Usage:
```sh
./rt [resolution]
```
- `resolution`: optional integer; if provided, the output image will be `resolution`×`resolution` pixels.
  If omitted, defaults to 200×160.

By default, the engine writes a PNG file named `left.png` into an `output/` directory under the project root.
You can override the output directory by setting the RT_OUTPUT environment variable.

## Cleaning
To remove built artifacts, run:
```sh
make clean
```

## Notes
- The engine uses a basic Makefile. Feel free to integrate a different build system (CMake, etc.) as needed.
- The default output format is PNG (via stb_image_write). Other formats (TGA, HDR, JPG) are supported in code and can be selected by modifying the ImgClass argument in your setup code.
