#pragma once

#include <cstdint>
#include <cmath>
#include <ostream>

using real_t = float;

/**
 * 3D Vector (or point)
 */
template <typename T>
class vector3
{
public:
    T x, y, z;
    
    /**
     * The length of this vector
     *
     * @returns the length of this vector
     */
    constexpr real_t magnitude() const noexcept
    {
        // TODO make constexpr
        return std::sqrt(x * x + y * y + z * z);
    }
    
    /**
     * This vector with length of 1
     *
     * @returns This vector with length of 1
     */
    vector3 unit() const
    {
        // TODO make constexpr
        const real_t mag = magnitude();
        if(mag == 0.0) return vector3{0.0,0.0,0.0};
        return (*this) / mag;
    }
    
    /**
     * Dot product of this and other
     *
     * @param other other vector
     * @returns Dot product of this and other
     */
    constexpr real_t dot(const vector3& other) const noexcept
    {
        return x * other.x + y * other.y + z * other.z;
    }
    
    /**
     * Get cross product vector of this vector
     *
     * @returns vector representing cross product
     */
    constexpr vector3 cross(const vector3& other) const noexcept
    {
        return vector3{(y * other.z) - (z * other.y), (z * other.x) - (x * other.z), (x * other.y) - (y * other.x)};
    }
    
    /**
     * The distance between this and other
     *
     * @param other other vector
     * @returns magnitude of this - other
     */
    constexpr real_t distance(const vector3& other) const noexcept
    {
        return operator-(other).magnitude();
    }
    
    /**
     * Get the angle between this and other
     * 
     * @param other other vector
     * @returns angle between this and other in radians
     */
//    real_t angle(const vector3& other) const
//    {
//        return std::acos(dot(other) / magnitude() * other.magnitude());
//    }
    
    constexpr vector3 operator+(const vector3& rhs) const noexcept
    {
        return vector3{x + rhs.x, y + rhs.y, z + rhs.z};
    }
    
    constexpr vector3 operator-(const vector3& rhs) const noexcept
    {
        return vector3{x - rhs.x, y - rhs.y, z - rhs.z};
    }
    
    constexpr vector3 operator-() const noexcept
    {
        return vector3{-x, -y, -z};
    }
    
    constexpr vector3 operator*(T d) const noexcept
    {
        return vector3{x * d, y * d, z * d};
    }
    
    constexpr vector3 operator/(T d) const noexcept
    {
        return vector3{x / d, y / d, z / d};
    }
    
    constexpr bool operator!=(const vector3& rhs) const noexcept
    {
        return !(*this==rhs);
    }
    
    constexpr bool operator==(const vector3& rhs) const noexcept
    {
        // TODO https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
    
    T& operator[](size_t pos)
    {
        if(pos == 0) return x;
        if(pos == 1) return y;
        if(pos == 2) return z;
        throw std::out_of_range("this type only has 3 dimensions");
    }
    
    const T& operator[](size_t pos) const
    {
        if(pos == 0) return x;
        if(pos == 1) return y;
        if(pos == 2) return z;
        throw std::out_of_range("this type only has 3 dimensions");
    }
};

template<typename T>
constexpr vector3<T> operator*(const T& lhs, const vector3<T>& rhs)
{
    return {rhs[0] * lhs, rhs[1] * lhs, rhs[2] * lhs};
}
    
template<typename T>
std::ostream& operator<<(std::ostream& os, const vector3<T>& v)
{
    return os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
}
