/* This file is part of Into.
 * Copyright (C) Intopii 2013.
 * All rights reserved.
 *
 * Licensees holding a commercial Into license may use this file in
 * accordance with the commercial license agreement. Please see
 * LICENSE.commercial for commercial licensing terms.
 *
 * Alternatively, this file may be used under the terms of the GNU
 * Affero General Public License version 3 as published by the Free
 * Software Foundation. In addition, Intopii gives you special rights
 * to use Into as a part of open source software projects. Please
 * refer to LICENSE.AGPL3 for details.
 */

#ifndef _PIIVECTOR_H
#define _PIIVECTOR_H

#include "PiiTypeTraits.h"
#include "PiiArithmeticBase.h"
#include "PiiMath.h"
#include <functional>
#include <cstdarg>

// Forward declaration for rebinder
template <class T, int D> class PiiVector;

/**
 * Type information structure for PiiVector. See PiiArithmeticBase for
 * more information.
 *
 */
template <class T, int D>
struct PiiArithmeticTraits<PiiVector<T,D> >
{
  typedef T Type;
  typedef T* Iterator;
  typedef const T* ConstIterator;
  template <class U> struct Rebind { typedef PiiVector<U,D> Type; };
};

/**
 * A `D` -dimensional vector. PiiVector can be used to represent
 * points and vectors in multidimensional spaces. The size of a vector
 * is fixed, and all of its data is within the class itself. That is,
 * there is no heap-allocated data, and `sizeof(PiiVector<T,D>) ==
 * sizeof(T) * D.`
 *
 * Matrices and vectors are distinct entities, but a row of a matrix
 * can be treated as a vector:
 *
 * ~~~(c++)
 * typedef PiiVector<int,3> Vec3;
 * PiiMatrix<int> matData(1,3, 1,2,3);
 * Vec3& vec = *reinterpret_cast<Vec3*>(matData[0]);
 * // Better yet: vec = *matData.rowAs<Vec3>();
 * QCOMPARE(vec[0], 1);
 * QCOMPARE(vec[1], 2);
 * QCOMPARE(vec[2], 3);
 * ~~~
 *
 */
template <class T, int D> class PiiVector :
  public PiiArithmeticBase<PiiVector<T,D> >
{
public:
  /**
   * Shorthand for the base class type.
   */
  typedef PiiArithmeticBase<PiiVector<T,D> > BaseType;

  /**
   * Shorthand for variable argument type.
   */
  typedef typename Pii::VaArg<T>::Type VaArgType;

  /**
   * Create a new vector with all values initialized to zero.
   */
  PiiVector()
  {
    for (int i=0; i<D; ++i)
      values[i] = 0;
  }

  /**
   * Create a new vector by copying values from a preallocated array.
   *
   * @param ptr a pointer to the first value. This array must hold at
   * leasts `D` elements.
   */
  PiiVector(const T* ptr)
  {
    for (int i=0; i<D; ++i)
      values[i] = ptr[i];
  }

  /**
   * Create a new measumerent point.
   *
   * ~~~(c++)
   * PiiVector<int,3>* p = new PiiVector<int,3>(1, 2, 3);
   * PiiVector<double,4>* d = new PiiVector<double,4>(1.0, 2.0, 3.0, 4.0);
   *
   * PiiVector<double,4> pt(1, 2, 3, 4); // WRONG! Arguments are ints
   * ~~~
   *
   * ! Using this constructor is dangerous. Make sure that all
   * arguments are passed correctly.
   */
  PiiVector(VaArgType firstValue, ...)
  {
    values[0] = firstValue;
    va_list argp;
    // initalize var ptr
    va_start(argp, firstValue);
    // repeat for each arg
    for (int i=1; i<D; ++i)
      values[i] = static_cast<T>(va_arg(argp, VaArgType));

    // done with args
    va_end(argp);
  }

  /**
   * Copy another vector.
   */
  PiiVector& operator= (const PiiVector& other) { return BaseType::operator=(other); }

  /**
   * Returns an stl-style const iterator to the beginning of data.
   */
  typename BaseType::ConstIterator begin() const { return values; }
  /**
   * Returns an stl-style iterator to the beginning of data.
   */
  typename BaseType::Iterator begin() { return values; }
  /**
   * Returns an stl-style const iterator to the end of data.
   */
  typename BaseType::ConstIterator end() const { return values + D; }
  /**
   * Returns an stl-style iterator to the end of data.
   */
  typename BaseType::Iterator end() { return values + D; }

  /**
   * Calculate squared geometric distance to another vector.
   */
  double squaredDistance(const PiiVector& other) const
  {
    double sum = 0;
    for (int i=D; i--; )
      {
        double diff = double(other.values[i]) - double(values[i]);
        sum += diff*diff;
      }
    return sum;
  }

  /**
   * Get the squared length of this vector.
   */
  double squaredLength() const
  {
    double sum = 0;
    for (int i=D; i--; )
      sum += values[i]*values[i];
    return sum;
  }

  double length() const
  {
    return Pii::sqrt(squaredLength());
  }

  /**
   * Change vector to a unit vector with the original orientation. If
   * the length of the vector is zero, no changes will be made.
   */
  void normalize()
  {
    double length = squaredLength();

    // Avoid divzero and numerical instability
    if (Pii::almostEqualRel(length, 0.0, Pii::Numeric<double>::tolerance()))
      return;

    length = Pii::sqrt(length);

    for(int i=D; i--; )
      values[i] /= length;
  }

  /**
   * Get a unit vector that has the same orientation as this vector.
   * If the length of the vector is zero, the result will be equal to
   * this vector.
   */
  PiiVector normalized() const
  {
    double length = squaredLength();

    // Avoid divzero and numerical instability
    if(Pii::almostEqualRel(length, 0.0, Pii::Numeric<double>::tolerance()))
      return *this;

    length = Pii::sqrt<double>(length);

    PiiVector vec;

    for(int i=D; i--; )
      vec[i]= values[i] / length;

    return vec;
  }

  /**
   * Get the value of the vector at `index` as a copy.
   */
  T operator[] (int index) const
  {
    return values[index];
  }

  /**
   * Get a reference to the value of the vector at `index`.
   */
  T& operator[] (int index)
  {
    return values[index];
  }

  /**
   * Analogous to operator[]. This operator is useful for template
   * functions that work with both PiiMatrix and PiiVector.
   */
  T operator() (int index) const
  {
    return values[index];
  }

  /**
   * Analogous to operator[], non-const version. This operator is
   * useful for template functions that work with both PiiMatrix and
   * PiiVector.
   */
  T& operator() (int index)
  {
    return values[index];
  }

  /**
   * The coordinates.
   */
  T values[D];
};

namespace Pii
{
  /**
   */
  template <class T, int D> inline T innerProduct(const PiiVector<T,D>& v1, const PiiVector<T,D>& v2)
  {
    return innerProductN(v1.begin(), D, v2.begin());
  }
  /**
   */
  template <class T> inline T innerProduct(const PiiVector<T,2>& v1, const PiiVector<T,2>& v2)
  {
    return v1[0]*v2[0] + v1[1]*v2[1];
  }
  /**
   */
  template <class T> inline T innerProduct(const PiiVector<T,3>& v1, const PiiVector<T,3>& v2)
  {
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
  }
}


#endif //_PIIVECTOR_H
