#include <stdio.h>
#include <stdlib.h>

#define TESTa 1

#ifdef TEST
#define TEST2 2
#elif TESTa
#define TEST3 3
#elif TESTb
#define TEST4 4
#endif

int main()
{
	int i = atoi("h");
	//printf("%d\n", TEST3);

	printf("%d", i);
	return 0;
}