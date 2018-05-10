#ifndef _IUGERUM_ROOTS_H_
#define _IUGERUM_ROOTS_H_

#include "functions.h"

// f1 - first function, f2 - second function, g1 - first function's derivative
// g2 - second function's derivative
extern int last_root_call_num_steps;
double root(math_function f1, math_function f2, math_function g1, math_function g2,
			double a, double b, double eps1);

#endif