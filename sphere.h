#ifndef SPHERE_H
#define SPHERE_H
#include"rtweekend.h"
#include "hittable.h"


class sphere: public hittable{
public:
	sphere(const point3& centre,double radius):centre(centre),radius(std::fmax(0,radius)){}//constructor

	bool hit(const ray& r,double ray_tmin,double ray_tmax,hit_record& rec) const override{

		vec3 oc=centre-r.origin();
		auto a=r.direction().length_squared();
		auto h=dot(r.direction(),oc);
		auto c=oc.length_squared()-radius*radius;

		auto discriminant =h*h-a*c;
		if(discriminant<0)
			return false;
		auto sqrtd=std::sqrt(discriminant);

		//finding nearest root in acceptable range
		auto root =(h-sqrtd)/a;
		if(root<=ray_tmin||root>=ray_tmax){
			root=(h+sqrtd)/a;
			if(root<=ray_tmin||root>=ray_tmax)
				return false;
		}
		rec.t=root;
		rec.p=r.at(rec.t);
		vec3 outward_normal=(rec.p-centre)/radius;
		rec.set_face_normal(r,outward_normal);
		return true;
	}
	private:
		point3 centre;
		double radius;
};

#endif
