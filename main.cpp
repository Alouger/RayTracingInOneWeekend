#include <iostream>
#include <fstream>
#include <string>
#include "color.h"
#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "ray.h"

// center表示球心
// double hit_sphere(const point3 & center, double radius, const ray & r) {
//     vec3 oc = r.origin() - center;
//     // 这里的a，b，c是一元二次方程的a*x^2+b*x+c=0
//     auto a = dot(r.direction(), r.direction());
//     auto b = 2.0 * dot(oc, r.direction());
//     auto c = dot(oc, oc) - radius * radius;
//     // discriminant > 0的话，方程有两个实根，说明光线击中小球
//     auto discriminant = b*b - 4*a*c;
//     if (discriminant < 0) {
//         return -1.0;
//     } else {
//         // 返回交点的t，这个t是光线a+t*b的t
//         // 在有两个交点的情况下, 我们选取最近的交点smallest(t)
//         return (-b - sqrt(discriminant)) / (2.0 * a);
//     }
// }

// 简化后的射线与球体求交
// 设b=2h
// double hit_sphere(const point3 & center, double radius, const ray & r) {
//     vec3 oc = r.origin() - center;
//     // 这里的a，b，c是一元二次方程的a*x^2+b*x+c=0
//     auto a = r.direction().length_squared();
//     auto half_b = dot(oc, r.direction());
//     auto c = oc.length_squared() - radius * radius;
//     // discriminant > 0的话，方程有两个实根，说明光线击中小球
//     auto discriminant = half_b*half_b - a*c;
//     if (discriminant < 0) {
//         return -1.0;
//     } else {
//         // 返回交点的t，这个t是光线a+t*b的t
//         // 在有两个交点的情况下, 我们选取最近的交点smallest(t)
//         return (-half_b - sqrt(discriminant)) / a;
//     }
// }

// 第6章前的函数ray_color
// color ray_color(const ray & r, const hittable & world) {
//     // 球心是(0, 0, -1)
//     auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
//     if (t > 0.0) {
//         // at函数返回光线向量，减去的是球心(0, 0, -1)，得到球的法线向量N
//         vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
//         // 直接使用法线值作为颜色输出
//         return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
//     }
//     // if (hit_sphere(point3(0, 0, -1), 0.5, r))
//     //     return color(1, 0, 0); // 使用红色来表示视线击中这个小球
//     vec3 unit_direction = unit_vector(r.direction());
//     // ray_color(ray)函数根据y值将蓝白做了个线性插值的混合, 我们这里把射线做了个单位化, 以保证y的取值范围(-1.0<y<1.0)。
//     // 因为我们使用y轴做渐变, 所以你可以看到这个蓝白渐变也是竖直的。
//     // 我们接下来使用了一个标准的小技巧将y的范围从-1.0 ≤ y ≤ 1.0映射到了0 ≤ y ≤ 1.0。
//     // 这样t=1.0时就是蓝色, 而t=0.0时就是白色。
//     t = 0.5 * (unit_direction.y() + 1.0);
//     // 在蓝白之间我想要一个混合效果(blend)。现在我们采用的是线性混合(linear blend)或者说线性插值(liner interpolation)。
//     // 或者简称其为lerp。一个lerp一般来说会是下面的形式:
//     // blendedValue = (1 - t) * startValue + t * endValue
//     return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
// }

// 第6章更新后的函数ray_color
// color ray_color(const ray & r, const hittable & world) {
//     hit_record rec;
//     // 判断是否光线击中物体
//     if (world.hit(r, 0, infinity, rec)) {
//         // 直接使用法线值作为颜色输出
//         return 0.5 * (rec.normal + color(1, 1, 1));
//     }
//     vec3 unit_direction = unit_vector(r.direction());
//     // 该函数根据y值将蓝白做了个线性插值的混合, 我们这里把射线做了个单位化, 以保证y的取值范围(-1.0<y<1.0)。
//     // 因为我们使用y轴做渐变, 所以你可以看到这个蓝白渐变也是竖直的。
//     // 我们接下来使用了一个标准的小技巧将y的范围从-1.0 ≤ y ≤ 1.0映射到了0 ≤ y ≤ 1.0。
//     // 这样t=1.0时就是蓝色, 而t=0.0时就是白色。
//     auto t = 0.5 * (unit_direction.y() + 1.0);
//     // 在蓝白之间我想要一个混合效果(blend)。现在我们采用的是线性混合(linear blend)或者说线性插值(liner interpolation)。
//     // 或者简称其为lerp。一个lerp一般来说会是下面的形式:
//     // blendedValue = (1 - t) * startValue + t * endValue
//     return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
    
// }

// // 第8章使用我们vec3.h中新的生成随机随机反射方向的函数random_in_unit_sphere()来更新一下我们的ray_color()函数
// color ray_color(const ray & r, const hittable & world, int depth) {
//     hit_record rec;
//     // 注意ray_color函数是一个递归函数。那么递归终止的条件是什么呢?当它没有击中任何东西。
//     // 但是, 在某些条件下, 达到这个终止条件的时间会非常长, 长到足够爆了函数栈
//     // 【译注:想象一下一条光线在一个镜子材质的密封的盒子(并不吸收光线)中反复折射, 永无尽头】。
//     // 为了避免这种情况的发生, 我们使用一个变量depth限制递归层数。
//     // 当递归层数达到限制值时我们终止递归, 返回黑色，表示此处应该是多个物体之间的夹缝，非常暗。
//     if (depth <= 0)
//         return color(0, 0, 0);

