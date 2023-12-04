#include <iostream>
#include <cmath>

using namespace std;

int main() {
    int base = 9;
    int exponent = 22;
    int modulus = 23;

    // Calculate (base ^ exponent) % modulus using pow from cmath
    double result = fmod(pow(7, 20), 23);

    std::cout << "Result: " << result << std::endl;

    return 0;
}
