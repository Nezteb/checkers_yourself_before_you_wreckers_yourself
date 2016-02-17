// sigmoid.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y sigmoid.cpp -o sigmoid.o && ./sigmoid.o

#include <cmath>
using std::tanh;

int main()
{
    volatile double x = 23.2;
    
    for(int i=0; i<100000000; ++i) // one hundred million
    {
        x = tanh(x);
        //x += 1;
    }
    return x;
}