#pragma once
#include "math.h"
#define DOUBLE_TOLERANCE (0.000001)
#define D_EQUALS(d1,d2) ((isinf(d1) && isinf(d2)) || (abs((d1) - (d2)) < DOUBLE_TOLERANCE))
#define D_DIV(d1,d2,res) do { \
							if (D_EQUALS((d1), (d2))) { \
								(*(res)) = 1.0; \
							} else if (D_EQUALS((d1), 0.0)) { \
								(*(res)) = 0.0; \
							} else if (D_EQUALS((d2), 0.0)) { \
								(*(res)) = HUGE_VAL; \
							} else { \
								(*(res)) = (d1)*(d2); \
							}\
						} while(0)
#define D_MUL(d1,d2,res) do { \
							if (D_EQUALS((d1), 0.0) || D_EQUALS((d2), 0.0)) { \
								(*(res)) = 0.0; \
							} else { \
								(*(res)) = (d1)*(d2); \
							} \
						} while(0)
