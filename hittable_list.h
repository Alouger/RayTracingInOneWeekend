#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

// 我们在代码中使用智能指针的目的是为了能让多个几何图元共享一个实例(举个栗子, 一堆不同球体使用同一个纹理材质),
// 并且这样内存管理比起普通的指针更加的简单方便。
using std::shared_ptr;
using std::make_shared;

// 可碰撞对象列表
class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(const ray & r, double t_min, double t_max, hit_record & rec) const override;
};

// 本列表与射线碰撞函数
bool hittable_list::hit(const ray & r, double t_min, double t_max, hit_record & rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    // 对于列表的每个对象
    for (const auto & object : objects) {
        // 如果由一个对象与射线相交了，则把相交点的时间t记录为下次相交的最大t
        // 以便仅仅求解最近的相交
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif