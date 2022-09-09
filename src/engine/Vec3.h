#pragma once

template<typename T>
struct Vec3 {
    T x, y, z;
    float r, g, b;
    float nx, ny, nz;
    float tx, ty;

    Vec3(T _x, T _y, T _z, float _r = 1, float _g = 1, float _b = 1)
        : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), nx(0), ny(0), nz(0), tx(0), ty(0) {
    }
    Vec3(T _x, T _y, T _z, float _r, float _g, float _b, float _nx, float _ny, float _nz)
        : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), nx(_nx), ny(_ny), nz(_nz), tx(0), ty(0) {
    }
    Vec3(T _x, T _y, T _z, float _r, float _g, float _b, float _nx, float _ny, float _nz, float _tx, float _ty)
        : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), nx(_nx), ny(_ny), nz(_nz), tx(_tx), ty(_ty) {
    }
    Vec3() : x(0), y(0), z(0), r(1), g(1), b(1), nx(0), ny(0), nz(0), tx(0), ty(0) {}
    ~Vec3() = default;
};
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;