#pragma once
#include <UnitTest++/UnitTest++.h>

struct Circle
{
  double x, y;
  double radius;
};

Circle read_circle();
bool overlapped(Circle c1, Circle c2);