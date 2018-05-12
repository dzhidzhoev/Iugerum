#include "integral.h"

#include <math.h>

double integral(math_function f, double a, double b, double eps) {
	unsigned n = 20;
	double first, second = (b - a) / 6 * (f(a) + 4 * f((b - a) / 2) + f(b));
	do {
		first = second;
		n *= 2;
		second = 0;
		
		double len = (b - a) / n;
		for (int i = 0; i < n; i++) {
			second += len / 6 * (f(a + i * len) + f(a + i * len + len) + 4 * f(a + i * len + len / 2));
		}
		printf("Step %d: %.5f\n", n, second); // todo: remove this
	} while (1.0 / 15.0 * fabs(first - second) >= eps);
	return second;
}
