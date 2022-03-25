#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "mx-new.h"
#include "mx-open.h"
#include "mx-list.h"
#include "mx-line.h"

#define ASSERT_EQ(A, B)		assert(strcmp(A, B) == 0)
#define ASSERT_EQF(A, B)	assert(strcmp(A, B) == 0); free(A)

#define ASSERT(C, A)		printf("%s\n", A); assert( mx_ ## C (A) )


void main() {
	
	system("rm -fr .tst/*");
	
	mx_inc = (char**) {".tst", NULL};
	
	assert( mx_new("show%") );
	assert( mx_line("10 print self") );

	assert( mx_run(" 10 show") );
}
