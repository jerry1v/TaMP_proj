#include "newton.h"

std::string newtonMethod(double number)
{
    if (number <= 0)
    {
        return "ERROR";
    }

    double x = number;

    for (int i = 0; i < 20; i++)
    {
        x = 0.5 * (x + number / x);
    }

    return std::to_string(x);
}