#include "roots.h"

#include <math.h>

static inline double calc(math_function f1, math_function f2, double x) {
	return f1(x) - f2(x);
}

int last_root_call_num_steps;

double root(math_function f1, math_function f2, math_function g1, math_function g2,
			double a, double b, double eps1) {
	double left = a, right = b;
	last_root_call_num_steps = 0;
	while (fabs(left - right) / 2 >= eps1) {
		// chord
		double new = left - calc(f1, f2, left) * (right - left) 
					/ (calc(f1, f2, right) - calc(f1, f2, left));
		if (calc(f1, f2, new) * calc(f1, f2, left) > eps1) {
			left = new;
		} else if (calc(f1, f2, new) * calc(f1, f2, left) < -eps1) {
			right = new;
		} else {
			left = right = new;
		}
		
		// tangent
		if (fabs(calc(g1, g2, left)) > fabs(calc(g1, g2, right))) {
			left = left - calc(f1, f2, left) / calc(g1, g2, left);
		} else {
			right = right - calc(f1, f2, right) / calc(g1, g2, right);
		}
		last_root_call_num_steps++;
		printf("%.5f %.5f\n", fabs(left), fabs(right)); // todo: remove this
	}
	return (left + right) / 2;
}
