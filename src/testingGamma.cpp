#include <iostream>
#include <cmath>

void printGamma(float n) {
    std::cout << "gamma(" << n << ") = " << std::tgamma(n+1) << "\n";

}

int main() {

    printGamma(0);
    printGamma(0.5);
    printGamma(1);
    printGamma(2);
    printGamma(3.6);
    printGamma(8.9);
    printGamma(27.12);
    printGamma(3.14159);
    printGamma(-3.14159);
    printGamma(-0.5);
    printGamma(-6.3);
    printGamma(-1);

    return 0;
}