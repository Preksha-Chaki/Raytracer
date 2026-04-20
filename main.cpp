#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

color ray_color(const ray& r){
	vec3 unit_direction=unit_vector(r.direction());//vector of magnitude 1
	auto a=0.5*(unit_direction.y()+1.0);
	return (1-a)*color(1.0,1.0,1.0)+a*color(0.5,0.7,1.0);//blue
}

int main() {

	// Image

	auto aspect_ratio=16.0/9.0;
	int image_width=400;

	//calc img height to be atleast 1
	int image_height=int(image_width/aspect_ratio);
	image_height=(image_height<1)?1:image_height;

	//camera
	auto focal_length=1.0;
	auto viewport_height=2.0;
	auto viewport_width=viewport_height*(double(image_width)/image_height);
	auto camera_centre=point3(0,0,0);

	//calc vectors across horizontal and down vertical viewport edges
	auto viewport_u=vec3(viewport_width,0,0);
	auto viewport_v=vec3(0,-viewport_height,0);//cuz we need opposite of the y axis

	//horizontal and vertical delta vectors pixel 2 pixel
	auto pixel_delta_u=viewport_u/image_width;
	auto pixel_delta_v=viewport_v/image_height;

	//calc locn of upper left pixel
	auto viewport_upper_left=camera_centre-vec3(0,0,focal_length)-viewport_u/2-viewport_v/2;
	auto pixel00_loc=viewport_upper_left+0.5*(pixel_delta_u+pixel_delta_v);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
		auto pixel_centre=pixel00_loc+(i*pixel_delta_u)+(j*pixel_delta_v);
		auto ray_direction=pixel_centre-camera_centre;
		ray r(camera_centre,ray_direction);
		color pixel_color=ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}