//     // 判断是否光线击中物体
//     // 这里hit函数第二个参数还有个不太重要的潜在bug。
//     // 有些物体反射的光线会在t=0时再次击中自己。
//     // 然而由于精度问题, 这个值可能是t=-0.000001或者是t=0.0000000001或者任意接近0的浮点数。
//     // 所以我们要忽略掉0附近的一部分范围, 防止物体发出的光线再次与自己相交。
//     if (world.hit(r, 0.001, infinity, rec)) {
//         // 首先通过 rec.p + rec.normal 计算出单位球体的球心。
//         // 然后通过调用random_unit_vector()函数来一个随机向量。
//         // 通过 球心 + 随机向量 得出球体的一个随机点。
//         point3 target = rec.p + rec.normal + random_unit_vector();
//         // 前面的章节是直接使用法线值作为颜色输出
//         // return 0.5 * (rec.normal + color(1, 1, 1));

//         // 再通过 碰撞到rec.p 到 target - rec.p 来获取反弹后的射线。
//         // 然后用 0.5 * ray_color(...) 来递归调用该函数来计算最终碰到的背景色，再用0.5来衰减它。
//         return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
//     }
//     vec3 unit_direction = unit_vector(r.direction());
//     // 该函数根据y值将蓝白做了个线性插值的混合, 我们这里把射线做了个单位化, 以保证y的取值范围(-1.0<y<1.0)。
//     // 因为我们使用y轴做渐变, 所以你可以看到这个蓝白渐变也是竖直的。
//     // 我们接下来使用了一个标准的小技巧将y的范围从-1.0 ≤ y ≤ 1.0映射到了0 ≤ y ≤ 1.0。
//     // 这样t=1.0时就是蓝色, 而t=0.0时就是白色。
//     auto t = 0.5 * (unit_direction.y() + 1.0);
//     // 在蓝白之间我想要一个混合效果(blend)。现在我们采用的是线性混合(linear blend)或者说线性插值(liner interpolation)。
//     // 或者简称其为lerp。一个lerp一般来说会是下面的形式:
//     // blendedValue = (1 - t) * startValue + t * endValue
//     return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);

// 第9章使用材质反射来更新一下我们的ray_color()函数
color ray_color(const ray & r, const hittable & world, int depth) {
    hit_record rec;
    // 注意ray_color函数是一个递归函数。那么递归终止的条件是什么呢?当它没有击中任何东西。
    // 但是, 在某些条件下, 达到这个终止条件的时间会非常长, 长到足够爆了函数栈
    // 【译注:想象一下一条光线在一个镜子材质的密封的盒子(并不吸收光线)中反复折射, 永无尽头】。
    // 为了避免这种情况的发生, 我们使用一个变量depth限制递归层数。
    // 当递归层数达到限制值时我们终止递归, 返回黑色，表示此处应该是多个物体之间的夹缝，非常暗。
    if (depth <= 0)
        return color(0, 0, 0);

    // 判断是否光线击中物体
    // 这里hit函数第二个参数还有个不太重要的潜在bug。
    // 有些物体反射的光线会在t=0时再次击中自己。
    // 然而由于精度问题, 这个值可能是t=-0.000001或者是t=0.0000000001或者任意接近0的浮点数。
    // 所以我们要忽略掉0附近的一部分范围, 防止物体发出的光线再次与自己相交。
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    // 该函数根据y值将蓝白做了个线性插值的混合, 我们这里把射线做了个单位化, 以保证y的取值范围(-1.0<y<1.0)。
    // 因为我们使用y轴做渐变, 所以你可以看到这个蓝白渐变也是竖直的。
    // 我们接下来使用了一个标准的小技巧将y的范围从-1.0 ≤ y ≤ 1.0映射到了0 ≤ y ≤ 1.0。
    // 这样t=1.0时就是蓝色, 而t=0.0时就是白色。
    auto t = 0.5 * (unit_direction.y() + 1.0);
    // 在蓝白之间我想要一个混合效果(blend)。现在我们采用的是线性混合(linear blend)或者说线性插值(liner interpolation)。
    // 或者简称其为lerp。一个lerp一般来说会是下面的形式:
    // blendedValue = (1 - t) * startValue + t * endValue
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
    
}


int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    // 单个像素采样次数
    const int samples_per_pixel = 100;
    // 该变量限制ray_color()函数的递归层数，避免递归爆炸
    const int max_depth = 50;

    // World
    hittable_list world;
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.1);

    // 加入金属球：
    // 这里的point3(0, -100.5, -1)和100是sphere构造函数的参数
    // 调用的构造函数是sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};
    // 所以point3(0, -100.5, -1)是球心，100是半径
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    // 虚拟视口高度
    // auto viewport_height = 2.0;
    // auto viewport_width = aspect_ratio * viewport_height;
    // auto focal_length = 1.0;

    // auto origin = point3(0, 0, 0);
    // auto horizontal = vec3(viewport_width, 0, 0);
    // auto vertical = vec3(0, viewport_height, 0);
    // auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // 因为我们把camera封装成了一个类，所以上面的7行代码不用了，直接新建一个camera对象
    camera cam;

    // Render
    std::ofstream outfile;
    std::string filePath = "ch9.txt";
    outfile.open(filePath);
    outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        // std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            // // 偏移向量的比例值
            // auto u = double(i) / (image_width - 1);
            // auto v = double(j) / (image_height - 1);
            // // 此处ray的构造函数是ray(const point3 & origin, const vec3 & direction)，注意形参含义
            // ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            // color pixel_color = ray_color(r, world);
            // write_color(outfile, pixel_color);
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                // 偏移向量的比例值，随机生成像素内采样点的射线
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(outfile, pixel_color, samples_per_pixel);
        }
    }
    outfile.close();
    // std::cerr << "\nDone.\n";
    return 0;
}