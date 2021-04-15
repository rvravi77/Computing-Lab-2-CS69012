#include <stdio.h>

int main(void) {
	char a[5];
	a[1] = '1';
	a[2] = '2';
	a[3] = '3';
	printf("%c\n", a[1]);
	printf("%c\n", a[2]);
	printf("%c\n", a[3]);
	return 0;
}