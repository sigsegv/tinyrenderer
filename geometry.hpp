#pragma once

#include "vector2.hpp"
#include "vector3.hpp"
#include "matrix.hpp"

using vector2i = vector2<int>;
using vector2f = vector2<float>;
using vector3i = vector3<int>;
using vector3f = vector3<float>;
using matrix21f = matrix<float,2,1>;
using matrix31f = matrix<float,3,1>;

/**
 * Compute barycentric of 2d triangle defined by v1, v2, v3.
 *
 * @param v1 a point of the triangle
 * @param v2 a point of the triangle
 * @param v3 a point of the triangle
 * @param p a point to test
 * @return barycentric vector, if any component is negative, p lies outside of the triangle
 */
template<typename T>
vector3f barycentric(const vector2<T>& v1, const vector2<T>& v2, const vector2<T>& v3, const vector2<T>& p)
{
    vector3<T> m{v3.x - v1.x, v2.x - v1.x, v1.x - p.x};
    vector3<T> n{v3.y - v1.y, v2.y - v1.y, v1.y - p.y};
    vector3<T> u = m.cross(n);
    if(std::abs(u.z) < 1) return {-1.f, 1.f, 1.f};
    vector3f uf = {static_cast<float>(u.x), static_cast<float>(u.y), static_cast<float>(u.z)};
    return {1.f - (uf.x + uf.y) / uf.z, uf.y / uf.z, uf.x / uf.z};
}

// source: https://gamedev.stackexchange.com/a/23745
template<typename T>
vector3f barycentric3(const vector3<T>& a, const vector3<T>& b, const vector3<T>& c, const vector3<T>& p)
{
    vector3<T> v0 = b - a;
    vector3<T> v1 = c - a;
    vector3<T> v2 = p - a;
    const float d00 = v0.dot(v0);
    const float d01 = v0.dot(v1);
    const float d11 = v1.dot(v1);
    const float d20 = v2.dot(v0);
    const float d21 = v2.dot(v1);
    const float denom = d00 * d11 - d01 * d01;
    if(denom == 0.f)
    {
        return {-1.f, 0.f, 0.f};
    }
    const float v = (d11 * d20 - d01 * d21) / denom;
    const float w = (d00 * d21 - d01 * d20) / denom;
    const float u = 1.f - v - w;
    return {u, v, w};
}

inline matrix41f vec3f_to_mat41f(const vector3f& v)
{
    return matrix41f{v.x, v.y, v.z, 1.f};
}

inline matrix31f vec3f_to_mat31f(const vector3f& v)
{
    return matrix31f{v.x, v.y, v.z};
}

inline vector3f mat41f_to_vec3f(const matrix41f& m)
{
    return vector3f{m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]};
}

inline vector3i vec3f_to_vec3i(const vector3f& v)
{
    return { static_cast<int>(v.x + .5f), static_cast<int>(v.y + .5f), static_cast<int>(v.z + .5f) };
}

inline vector2f mat21f_to_vec2f(const matrix21f& m)
{
    return {m[0][0], m[1][0]};
}
