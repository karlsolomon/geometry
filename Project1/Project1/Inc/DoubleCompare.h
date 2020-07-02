#pragma once
#include <cmath>
#include <cstdlib>
#define DOUBLE_TOLERANCE (0.000000001)
#define D_EQUALS(d1,d2) ((isinf(d1) && isinf(d2)) || (abs((d1) - (d2)) < DOUBLE_TOLERANCE))
