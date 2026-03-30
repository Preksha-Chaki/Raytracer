# Raytracer

In this Github repository, I will be documenting my journey making my own raytracer following [_Ray Tracing in One Weekend_] along whith what challenges i faced and how i solved them.

## 0. Creating a cpp file in directory and running it

First we create a main.cpp file.

then we create CMakeLists.txt which is a configuration file that tells CMake how to build your project.

CMakeLists.txt
```
cmake_minimum_required(VERSION 3.10)
project(MyProject)

add_executable(main main.cpp)
```
without CMakeLists.txt we would have to compile manually everytime 
```
g++ main.cpp -o main
```
this can get quite messy quite quickly as multiple files are linked together in future.

To build the program we use,
```
cmake -B build
cmake --build build
```
to run the build,
```
./build/main > image.ppm
```
>redirects the output and saves the output in the form of file image.ppm

## 1. Output an Image

The program generates an image in the `.ppm` format by writing pixel data to standard output.

to view the resultant image we use an image viewer
```
eog image.ppm
```
We now get to view our first image,

<img width="515" height="343" alt="image" src="https://github.com/user-attachments/assets/b3d1dfb9-96c8-4e66-bc63-e6c18003932e" />

## 2. vec3 Class

The term vec3 in C++ typically refers to a user-defined class or struct for representing a three-component vector, commonly used in graphics programming and physics simulations to store positions, directions, velocities, or colors.

Next we will be creating a vec3.h header file

References used here:
- https://www.reddit.com/r/cpp_questions/comments/j7wfef/what_is_the_need_for_ifndef_define_endif_inside/
- https://youtu.be/tOQZlD-0Scc?si=Vi7uAjPyavt1-1Lc
- https://www.geeksforgeeks.org/cpp/pointers-and-references-in-c/

  ### cross product
  if,u = (ux, uy, uz)
    v = (vx, vy, vz)

  then,
  u × v =
  ( uy*vz - uz*vy,
  uz*vx - ux*vz,
  ux*vy - uy*vx )

  ### Color Utility Functions
  
  In the file color.h we make another alias for vec3 color.
  pix_color is the object of color. Then we extract components r,g and b. These are in the range[0.0,1.0] but we need to convert these as image formats(like ppm) expect values in range [0,255]. For conversion we use the formula,
      value_in_255_range = 255.999 * value_in_0_1_range
  We use 255.999 because due to floating point rounding sometimes, int(255 * 0.999999) gives 254.



  








