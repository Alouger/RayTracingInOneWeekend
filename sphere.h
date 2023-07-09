#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
// #include "ray.h"

class sphere : public hittable {
    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
    public:
        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const override;
};

// 球体与射线碰撞函数
bool sphere::hit(const ray & r, double t_min, double t_max, hit_record & rec) const {
    vec3 oc = r.origin() - center;
    // 这里的a，b，c是一元二次方程的a*x^2+b*x+c=0
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    // // discriminant > 0的话，方程有两个实根，说明光线击中小球
    auto discriminant = half_b * half_b - a * c;
    // 如果函数无解则直接返回false
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);
    
    // 查找在可接受的t范围内，最近的碰撞点
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    // 碰撞时间
    rec.t = root;
    // 碰撞点
    rec.p = r.at(rec.t);
    // 碰撞法线
    // 目前，我们的碰撞法线是 (rec.p - center) / radius。即朝向球体外部
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    // hit_record中的材质指针会被球体的材质指针所赋值
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif