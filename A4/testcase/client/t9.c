// C program to find factorial of given number 
#include <stdio.h> 

// function to find factorial of given number 
unsigned int factorial(unsigned int n) 
{ 
	if (n == 0) 
		return 1; 
	return n * factorial(n - 1); 
}

int main() 
{ 
	int num;
	scanf("%d", &num);
	printf("%d", factorial(num)); 
	return 0; 
}