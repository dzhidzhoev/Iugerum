#include "lexer.h"

int main(void) {
	set_lexer_input(stdin);
	TOKEN_TYPE tt;
	while ((tt = (next_token(), current_token()))
	 	!= TOKEN_EOF) {
	 	printf("%d %f\n", tt, current_number());
	}
	
	return 0;
}