#include<iostream>
#include<cmath>
using namespace std;
 
long gcd(long x, long y) {
  if (x == 0) {
    return y;
  }
  while (y != 0) {
    if (x > y) {
      x = x - y;
    }
    else {
      y = y - x;
    }
  }
  return x;
}
 
int primeFactors(int n)
{
    int c = 0;
    while (n%2 == 0)
    {
        n = n/2;
        if(c == 0)  c++;
    }
    for (int i = 3; i <= sqrt(n); i = i+2)
    {
        bool f = true;
        while (n%i == 0)
        {
            if(f)
            {
                c++;
                f = false;
            }
            n = n/i;
        }
    }
    if (n > 2)
        c++;
    return c;
}
int check_prime(int a)
{
   int c;
 
   for ( c = 2 ; c <= a - 1 ; c++ )
   {
      if ( a%c == 0 )
	 return 0;
   }
   if ( c == a )
      return 1;
}
int main()
{
        int a,b;
        cin >> a >> b;
        int k = gcd(a,b);
        int n = (a*b)/k;
        //cout << n << endl;
        int m = primeFactors(n);
        //cout << m << endl;
        if(m == 1) cout << "No" << endl;
        else if(check_prime(m)) cout << "Yes" << endl;
        else cout << "No" << endl;
    return 0;
}