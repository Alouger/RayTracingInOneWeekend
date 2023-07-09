#ifndef HITTABLE_H
#define HITTABLE_H

// #include "ray.h"
// #include "vec3.h"
#include "rtweekend.h"

class material;

struct hit_record {
    vec3 p; // 碰撞点位置
    vec3 normal; // 碰撞点表面法线
    // 我们在hit_record中存储了材质的指针。
    shared_ptr<material> mat_ptr;
    double t; // 碰撞在射线上的时间t参数
    bool front_face;

    inline void set_face_normal(const ray & r, const vec3 & outward_normal) {
        // 们可以简单的将光线与法线做点乘来判断。如果法相与光线方向相同, 那就是从内部击中内表面
        // 如果相反则是从外部击中外表面
        // 如果我们永远让法线与入射方向相反, 我们就不用去用点乘来判断射入面是内侧还是外侧了
        // 但相对的, 我们需要用一个变量储存射入面的信息:
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// 与射线碰撞的物体基类
class hittable {
    public:
        // 计算和射线碰撞的函数，如果碰撞会返回碰撞点记录数据，本函数由子类实现
        // 纯虚函数是在基类中声明的虚函数，它在基类中没有定义，但要求任何派生类都要定义自己的实现方法。在基类中实现纯虚函数的方法是在函数原型后加 =0:
        virtual bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const = 0;
};

#endif