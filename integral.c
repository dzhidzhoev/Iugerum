#include "integral.h"

double integral(math_function *f, double a, double b, double eps) {
	unsigned n = 1;
	double first, second = (b - a) / 6 * (f(a) + 4 * f((b - a) / 2) + f(b));
	do {
		first = second;
		n *= 2;
		second = 0;
		
		double len = (b - a) / n;
		for (int i = 0; i < n; i++) {
			second += len / 6 * (f(i * len) + f(i * len + len) + f(i * len + len / 2));
		}
	} while (fabs(first - second) >= eps);
	return second;
}