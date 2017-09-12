#pragma once

template<typename T>
class matrix4
{
public:
    /**
     * Create identity matrix
     */
    matrix4();
    
    matrix4(T a00, T a01, T a02, T a03, T a10, T a11, T a12, T a13, T a20, T a21, T a22, T a23, T a30, T a31, T a32, T a33);
    
//    matrix4(const matrix4& other);
    
//    matrix4& operator=(const matrix4& other);
    
    T (& operator[](unsigned char idx)) [4] { return m[idx]; }
    const T (& operator[](unsigned char idx) const) [4] { return m[idx]; }
    matrix4 operator*(const matrix4& rhs) const;
    
    
protected:
private:
    T m[4][4];
};

//template<typename T, unsigned char kRows, unsigned char kCols>
//class matrix
//{
//public:
//    /**
//     * Create identity matrix
//     */
//    matrix();
//    
//    matrix(const T val[kRows][kCols]);
//    
//    T (& operator[](unsigned char idx)) [kCols] { return m[idx]; }
//    const T (& operator[](unsigned char idx) const) [kCols] { return m[idx]; }
//    
//    matrix& operator*(const matrix& rhs) const;
//protected:
//private:
//    T m[kRows][kCols];
//};

void matrix_test();
