#include <cute_end.h>

#include "../main/scenegraph/math/EVector3.h"
#include "../main/scenegraph/math/EMatrix4.h"

using namespace end2;

void test_matrix(){
    EMatrix4 tm
    (1.0, 0.0, 0.0, 0.0,
     0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 1.0);

    // test identity
    ASSERT(tm.get(EMatrix4::_11) == 1.0);
    ASSERT(tm.get(EMatrix4::_21) == 0.0);
    ASSERT(tm.get(EMatrix4::_22) == 1.0);

    EVector3 result;

    // test translation
    EMatrix4::makeTransTM(tm, EVector3(2, 3, 5));
    EMatrix4::multiply(result, tm, EVector3(7, 11, 13));
    ASSERT_EQUAL(9, result.x);
    ASSERT_EQUAL(14, result.y);
    ASSERT_EQUAL(18, result.z);

    // test scale
    EMatrix4::makeScaleTM(tm, EVector3(2, 3, 5));
    EMatrix4::multiply(result, tm, EVector3(7, 11, 13));
    ASSERT_EQUAL(14, result.x);
    ASSERT_EQUAL(33, result.y);
    ASSERT_EQUAL(65, result.z);
}

CUTE_TEST(test_matrix);
