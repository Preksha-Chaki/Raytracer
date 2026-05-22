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
> redirects the output and saves the output in the form of file image.ppm

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
  
In the file color.h we make another alias for vec3 color.pix_color is the object of color.

Then we extract components r,g and b. These are in the range[0.0,1.0] but we need to convert these as image formats(like ppm) expect values in range [0,255]. For conversion we use the formula,
  
      value_in_255_range = 255.999 * value_in_0_1_range
  
  We use 255.999 because due to floating point rounding sometimes, int(255 * 0.999999) gives 254.

## 3.Rays,Camera,Background

  ray class is formed in which we implement the function p(t)=a+tb which gives a point along a 3D line.
  Then we choose a image aspect ratio where it is not 1:1.
  Additionally we set up a virtual viewport through which we pass our scene rays. It's a virtual rectangle in the 3D world that contains the grid of image pixel location.

we set viewport_width=viewport_height*(image width/image height)
we do not set it wrt to aspect ratio as it can vary due to 2 factors-
- image_height rounded down to nearest integer, increases ratio
- we don't allow image_height<1

For viewport prop to exactly match image proportion, use calculated image aspect ratio for final viewport width.

Focal Length = where all scene rays will origin

Focal length = distance between viewport and camera centre

We scan the image from upper left row by row, top to bottom

<img width="1001" height="650" alt="image" src="https://github.com/user-attachments/assets/eec44768-5211-49f1-9039-5923973bc689" />

To get point Q we use,

```
auto viewport_upper_left=camera_centre-vec3(0,0,focal_length)-viewport_u/2-viewport_v/2;
```
the calculations can be represented visually as follows

<img width="512" height="447" alt="image" src="https://github.com/user-attachments/assets/eb889ac7-edbe-430d-9754-aacd5e736292" />

<img width="737" height="511" alt="image" src="https://github.com/user-attachments/assets/6c8ab826-f1b9-4f5a-9fd1-6e1c8a37a173" />

here,

viewport_u=horizontal width of viewport and,

viewport_v=vertical height of viewport

Now we are at the upper left corner of the upper left most pixel but ray tracing works best when rays go through the centre of the pixel not the edge.
For this we use,

```
auto pixel00_loc=viewport_upper_left+0.5*(pixel_delta_u+pixel_delta_v);
```

Here,

pixel_delta_u moves 1 pixel to the right

pixel_delta_v moves 1 pixel down

viewport_upper_left+0.5*(pixel_delta_u+pixel_delta_v) together moves us to the centre of the current pixel

Then we use linear interpolation/linear blend to create a gradient.

for 0.0<=a<=1.0,

if a=1.0->blue        a=0.0-> white

if in between value then it results in a blend

This is refered to as a 'Lerp'

we use the formula,
```
blendedValue=(1-a).startValue+a.endValue
```

Implementing this we get the following image


<img width="398" height="222" alt="image" src="https://github.com/user-attachments/assets/0333be58-fa21-462f-b2ab-ec375df73187" />


## 4.Adding a Sphere

Solving the equation for a circle with C as the centre and P as any point on the circle. Then we get roots for the equation.

<img width="708" height="519" alt="image" src="https://github.com/user-attachments/assets/697e7254-d077-4e76-8b93-9bae8933a940" />

Applying this in code we get the following image

<img width="395" height="222" alt="image" src="https://github.com/user-attachments/assets/3298f2dd-3dbf-4b55-8fd7-9b7336296c51" />

## 5.Surface Normals 

Normal vectors are used for shading. They are perpendicular to the surface at the point of intersection. All normal vectors are of unit length.Implementing surface normals we get,

<img width="378" height="209" alt="image" src="https://github.com/user-attachments/assets/c7bb1c4d-c3bb-47db-96c9-18a727101363" />

```
return (-b-std::sqrt(discriminant))/(2*a);
```
The above return statement is used when discriminant is >=0. It chooses the -ve root instead of +ve root because quadratic equation gives 2 intersection points, one closer(entry) and one farther(exit).

