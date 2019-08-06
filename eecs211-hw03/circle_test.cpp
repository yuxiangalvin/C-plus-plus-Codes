#include "circle_lib.h"

#include <UnitTest++/UnitTest++.h>
#include <stdexcept>

using namespace std;

TEST(OVERLAPPED)
{
    Circle c1 {0, 0, 1};
    Circle c2 {2, 2, 1};
    Circle c3 {1, 1, 1};
    Circle c4 {0, 2, 1};
    CHECK_EQUAL(true, overlapped(c1, c3);
    CHECK_EQUAL(false, overlapped(c1, c2);
    CHECK_EQUAL(false, overlapped(c1, c4);
}

TEST(OVERLAPPED_ERROR)
{
  // [YOUR CODE HERE]
}

