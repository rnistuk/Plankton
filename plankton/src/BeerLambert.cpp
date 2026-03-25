#include "BeerLambert.h"

#include <cmath>

double beerLambert(double depth, const ReactorGeometry& geometry, double X) {
    return geometry.I0 * exp(-geometry.k * X * depth);
}

double depthAveragedIrradiance(const ReactorGeometry& geometry, double X) {
    if (X == 0.0) {
        return geometry.I0;
    }
    return (geometry.I0 / (geometry.k * X * geometry.depth)) * (1.0 - exp(-geometry.k * X * geometry.depth));
}
