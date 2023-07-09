#ifndef COLOR_H
#define COLOR_H

// #include "vec3.h"
#include "rtweekend.h"
#include <iostream>

// 第8章前的write_color()函数
// template<typename T>
// void write_color(T & out, color pixel_color, int samples_per_pixel) {
//     // 我们不会在每次发出射线采样时都计算一个0-1之间的颜色值, 
//     // 而是一次性把该像素内所有的颜色都加在一起, 然后最后只需要简单的一除(除以采样点个数)。
//     auto r = pixel_color.x();
//     auto g = pixel_color.y();
//     auto b = pixel_color.z();

//     // Divide the color by the number of samples
//     auto scale = 1.0 / samples_per_pixel;
//     r *= scale;
//     g *= scale;
//     b *= scale;
//     // Write the translated [0, 255] value of each color component.
//     out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
//         << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
//         << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
// }

// 第八章中修改了该函数：
// 因为所有的看图软件都默认图像已经经过了伽马校正(gamma corrected)。即在图片存入字节之前, 颜色值发生了一次转化。
// 这么做有许多好处, 但这并不是我们这里所讨论的重点。
// 我们使用"gamma 2"空间, 就意味着最终的颜色值要加上指数1/gamma, 在我们的例子里就是 ½, 即开平方根:
template<typename T>
void write_color(T & out, color pixel_color, int samples_per_pixel) {
    // 我们不会在每次发出射线采样时都计算一个0-1之间的颜色值, 
    // 而是一次性把该像素内所有的颜色都加在一起, 然后最后只需要简单的一除(除以采样点个数)。
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    // Write the translated [0, 255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

// void write_color(std::ofstream & out, color pixel_color) {
//     // Write the translated [0, 255] value of each color component.
//     out << static_cast<int>(255.999 * pixel_color.x()) << ' '
//         << static_cast<int>(255.999 * pixel_color.y()) << ' '
//         << static_cast<int>(255.999 * pixel_color.z()) << '\n';
// }

#endif