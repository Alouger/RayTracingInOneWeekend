#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
// #include "hittable.h"
#include "ray.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(
            const ray & r_in, const hit_record & rec, color & attenuation, ray & scattered
        ) const = 0;
};

// 对于我们之前写过的Lambertian(漫反射)材质来说,
// 这里有两种理解方法, 要么是光线永远发生散射, 每次散射衰减至R, 要么是光线并不衰减, 转而物体吸收(1-R)的光线。
// 你也可以当成是这两种的结合。我
// 们这里用的是albedo成员变量作为衰减值，让光线总是散射出去，于是我们可以写出Lambertian的材质类:
class lambertian : public material {
    public:
        vec3 albedo; // 散射衰减值
    
    public:
        lambertian(const vec3 & a) : albedo(a) {}

        virtual bool scatter(
            const ray & r_in, const hit_record & rec, vec3 & attenuation, ray & scattered
        ) const {
            vec3 scatter_direction = rec.normal + random_unit_vector();
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo; // 把散射衰减值传出去
            return true;
        }
};

// 金属材料计算反射
class metal : public material {
    public:
        color albedo;
        // 反射方向随机偏移的球体越大，则金属越粗糙。
        // 以我们这里引入一个变量来表示模糊的程度(fuzziness)(所以当fuzz=0时不会产生模糊)。
        // 如果fuzz越大, 也就是随机球的半径很大, 光线可能会散射到物体内部去。
        // 这时候我们可以认为物体吸收了光线。
        double fuzz;

    public:
        metal(const color & a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray & r_in, const hit_record & rec, color & attenuation, ray & scattered
        ) const override {
            // 向量反射
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            // 如果fuzz, 也就是随机球的半径很大, 光线可能会散射到物体内部去。这时候我们可以认为物体吸收了光线。
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            // 只要反射的方向和法线是同一个半球，则表示反射成功
            return (dot(scattered.direction(), rec.normal) > 0);
        }
};

#endif