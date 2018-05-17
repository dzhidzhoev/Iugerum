#include "integral.h"

#include <math.h>

double integral(math_function f, double a, double b, double eps) {
	double left = f(a), right = f(b), middle = f( (a + b) / 2 );
	double left_old, right_old, middle_old;
	double len = b - a;
	do {
		left_old = left;
		right_old = right;
		middle_old = middle;
		left = left_old + middle_old;
		right = right_old + middle_old;
		middle = 0;
		
		len /= 2;
		for (double x = a + len / 2; x < b; x += len)
			middle += f(x);
	} while (1.0 / 15.0 * fabs(len / 6 * (left + 4 * middle + right) - 2 * len / 6 * (left_old + 4 * middle_old + right_old)) >= eps);
	return len / 6 * (left + 4 * middle + right);
}
