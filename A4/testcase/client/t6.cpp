




// C++ program to illustrate 
// the SIGSEGV error 
#include <bits/stdc++.h> 
using namespace std; 

// Function with infinite 
// Recursion 
void infiniteRecur(int a) 
{ 
	return infiniteRecur(a); 
} 

// Driver Code 
int main() 
{ 

	// Infinite Recursion 
	infiniteRecur(5); 
} 

