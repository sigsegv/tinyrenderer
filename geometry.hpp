#pragma once

#include "vector2.hpp"
#include "vector3.hpp"

using vector2i = vector2<int>;
using vector2f = vector2<float>;
using vector3i = vector3<int>;
using vector3f = vector3<float>;

/**
 * Compute barycentric of 2d triangle defined by v1, v2, v3.
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
