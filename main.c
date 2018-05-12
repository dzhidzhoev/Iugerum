#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

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
const double eps = 0.001;

static int cmp_double(const void *_l, const void *_r) {
	const double *l = _l;
	const double *r = _r;
	if (fabs(*l - *r) < eps) {
		return 0;
	}
	if (*l > *r)
		return 1;
	return -1;
}

// structure describing functions' intersection point,
// and what functions intersect in it
typedef struct {
		double x;
		int fnum_1, fnum_2;
} function_inter;

static int cmp_function_value(const void *l, const void *r) {
	return cmp_double(&((function_inter*)l)->x, &((function_inter*)r)->x);
}

static double find_area(const double eps, bool print_inters, bool print_iter_count) {
	function_inter x[3];
	x[0].x = root(f1, f2, g1, g2, left_border, right_border, eps / 10);
	x[0].fnum_1 = 0;
	x[0].fnum_2 = 1;
	if (print_iter_count) {
		printf("Finding intersections of f1 and f2 took %u iterations.\n", last_root_call_num_steps);
	}
	x[1].x = root(f1, f3, g1, g3, left_border, right_border, eps / 10);
	x[1].fnum_1 = 0;
	x[1].fnum_2 = 2;
	if (print_iter_count) {
		printf("Finding intersections of f1 and f3 took %u iterations.\n", last_root_call_num_steps);
	}
	x[2].x = root(f2, f3, g2, g3, left_border, right_border, eps / 10);
	x[2].fnum_1 = 1;
	x[2].fnum_2 = 2;
	if (print_iter_count) {
		printf("Finding intersections of f2 and f2 took %u iterations.\n", last_root_call_num_steps);
	}
	if (print_inters) {
		printf("f1 and f2 intersect at x = %.10f\n", x[0].x);
		printf("f1 and f3 intersect at x = %.10f\n", x[1].x);
		printf("f2 and f3 intersect at x = %.10f\n", x[2].x);
	}
	qsort(x, 3, sizeof(x[0]), cmp_double);
	
	// area on [x_0; x_1]
	double seg1_f1_val = f[x[0].fnum_1]((x[0].x + x[1].x) / 2);
	double seg1_f2_val = f[x[0].fnum_2]((x[0].x + x[1].x) / 2);
	double f2_int1 = integral(f[x[0].fnum_2], x[0].x, x[1].x, eps / 10);
	double f1_int1 = integral(f[x[0].fnum_1], x[0].x, x[1].x, eps / 10);
	double seg1_area = f2_int1 - f1_int1;
	printf("Segment 1[%.5f;%.5f] functions: %d %d\n", x[0].x, x[1].x, x[0].fnum_1, x[0].fnum_2);
	if (seg1_f1_val > seg1_f2_val) {
		seg1_area = - seg1_area;
	}
	// area on [x_1; x_2]
	double seg2_f1_val = f[x[2].fnum_1]((x[1].x + x[2].x) / 2);
	double seg2_f2_val = f[x[2].fnum_2]((x[1].x + x[2].x) / 2);
	double seg2_area = integral(f[x[2].fnum_2], x[1].x, x[2].x, eps / 10)
					- integral(f[x[2].fnum_1], x[1].x, x[2].x, eps / 10);
	printf("Segment 2[%.5f;%.5f] functions: %d %d\n", x[1].x, x[2].x, x[2].fnum_1, x[2].fnum_2);
	if (seg2_f1_val > seg2_f2_val) {
		seg2_area = - seg2_area;
	}
	printf("Area of segment 1: %.5f, segment 2: %.5f\n", seg1_area, seg2_area);
	return seg1_area + seg2_area;
}

int main(int argc, char *argv[]) {
	bool print_intersections = false, print_iterations_count = false;
	if (argc > 1) {
		if (strcmp(argv[1], "-root") == 0) {
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
		} else if (strcmp(argv[1], "-integral") == 0) {
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
		} else if (strcmp(argv[1], "-help") == 0) {
			printf("Iugerum - program, calculating areas.\n"
					"Usage: iugerum [-help|-root <function 1> <function 2> <a> <b> <eps>|\n"
					"		|-integral <function> <a> <b> <eps>|\n"
					"		| [ [--print-intersections] [--print-iter-count] ]]\n"
					"\n"
					"Modes:\n"
					"*default* 	-	calculates area of figure within graphs of 3 functions\n"
					"				on a segment\n" 
					"-help		-	prints this reference\n"
					"-root		-	calculates intersection of <function 1> and <function 2>\n"
					"				on segment [<a>;<b>] with <eps> tolerance\n"
					"-integral	-	calculates integral of <function> from <a> to <b> with\n"
					"				<eps> tolerance\n"
					"\n"
					"Options (when default mode is on):\n"
					"--print-intersections  - prints intersection points of three figures\n"
					"--print-iter-count     - prints iterations count spent to find intersection\n"
					"                         points of three functions\n"
					"\n");
					
			return 0;
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
	
	double area = find_area(eps, print_intersections, print_iterations_count);
	printf("Area: %.5f\n", area);

	return 0;
}
