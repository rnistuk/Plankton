#pragma once
#include <stdexcept>

struct ReactorGeometry {
    double depth; // Depth of the reactor in meters
    double I0;   // surface irradiance at the reactor surface
    double k;     // extinction coefficient

    ReactorGeometry() = delete;

    ReactorGeometry(double depth, double I0, double k): depth(depth), I0(I0), k(k) {
        validateReactorGeometry();
    };

private:
    void validateReactorGeometry() const {
        if (depth <= 0) {
            throw std::invalid_argument("depth must be greater than zero");
        }

        if (I0 < 0) {
            throw std::invalid_argument("I0 must be greater than zero");
        }

        if (k <= 0) {
            throw std::invalid_argument("k must be greater than zero");
        }
    };


};
