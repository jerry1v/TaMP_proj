#include "newton.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

std::string newtonMethod(double n, int iterations)
{
    if (n < 0)
        throw std::invalid_argument("Cannot compute sqrt of negative number");

    if (n == 0) return "0.0000000000";

    double x = n / 2.0;
    for (int i = 0; i < iterations; i++)
        x = (x + n / x) / 2.0;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(10) << x;
    return oss.str();
}
