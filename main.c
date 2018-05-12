#include <string.h>
#include <stdio.h>

#include "functions.h"
#include "integral.h"
#include "roots.h"

math_function f[3] = {
	f1, f2, f3
};
math_function g[3] = {
	g1, g2, g3
};

int main(int argc, char *argv[]) {
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
			} else {
				fprintf(stderr, "Incorrect number of arguments for -integral command!\n");
				return 1;
			}
		}
	}

	return 0;
}
