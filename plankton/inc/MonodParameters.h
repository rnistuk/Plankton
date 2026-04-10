#pragma once
#include <stdexcept>

struct MonodParameters {
    double Ks;
    double mu_max;
    double Yx_s;
    double Ki; // light half-saturation constant (µmol photons m⁻² s⁻¹)
    double kd;
    double dt;

    MonodParameters(double Ks, double mu_max, double Yx_s, double Ki, double kd, double dt) :
        Ks(Ks), mu_max(mu_max), Yx_s(Yx_s), Ki(Ki), kd(kd), dt(dt) {
        if (Ki <= 0) {
            throw std::invalid_argument("Ki must be positive");
        }

        if (kd < 0) {
            throw std::invalid_argument("kd must be non-negative");
        }
    }
};
