#pragma once
#include <stdexcept>

struct MonodState {
    double X; // biomass concentration
    double S; // nutrient concentration
    MonodState(double X, double S) : X(X), S(S) {
        if (this->X < 0) {
            throw std::invalid_argument("Biomass cannot be negative");
        }

        if (this->S < 0) {
            throw std::invalid_argument("Substrate concentration cannot be negative");
        }
    }
};
