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

#include "TestPiiTypeTraits.h"


#include <PiiTypeTraits.h>
#include <QtTest>

struct Base {};
struct Derived : Base {};

void TestPiiTypeTraits::isTests()
{
#define TEST(TRAIT, TRUETYPE, FALSETYPE) \
  QVERIFY(Pii::TRAIT<TRUETYPE>::boolValue);     \
  QVERIFY(!Pii::TRAIT<FALSETYPE>::boolValue)

  TEST(IsPointer, int*, int);
  TEST(IsReference, int&, int);
  TEST(IsArray, int[], int);
  TEST(IsSigned, int, unsigned int);
  TEST(IsUnsigned, unsigned char, long);
  TEST(IsInteger, short, bool);
  TEST(IsFloatingPoint, double, int);
  TEST(IsNumeric, float, bool);
  TEST(IsPrimitive, bool, Base);
  TEST(IsConst, const Derived, Derived);

  #define DERIVED_TEST(NOT, BASE,DERIVED)                         \
    typedef Pii::IsBaseOf<BASE,DERIVED> IsBaseOf ## BASE ## DERIVED;  \
    QVERIFY(NOT IsBaseOf ## BASE ## DERIVED::boolValue)

  DERIVED_TEST(, Base, Derived);
  DERIVED_TEST(, Base, Base);
  DERIVED_TEST(, Derived, Derived);
  DERIVED_TEST(!, Derived, Base);
}

QTEST_MAIN(TestPiiTypeTraits)
