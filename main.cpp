#include "color.h"
#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include <iostream>
#include <fstream>
#include <string>

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
color ray_color(const ray & r, const hittable & world) {
    hit_record rec;
    // 判断是否光线击中物体
    if (world.hit(r, 0, infinity, rec)) {
        // 直接使用法线值作为颜色输出
        return 0.5 * (rec.normal + color(1, 1, 1));
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

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    // 这里的point3(0, -100.5, -1)和100是sphere构造函数的参数
    // 调用的构造函数是sphere(point3 cen, double r) : center(cen), radius(r) {};
    // 所以point3(0, -100.5, -1)是球心，100是半径
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

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
    std::string filePath = "ch6.txt";
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
                pixel_color += ray_color(r, world);
            }
            write_color(outfile, pixel_color, samples_per_pixel);
        }
    }
    outfile.close();
    // std::cerr << "\nDone.\n";
    return 0;
}