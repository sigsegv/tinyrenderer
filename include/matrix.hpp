#pragma once

#include <cassert>
#include <array>
#include <iostream>
#include <initializer_list>


template<typename T, unsigned kRows, unsigned kCols>
class matrix
{
public:
    /**
     * Create empty matrix
     */
    matrix() : m{}
    {
        
    }
    
    /**
     * Create matrix from initializer list. Values are set row first. Unset values are zero.
     */
    matrix(std::initializer_list<T> l) : m{}
    {
        assert(l.size() <= (kRows * kCols));
        unsigned row = 0, col = 0;
        for(auto v : l)
        {
            m[row][col] = v;
            ++col;
            if(col == kCols)
            {
                ++row;
                col = 0;
            }
        }
    }
    
    /**
     * Create identity matrix
     */
    static matrix identity()
    {
        matrix m;
        for(unsigned c = 0; c < kCols; ++c)
        {
            m[c][c] = 1.f;
        }
        return m;
    }
    
    /**
     * Return a matrix that represents this matrix flipped over its diagonal.
     */
    matrix<T, kCols, kRows> transpose() const
    {
        matrix<T, kCols, kRows> result;
        for(unsigned r = 0; r < kRows; ++r)
        {
            for(unsigned c = 0; c < kCols; ++c)
            {
                result[c][r] = m[r][c];
            }
        }
        return result;
    }
    
	/**
	 * Return the submatrix of this matrix by removing row and col.
	 */
    matrix<T, kRows - 1, kCols -1> submatrix(unsigned row, unsigned col) const
    {
        static_assert(kRows > 1 && kCols > 1, "Matrix too small to get submatrix");
        matrix<T, kRows - 1, kCols -1> result;
        for(unsigned r = 0; r < kRows; ++r)
        {
            if(r == row) continue;
            unsigned new_row = r > row ? r - 1 : r;
            for(unsigned c = 0; c < kCols; ++c)
            {
                if(c == col) continue;
                unsigned new_col = c > col ? c - 1 : c;
                result[new_row][new_col] = m[r][c];
            }
        }
        return result;
    }
    
    float determinant() const
    {
        return determinant_impl(*this);
    }
    
    matrix inverse() const
    {
        static_assert(kRows == kCols, "inverse only possible on square matrices");
        const float det = determinant();
        if(det == 0.f) throw std::runtime_error("matrix is singular (not invertable)");
        return inverse_impl(det, *this);
    }
    
    /**
     * Replace column with values from new 1 row matrix
     *
     * @return this
     */
    matrix& set_col(unsigned col, const matrix<T, 1, kRows>& v)
    {
        if(!(col < kCols)) throw std::runtime_error("Out of bounds col");
        for(unsigned i = 0; i < kRows; ++i)
        {
            m[i][col] = v[0][i];
        }
        return *this;
    }

    /**
     * @return row i
     */
    std::array<T, kCols>& operator[](size_t i)
    {
        assert(i < kRows);
        return m[i];
    }
    
    /**
     * @return row i
     */
    const std::array<T, kCols>& operator[](size_t i) const
    {
        assert(i < kRows);
        return m[i];
    }
    
    template<unsigned kRhsCols>
    matrix<T, kRows, kRhsCols> operator*(const matrix<T, kCols, kRhsCols>& rhs) const
    {
        matrix<T, kRows, kRhsCols> result;
        for(unsigned r = 0; r < kRows; ++r)
        {
            for(unsigned c = 0; c < kRhsCols; ++c)
            {
                T y = T(0);
                for(unsigned i = 0; i < kCols; ++i)
                {
                    y += m[r][i] * rhs[i][c];
                }
                result[r][c] = y;
            }
        }
        return result;
    }
    
    template<typename N>
    matrix operator*(const N& rhs) const
    {
        matrix<T, kRows, kCols> result;
        for(unsigned r = 0; r < kRows; ++r)
        {
            for(unsigned c = 0; c < kCols; ++c)
            {
                result[r][c] = m[r][c] * rhs;
            }
        }
        return result;
    }
    
    template<unsigned kRhsRows, unsigned kRhsCols>
    bool operator==(const matrix<T, kRhsRows, kRhsCols>& rhs) const
    {
        if(kRhsRows != kRows) return false;
        if(kRhsCols != kCols) return false;
        for(unsigned r = 0; r < kRows; ++r)
        {
            for(unsigned c = 0; c < kCols; ++c)
            {
                if(m[r][c] != rhs[r][c]) return false;
            }
        }
        return true;
    }
    
private:
    std::array< std::array<T, kCols>, kRows> m;
};
    
template<typename T, unsigned kRows, unsigned kCols>
float determinant_impl(const matrix<T, kRows, kCols>& m);
    
template<>
inline float determinant_impl<float, 2, 2>(const matrix<float, 2, 2>& m)
{
    return (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);
}
    
template<>
inline float determinant_impl<float, 3, 3>(const matrix<float, 3, 3>& m)
{
    return (m[0][0] * m.submatrix(0,0).determinant()) - (m[0][1] * m.submatrix(0,1).determinant()) + (m[0][2] * m.submatrix(0, 2).determinant());
}
    
template<>
inline float determinant_impl<float, 4, 4>(const matrix<float, 4, 4>& m)
{
    return (m[0][0] * m.submatrix(0,0).determinant()) - (m[0][1] * m.submatrix(0,1).determinant()) + (m[0][2] * m.submatrix(0, 2).determinant()) - (m[0][3] * m.submatrix(0, 3).determinant());
}
    
template<typename T, unsigned kRows, unsigned kCols>
inline matrix<T, kRows, kCols> inverse_impl(float determinant, const matrix<T, kRows, kCols>& m)
{
    const float denom = 1.f / determinant;
    matrix<T, kRows, kCols> result;
    for(unsigned r = 0; r < kRows; ++r)
    {
        for(unsigned c = 0; c < kCols; ++c)
        {
            auto mm = m.submatrix(r, c);
            result[r][c] = mm.determinant() * ((r + c) % 2 == 0 ? 1.f : -1.f);
        }
    }
    result = result.transpose();
    result = result * denom;
    return result;
}
    
template<>
inline matrix<float, 2, 2> inverse_impl<float, 2, 2>(float determinant, const matrix<float, 2, 2>& m)
{
    const float denom = 1.f / determinant;
    matrix<float, 2, 2> result = {m[1][1], -m[0][1], -m[1][0], m[0][0]};
    return result * denom;
}
    
template<typename N, typename T, unsigned kRows, unsigned kCols>
matrix<T, kRows, kCols> operator*(const N& n, const matrix<T, kRows, kCols>& m)
{
    return m * n;
}

template<typename T, unsigned kRows, unsigned kCols>
std::ostream& operator<<(std::ostream& os, const matrix<T, kRows, kCols>& m)
{
    for(unsigned r = 0; r < kRows; ++r)
    {
        os << "[";
        for(unsigned c = 0; c < kCols; ++c)
        {
            os << m[r][c];
            if(c + 1 < kCols) os << ", ";
        }
        os << "]";
        if(r + 1 < kRows) os << "\n";
    }
    return os;
}

using matrix41f = matrix<float, 4, 1>;
using matrix44f = matrix<float, 4, 4>;