Then we simplify the ray-sphere intersection code by taking b=-2h

On doing that we get,

<img width="397" height="222" alt="image" src="https://github.com/user-attachments/assets/1ce86e27-5156-4e9e-9717-b1e284a254ab" />

### Hittable.h

The following class gives information about ray-object intersection,
```
class hit_record{
public:
    point3 p;
    vec3 normal;
    double t;
}
```

p -> point of Intersection

normal -> Surface normal at point p

t -> distance along ray

we use,
```
virtual bool hit(const ray& r,double ray_tmin,double ray_tmax,hit_record& rec)const=0;
```
This is a Pure Virtual Function, which means
- This class can't be instantiated
- any derived class from class hittable must implement hit

## 6.Front faces vs Back faces

### Case 1: Normal always points outwards

This is what we are currently using.

Normal = from centre to surface, always points outside sphere

Ray hits from outside
- Normal points outwards
- Ray comes from outside

This causes ray to to be perpendicular to ray direction,i.e.,points opposite to to ray.

Ray hits from inside
- Ray starts from inside sphere
- normal still still points outwards when it should point in opposite direction

### Case 2:Normals always oppose rays

Instead of always outwards, Normal always against ray direction. Now we use this case instead of case 1 for our raytracer.

For this, dot(ray_direction,outward_vector)

-ve -> ray and normal opposite, keep both same

+ve -> ray and normal in same direction, flip normal

Hittable.h -> class that stores a list of hittables

rtweekend.h -> common header file

Implementing these header files and making a hittable ground we get,

<img width="396" height="223" alt="image" src="https://github.com/user-attachments/assets/15421734-1354-40bb-8781-7c6026316f66" />

  
## 7.Antialiasing


Aliasing -> harsh “stair step” nature of edges in rendered images

To solve this we sample the square region centered at the pixel that extends halfway to each of the four neighboring pixels

Then we update write_color function in color.h to account for number of samples we use,i.e, find avg across all samples we take.

To ensure that the color components of the final result remain within the proper [0,1] bounds we add clamp function to interval.h.

Update in camera.h we update get_ray(i,j) function to generate different samples for each pixel.

Main is updated to include samples_per_pixel parameter.

Implementing these we get the following image,

<img width="392" height="221" alt="image" src="https://github.com/user-attachments/assets/6d5848bb-f287-4146-ad1a-01f39d9782f8" />

As we can see in this image the sphere has smoother edges compared to previous images.

## 8.Difffuse/Matte Materials

These materials don't emit their own light but only take on color of their surrounds and modulate that with their own colour. 

The ray also may be absorbed, the darker the surface the more chance ray is absorbed.

# Case1: a surface that randomly bounces a ray equally in all directions

Here a ray that hits the surface has equal probablity of bouncing off in any direction. For this we add random utility function to vec3.h.

We need to get results of random vectors such thaey are on surface of hemisphere.

For this we use, a rejection method.

- Generate a random vector inside the unit sphere
- Normalize this vector to extend it to the sphere surface
- Invert the normalized vector if it falls onto the wrong hemisphere

First we pick a random point in the cube enclosing the unit sphere(radius 1). If this point lies outside the unit sphere, then generate a new one until we find one that lies inside or on the unit sphere.

Since floating-point numbers have finite precision, a very small value can underflow to zero when squared. We'll reject points that lie inside this “black hole” around the center.

Now we need to determine if our random vector is on the correct hemisphere by compairing it against surface normal. If dot product is positive then it is in correct hemisphere, if negative then we need to invert the vector.

If ray bounces off and keeps 100% of it's colour-> white, 0% -> black

We set ray_color to return rays with 50% of their color resulting in a grey sphere

<img width="398" height="225" alt="image" src="https://github.com/user-attachments/assets/edc02f18-fd0c-4add-a379-1e54475611fa" />





