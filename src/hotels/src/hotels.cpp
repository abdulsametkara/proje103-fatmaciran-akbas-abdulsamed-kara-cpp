#include "../header/hotels.h"
#include <stdexcept>

using namespace Coruh::Hotels;

double Hotels::add(double a, double b) {
    return a + b;
}

double Hotels::subtract(double a, double b) {
    return a - b;
}

double Hotels::multiply(double a, double b) {
    return a * b;
}

double Hotels::divide(double a, double b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero is not allowed.");
    }
    return a / b;
}