#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "functions.h"
#include "integral.h"
#include "roots.h"

math_function f[3] = {
	f1, f2, f3
};
math_function g[3] = {
	g1, g2, g3
};

// these variables must be declared in code.asm
extern double left_border, right_border;

double find_area(double eps, bool print_inters, bool print_iter_count) {
	double f1f2 = root(f1, f2, g1, g2, left_border, right_border, eps / 10);
	if (print_iter_count) {
		printf("Finding intersections of f1 and f2 took %u iterations.\n", last_root_call_num_steps);
	}
	double f1f3 = root(f1, f3, g1, g3, left_border, right_border, eps / 10);
	if (print_iter_count) {
		printf("Finding intersections of f1 and f3 took %u iterations.\n", last_root_call_num_steps);
	}
	double f2f3 = root(f2, f3, g2, g3, left_border, right_border, eps / 10);
	if (print_iter_count) {
		printf("Finding intersections of f2 and f2 took %u iterations.\n", last_root_call_num_steps);
	}
	if (print_inters) {
		printf("f1 and f2 intersect at x = %.10f\n", f1f2);
		printf("f1 and f3 intersect at x = %.10f\n", f1f3);
		printf("f2 and f3 intersect at x = %.10f\n", f2f3);
	}
}

int main(int argc, char *argv[]) {
	bool print_intersections = false, print_iterations_count = false;
	if (argc > 1) {
		if (strcmp(argv[1], "root") == 0) {
			if (argc == 7) {
				int fnum1, fnum2;
				double a, b, eps;
				sscanf(argv[2], "%d", &fnum1);
				sscanf(argv[3], "%d", &fnum2);
				sscanf(argv[4], "%lf", &a);
				sscanf(argv[5], "%lf", &b);
				sscanf(argv[6], "%lf", &eps);
				double x = root(f[fnum1 - 1], f[fnum2 - 1], g[fnum1 - 1], g[fnum2 - 1],
								a, b, eps);
				printf("Root is %.30f\n", x);
				printf("Iterations count: %d\n", last_root_call_num_steps);
				return 0;
			} else {
				fprintf(stderr, "Incorrect number of arguments for -root command!\n");
				return 1;
			}
		} else if (strcmp(argv[1], "integral") == 0) {
			if (argc == 6) {
				int fnum1;
				double a, b, eps;
				sscanf(argv[2], "%d", &fnum1);
				sscanf(argv[3], "%lf", &a);
				sscanf(argv[4], "%lf", &b);
				sscanf(argv[5], "%lf", &eps);
				double x = integral(f[fnum1 - 1], a, b, eps);
				printf("Integral value is %.30f\n", x);
				return 0;
			} else {
				fprintf(stderr, "Incorrect number of arguments for -integral command!\n");
				return 1;
			}
		} else {
			for (int i = 1; i < argc; i++) {
				if (strcmp(argv[i], "--print-intersections") == 0) {
					print_intersections = true;
				} else if (strcmp(argv[i], "--print-iter-count") == 0) {
					print_iterations_count = true;
				}
			}
		}
	}
	
	double area = find_area(0.001, print_intersections, print_iterations_count);

	return 0;
}
