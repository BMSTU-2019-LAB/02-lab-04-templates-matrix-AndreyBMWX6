// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_MATRIX_HPP_
#define INCLUDE_MATRIX_HPP_

#pragma once
#include<math.h>
#include<type_traits>
#include<limits>

template <class T>
class Matrix
{
private:
 T **p;
 int rows, cols;
 static_assert(std::is_arithmetic<T>::value, "Non arithmetic type");
public:
 ~Matrix();
 Matrix();
 Matrix(int m, int n);
 Matrix(const Matrix& rhs);
 int get_rows() const;
 int get_columns() const;
 size_t Rows() const;
 size_t Cols() const;
 Matrix<T> Inverse();
 T* operator[](int i) const;
 Matrix& operator=(const Matrix<T>& rhs);
 template<class V>
 friend bool operator==(const Matrix<V>& m1, const Matrix<V>& m2);
 template<class V>
 friend bool operator!=(const Matrix<V>& m1, const Matrix<V>& m2);
 Matrix<T> operator+(Matrix<T>& m2);
 Matrix<T> operator-(Matrix<T>& m2);
 Matrix<T> operator*(Matrix<T>& m2);
 Matrix<T> remove_rows_and_cols(int i, int j);
 double Determinant(Matrix<T> m);
};

template<class T>
Matrix<T>::~Matrix()
{
 for (int i = 0; i < rows; i++)
  delete[] p[i];
 delete[] p;
}

template<class T>
Matrix<T>::Matrix()
{
 rows = cols = 0;
}

template<class T>
Matrix<T>::Matrix(int m, int n)
{
 rows = m;
 cols = n;
 p = new T*[m];
 for (int i = 0; i < m; i++)
  p[i] = new T[n];
 for (int i = 0; i < rows; i++)
  for (int j = 0; j < cols; j++)
   p[i][j] = 0;
}

template<class T>
Matrix<T>::Matrix(const Matrix& rhs)
{
 rows = rhs.rows;
 cols = rhs.cols;
 p = new T*[rows];
 for (int i = 0; i < rows; i++)
  p[i] = new T[cols];
 for (int i = 0; i < rows; i++)
  for (int j = 0; j < cols; j++)
   p[i][j] = rhs.p[i][j];
}

template<class T>
int Matrix<T>::get_rows() const
{
 return rows;
}

template<class T>
int Matrix<T>::get_columns() const
{
 return cols;
}

template<class T>
size_t Matrix<T>::Rows() const
{
 size_t r = rows;
 return r;
}

template<class T>
size_t Matrix<T>::Cols() const
{
 size_t c = cols;
 return c;
}

template<class T>
T* Matrix<T>::operator[](int i) const
{
 return p[i];
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs)
{
 if (&rhs != this)
 {
  rows = rhs.rows;
  cols = rhs.cols;
  for (int i = 0; i < rows; i++)
   for (int j = 0; j < cols; j++)
    p[i][j] = rhs[i][j];
 }
 return *this;
}

template<class T>
bool operator==(const Matrix<T>& m1, const Matrix<T>& m2)
{
 if (m1.rows != m2.rows || m1.cols != m2.cols)
  return false;
 else
 {
  for (int i = 0; i < m1.rows; i++)
   for (int j = 0; j < m1.cols; j++)
    if (m1.p[i][j] != m2.p[i][j])
     return false;
 }
 return true;
}

template<class T>
bool operator!=(const Matrix<T>& m1, const Matrix<T>& m2)
{
 return !(m1 == m2);
}

template<class T>
Matrix<T> Matrix<T>::operator+(Matrix<T>& m2)
{
 Matrix<T> m(0, 0);
 if (rows == m2.rows && cols == m2.cols)
 {
  Matrix<T> m(rows, cols);
  for (int i = 0; i < rows; i++)
   for (int j = 0; j < cols; j++)
    m[i][j] = p[i][j] + m2[i][j];
  return m;
 }
 return m;
}

template<class T>
Matrix<T> Matrix<T>::operator-(Matrix<T>& m2)
{
 Matrix<T> m(0, 0);
 if (rows == m2.rows && cols == m2.cols)
 {
  Matrix<T> m(rows, cols);
  for (int i = 0; i < m.rows; i++)
   for (int j = 0; j < m.cols; j++)
    m[i][j] = p[i][j] - m2[i][j];
  return m;
 }
 return m;
}

template<class T>
Matrix<T> Matrix<T>::operator*(Matrix<T>& m2)
{
 Matrix<T> m(0, 0);
 if (cols == m2.rows)
 {
  Matrix<T> m(rows, m2.cols);
  for (int i = 0; i < m.rows; i++)
   for (int j = 0; j < m.cols; j++)
    for (int k = 0; k < cols; k++)
     m[i][j] += p[i][k] * m2[k][j];
  return m;
 }
 return m;
}

template<class T>
Matrix<T> Matrix<T>::remove_rows_and_cols(int i, int j)
{
 Matrix<T> m(rows - 1, cols - 1);
 int rownum = 0;
 int colnum = 0;
 for (int a = 0; a < rows; a++)
 {
  if (a != i)
  {
   for (int b = 0; b < cols; b++)
   {
    if (b != j)
    {
     m[rownum][colnum] = p[a][b];
     colnum++;
    } else {
     continue;
    }
   }
   rownum++;
   colnum = 0;
  } else {
   continue;
  }
 }
 return m;
}

template<class T>
double Matrix<T>::Determinant(Matrix<T> m)
{
 double det = 0;
 if (m.rows > 2)
 {
  for (int i = 0; i < m.rows; i++)
   det += pow(-1, i) * m[0][i] * Determinant(m.remove_rows_and_cols(0, i));
 } else {
  det = m[0][0] * m[1][1] - m[1][0] * m[0][1];
 }
 return det;
}

template<class T>
Matrix<T> Matrix<T>::Inverse()
{
 double det = Determinant(*this);
 Matrix m(rows, cols);
 for (int i = 0; i < rows; i++)
  for (int j = 0; j < cols; j++)
  {
   m[i][j] = pow(-1, i + j) * Determinant(this->remove_rows_and_cols(i, j));
  }
   Matrix newm(rows, cols);
 for (int i = 0; i < rows; i++)
  for (int j = 0; j < cols; j++)
  {
   T el = m[j][i];
   T newel = el / det;
   newm[i][j] = newel;
  }
 return newm;
}

template<>
bool operator==(const Matrix<double>& m1, const Matrix<double>& m2)
{
 if (m1.rows != m2.rows || m1.cols != m2.cols)
  return false;
 else
 {
  for (int i = 0; i < m1.rows; i++)
   for (int j = 0; j < m1.cols; j++)
    if (abs(m1[i][j] - m2[i][j]) > std::numeric_limits<double>::epsilon())
     return false;
 }
 return true;
}

template<>
bool operator!=(const Matrix<double>& m1, const Matrix<double>& m2)
{
 return !(m1 == m2);
}

template<>
bool operator==(const Matrix<float>& m1, const Matrix<float>& m2)
{
 if (m1.rows != m2.rows || m1.cols != m2.cols)
  return false;
 else
 {
  for (int i = 0; i < m1.rows; i++)
   for (int j = 0; j < m1.cols; j++)
    if (abs(m1[i][j] - m2[i][j]) > std::numeric_limits<float>::epsilon())
     return false;
 }
 return true;
}

template<>
bool operator!=(const Matrix<float>& m1, const Matrix<float>& m2)
{
 return !(m1 == m2);
}

#endif // INCLUDE_MATRIX_HPP_

