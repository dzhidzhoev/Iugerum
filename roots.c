#include "roots.h"

#include <math.h>

double root(math_function f1, math_function f2, math_function g1, math_function g2,
			double a, double b, double eps1) {
	double x0 = a, x1 = b;
	while (fabs(x0 - x1) / 2 < eps1) {
		
	}
	return (x0 + x1) / 2;
}