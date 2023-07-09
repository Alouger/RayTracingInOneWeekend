#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include "rtweekend.h"

using std::sqrt;

class vec3 {
    public:
        double e[3];
    public:
        // 使用初始化列表来初始化字段（例子）：
        // Line::Line( double len): length(len)
        // {
        //     cout << "Object is being created, length = " << len << endl;
        // }
        // 上面的语法等同于如下语法：
        // Line::Line( double len)
        // {
        //     length = len;
        //     cout << "Object is being created, length = " << len << endl;
        // }
        vec3() : e{0, 0, 0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
        // 将const修饰的“成员函数”称之为const成员函数，const修饰类成员函数，表明在该成员函数中不能对类的任何成员进行修改。
        // 返回值类型 函数名称 ( ) const {
                //函数体.....
                            // }
        // 到底要不要使用const去修饰成员函数，就看你函数中的变量需不需被修改，如果不希望被修改，则加上即可。
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }
        // 如果函数的返回值为引用，则不需要创建无名临时对象，避免构造函数和析构函数的调用，从空间和时间上提高了程序执行的效率。
        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(const double t) {
            return *this *= 1 / t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        inline static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        inline static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3; // RGB color

// vec3 Utility Functions

// 在 c/c++ 中，为了解决一些频繁调用的小函数大量消耗栈空间（栈内存）的问题，特别的引入了 inline 修饰符，表示为内联函数。
inline std::ostream& operator<<(std::ostream & out, const vec3 & v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 & u, const vec3 & v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 & u, const vec3 & v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 & u, const vec3 & v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 & v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 & v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3 & u, const vec3 & v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 & u, const vec3 & v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// 在第八章我们需要一个算法来生成球体内的随机点。我们会采用最简单的做法:否定法(rejection method)。
// 首先, 在一个xyz取值范围为-1到+1的单位立方体中选取一个随机点, 如果这个点在球外就重新生成直到该点在球内:
vec3 random_in_unit_sphere() {
    while (true) {
        // 生成单位球体的一个随机向量，x，y，z分量都是从-1到1的随机数
        auto p = vec3::random(-1, 1);
        // 如果随机向量的长度 >= 1 表示在单位球体之外，则重新随机生成
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

// 拒绝法生成的点是单位球体积内的的随机点, 这样生成的向量大概率上会和法线方向相近。
// 真正的lambertian散射后的光线距离法相比较近的概率会更高, 但是分布律会更加均衡。
// 这是因为我们选取的是单位球面上的点。
// 我们可以通过在单位球内选取一个随机点, 然后将其单位化来获得该点。
// 【译注: 然而下面的代码却用了极坐标的形式】
// 这与之前的拒绝法生成区别是随机生成的点都是在球面上，而不在球体里面
vec3 random_unit_vector() {
    auto a = random_double(0, 2 * pi);
    auto z = random_double(-1, 1);
    auto r = sqrt(1 - z * z);
    return vec3(r * cos(a), r * sin(a), z);
}

vec3 random_in_hemisphere(const vec3 & normal) {
    // 另一种具有启发性的方法是, 直接从入射点开始选取一个随机的方向, 然后再判断是否在法向量所在的那个半球。
    // 在使用lambertian漫发射模型前, 早期的光线追踪论文中大部分使用的都是这个方法
    vec3 in_unit_sphere = random_in_unit_sphere();
    // 如果是在法线相同方向的正半球，则直接返回这个随机单位向量
    if (dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        // 否则返回负的随机单位向量
        return -in_unit_sphere;
}

vec3 reflect(const vec3 & v, const vec3 & n) {
    return v - 2 * dot(v, n) * n;
}

#endif