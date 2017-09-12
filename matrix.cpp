#include "matrix.hpp"
#include <cassert>

const unsigned int kRows = 4;
const unsigned int kCols = 4;

template<typename T>
matrix4<T>::matrix4()
{
    for(unsigned int i = 0; i < kRows; ++i)
    {
        for(unsigned int j = 0; j < kCols; ++j)
        {
            if(j == i) m[i][j] = T(1);
            else m[i][j] = T(0);
        }
    }
}

template<typename T>
matrix4<T>::matrix4(T a00, T a01, T a02, T a03, T a10, T a11, T a12, T a13, T a20, T a21, T a22, T a23, T a30, T a31, T a32, T a33)
{
    m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
    m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
    m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
    m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
}

//template<typename T>
//matrix4<T>::matrix4(const matrix4& other)
//{
//    return this->operator=(other);
//}
//
//template<typename T>
//matrix4<T>& matrix4<T>::operator=(const matrix4& other)
//{
//    if(this != &other)
//    {
//        m[0][0] = other[0][0]; m[0][1] = other[0][1]; m[0][2] = other[0][2];
//        m[1][0] = other[1][0]; m[1][1] = other[1][1]; m[1][2] = other[1][2];
//        m[2][0] = other[2][0]; m[2][1] = other[2][1]; m[2][2] = other[2][2];
//    }
//    return *this;
//}

template<typename T>
matrix4<T> matrix4<T>::operator*(const matrix4<T>& rhs) const
{
    matrix4 result;
    for(unsigned int i = 0; i < kRows; ++i)
    {
        for(unsigned int j = 0; j < kCols; ++j)
        {
            T y = T(0);
            for(unsigned int x = 0; x < kRows; ++x)
            {
                y += m[i][x] * rhs[x][j];
            }
            result[i][j] = y;
        }
    }
    return result;
}

void matrix_test()
{
    using matrix4f = matrix4<float>;

    matrix4f m0;
    assert(m0[0][0] == 1.f);
    assert(m0[0][1] == 0.f);
    assert(m0[3][3] == 1.f);

    matrix4f a(1.f,2.f,3.f, 4.f,5.f,6.f, 7.f,8.f,9.f,10.f,11.f,12.f,13.f,14.f,15.f,16.f);
    matrix4f b(1.f,1.f,1.f,1.f, 2.f,2.f,2.f,2.f, 3.f,3.f,3.f,3.f,4.f,4.f,4.f,4.f);
    matrix4f c = a*b;
}

//template<typename T, unsigned char kRows, unsigned char kCols>
//matrix<T, kRows, kCols>::matrix()
//{
//    for(unsigned char i = 0; i < kRows; ++i)
//    {
//        for(unsigned int j = 0; j < kCols; ++j)
//        {
//            if(j == i) m[i][j] = T(1);
//            else m[i][j] = T(0);
//        }
//    }
//}
//
//template<typename T, unsigned char kRows, unsigned char kCols>
//matrix<T, kRows, kCols>::matrix(const T val[kRows][kCols])
//{
//    
//}
//
//template<typename T, unsigned char kRows, unsigned char kCols>
//matrix<T, kRows, kCols>& matrix<T, kRows, kCols>::operator*(const matrix& rhs) const
//{
//    matrix<T, kRows, kCols> result;
//    for(unsigned char i = 0; i < kRows; ++i)
//    {
//        for(unsigned int j = 0; j < kCols; ++j)
//        {
//            T y = T(0);
//            for(unsigned int x = 0; x < kRows; ++x)
//            {
//                y += m[i][j] * rhs[j][i];
//            }
//            result[i][j] = y;
//        }
//    }
//    return result;
//}
//
//void matrix_test()
//{
//    using matrix3f = matrix<float, 3, 3>;
//    using matrix4f = matrix<float, 4, 4>;
//    
//    matrix3f m0;
//    assert(m0[0][0] == 1.f);
//    assert(m0[0][1] == 0.f);
//    
//    float arr[3][3] = {{1.f,2.f,3.f}, {4.f,5.f,6.f}, {7.f,8.f,9.f}};
//    matrix3f m1(arr);
//}
