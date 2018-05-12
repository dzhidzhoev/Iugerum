#include "integral.h"

#include <math.h>

double integral(math_function f, double a, double b, double eps) {
	unsigned n = 1000;
	double first, second = (b - a) / 6 * (f(a) + 4 * f((b - a) / 2) + f(b));
	do {
		first = second;
		n *= 2;
		second = 0;
		
		double len = (b - a) / n;
		for (int i = 0; i < n; i++) {
			second += len / 6 * (f(i * len) + f(i * len + len) + 4 * f(i * len + len / 2));
		}
	} while (1.0 / 15.0 * fabs(first - second) >= eps);
	return second;
}
