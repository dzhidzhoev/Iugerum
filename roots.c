#include "roots.h"

#include <math.h>

static double calc(math_function f1, math_function f2, double x) {
	return f1(x) - f2(x);
}

double root(math_function f1, math_function f2, math_function g1, math_function g2,
			double a, double b, double eps1) {
	double left = a, right = b;
	while (fabs(left - right) / 2 < eps1) {
		// chord
		double new = left - calc(f1, f2, left) * (right - left) 
					/ (calc(f1, f2, right) - calc(f1, f2, left));
		if (signbit(calc(f1, f2, new)) == signbit(calc(f1, f2, left))) {
			left = new;
		} else {
			right = new;
		}
		
		// tangent
		if (fabs(calc(g1, g2, left)) > fabs(calc(g1, g2, right))) {
			right = right - calc(f1, f2, right) / calc(g1, g2, right);
		} else {
			left = left - calc(f1, f2, left) / calc(g1, g2, left);
		}
	}
	return (left + right) / 2;
}