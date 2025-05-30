#include <iostream>
using namespace std;

int square(int x)
{
    int result = x * x;
    return result;
}

double square(double x)
{
    double result = x * x;
    return result;
}

int main()
{
    int num = 4;
    double pi = 3.14;
    cout << square(num) << endl;
    cout << square(pi) << endl;
    
    int sum = 0;
    for(int i = 1; i <= 5; i++) {
        sum = sum + i;
    }
    cout << "Sum: " << sum << endl;
    return 0;
}