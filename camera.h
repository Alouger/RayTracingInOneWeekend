#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
    public:
        camera() {
            // 屏幕高宽比
            auto aspect_ratio = 16.0 / 9.0;
            // 虚拟视口高度
            auto viewport_height = 2.0;
            auto viewport_width = aspect_ratio * viewport_height;
            // 焦距
            auto focal_length = 1.0;

            origin = point3(0, 0, 0);
            horizontal = vec3(viewport_width, 0, 0);
            vertical = vec3(0.0, viewport_height, 0.0);
            lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
        }

        ray get_ray(double u, double v) const {
            // 此处ray的构造函数是ray(const point3 & origin, const vec3 & direction)，注意形参含义
            return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
        }
    
    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif