#pragma once

#include <cstdint>
#include <cmath>
#include <ostream>

using real_t = float;

/**
 * 2D Vector (or point)
 */
template <typename T>
class vector2
{
public:
    T x, y;
    
    /**
     * The length of this vector
     *
     * @returns the length of this vector
     */
    constexpr real_t magnitude() const noexcept
    {
        // TODO make constexpr
        return std::sqrt(x * x + y * y);
    }
    
    /**
     * This vector with length of 1
     *
     * @returns This vector with length of 1
     */
    vector2 unit() const
    {
        // TODO make constexpr
        const real_t mag = magnitude();
        if(mag == 0.0) return vector2{0.0,0.0};
        return (*this) / magnitude();
    }
    
    /**
     * Dot product of this and other
     *
     * @param other other vector
     * @returns Dot product of this and other
     */
    constexpr real_t dot(const vector2& other) const noexcept
    {
        return x * other.x + y * other.y;
    }
    
    /**
     * Get a normal vector of this vector
     *
     * @returns one of the two possible normal vectors
     */
    constexpr vector2 normal() const noexcept
    {
        return vector2{-y, x};
    }
    
    /**
     * The distance between this and other
     *
     * @param other other vector
     * @returns magnitude of this - other
     */
    constexpr real_t distance(const vector2& other) const noexcept
    {
        return operator-(other).magnitude();
    }
    
    /**
     * Get the angle between this and other
     * 
     * @param other other vector
     * @returns angle between this and other in radians
     */
    real_t angle(const vector2& other) const
    {
        return std::acos(dot(other) / magnitude() * other.magnitude());
    }
    
    constexpr vector2 operator+(const vector2& rhs) const noexcept
    {
        return vector2{x + rhs.x, y + rhs.y};
    }
    
    constexpr vector2 operator-(const vector2& rhs) const noexcept
    {
        return vector2{x - rhs.x, y - rhs.y};
    }
    
    constexpr vector2 operator-() const noexcept
    {
        return vector2{-x, -y};
    }
    
    constexpr vector2 operator*(T d) const noexcept
    {
        return vector2{x * d, y * d};
    }
    
    constexpr vector2 operator/(T d) const noexcept
    {
        return vector2{x / d, y / d};
    }
    
    constexpr bool operator!=(const vector2& rhs) const noexcept
    {
        return !(*this==rhs);
    }
    
    T& operator[](size_t pos)
    {
        if(pos == 0) return x;
        if(pos == 1) return y;
        throw std::out_of_range("this type only has 2 dimensions");
    }
    
    const T& operator[](size_t pos) const
    {
        if(pos == 0) return x;
        if(pos == 1) return y;
        throw std::out_of_range("this type only has 2 dimensions");
    }
    
    constexpr bool operator==(const vector2& rhs) const noexcept
    {
        // TODO https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
        return x == rhs.x && y == rhs.y;
    }
};
    
template<typename T>
std::ostream& operator<<(std::ostream& os, const vector2<T>& v)
{
    return os << "<" << v.x << ", " << v.y << ">";
}
