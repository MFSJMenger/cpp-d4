/* This file is part of cpp-d4.
 *
 * Copyright (C) 2019-2020 Sebastian Ehlert
 *
 * cpp-d4 is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cpp-d4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with cpp-d4.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <cstdlib>
#include <cstring>

namespace dftd {

// Define a vector
template <class T>
class TVector {
 public:
  int N;            // Dimension of the vector
  int ElementSize;  // Size of each element in the vector
  T* p;             // The pointer to the vector

  TVector() {
    N = 0;
    p = 0;
    ElementSize = sizeof(T);
  }
  ~TVector() {
    if (p != 0) Delete();
  }
  void New(int VectorLength) {
    if (VectorLength < 0) {
      std::exit(EXIT_FAILURE);
    }
    if (p != 0 && N == VectorLength) {
      Init();
    } else {
      Delete();
      if (VectorLength == 0) {
        return;
      }
      // get new memory
      p = new T[VectorLength];
      if (!p) {
        std::exit(EXIT_FAILURE);
      }
      N = VectorLength;
      Init();
    }
  }
  void Delete(void) {
    if (p != 0 && N != 0) {
      delete[] p;
    }
    p = 0;
    N = 0;
  }
  void CopyVec(const TVector& v) {
    long int mem;
    if (N != v.N) {
      Delete();
      New(v.N);
    }
    if (v.N == 0) return;
    mem = (long int)N * ElementSize;
    std::memcpy(p, v.p, mem);
  }
  void Init(void) {
    if (p != 0) {
      long int mem = (long int)N * ElementSize;
      std::memset(p, 0, mem);
    }
  }

  inline T& operator()(int i) { return p[i]; }
  inline const T& operator()(int i) const { return p[i]; }
  inline T& operator[](int i) { return p[i]; }
  inline const T& operator[](int i) const { return p[i]; }
};

// Define a normal matrix
template <class T>
class TMatrix {
 public:
  int rows, cols;   // dimensions
  int ElementSize;  // Size of elements in matrix
  T* p;             // pointer to dynamic memory

  TMatrix() {
    cols = 0;
    rows = 0;
    p = 0;
    ElementSize = sizeof(T);
  }
  ~TMatrix() {
    if (p != 0) Delete();
  }

  void New(int r, int c) {
    if (r < 0 || c < 0) std::exit(EXIT_FAILURE);
    if (p != 0 && r == rows && c == cols) {
      Init();
    } else {
      long int mem = (long int)r * (long int)c;
      if (p != 0) Delete();  // Eventually delete old matrix

      if (mem == 0) return;  // don't touch pointer if no memory is allocated

      p = new T[mem];
      if (!p) std::exit(EXIT_FAILURE);
      rows = r;
      cols = c;
      Init();
    }
    return;
  }

  void New(const TMatrix& v) { New(v.rows, v.cols); }

  void Delete(void) {
    if (p != 0 && rows * cols != 0) {
      delete[] p;
    }
    rows = 0;
    cols = 0;
    p = 0;
  }

  void Init(void) {
    long int mem;
    if (p != 0) {
      mem = (long int)cols * (long int)rows * ElementSize;
      std::memset(p, 0, mem);
    }
  }

  void Transpose(void) {
    T x;
    int i, j;

    if (p != 0) {
      if (rows == cols) {
        for (i = 0; i < rows; i++) {
          for (j = 0; j < i; j++) {
            x = p[i * cols + j];
            p[i * cols + j] = p[j * cols + i];
            p[j * cols + i] = x;
          }  // j
        }    // i
      }      // if NxN
      else {
        // for non-square matrix, we need an additional copy
        TMatrix<T> temp;
        temp.CopyMat(*this);
        New(cols, rows);
        for (i = 0; i < rows; i++) {
          for (j = 0; j < cols; j++) {
            p[i * cols + j] = temp.p[j * cols + i];
          }  // j
        }    // i
      }
    }  // if data is loaded
  }    // for NxN matrices transpose elements

  void CopyMat(const TMatrix& m) {
    long int mem;

    if ((m.rows != rows) || (m.cols != cols)) {
      Delete();
      New(m.rows, m.cols);
    }
    mem = (long int)rows * (long int)cols * ElementSize;
    if (mem == 0) return;
    std::memcpy(p, m.p, mem);
  }

  inline T& operator()(int i, int j) { return p[i * cols + j]; }
  inline const T& operator()(int i, int j) const { return p[i * cols + j]; }
  inline T* operator[](int i) { return p + i * cols; }
};

}  // namespace dftd
