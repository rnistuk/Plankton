#include "BeerLambert.h"

#include <cmath>

double beerLambert(const double depth, const ReactorGeometry& geometry, const double X) {
    return geometry.I0 * exp(-geometry.k * X * depth);
}

double depthAveragedIrradiance(const ReactorGeometry& geometry, const double X) {
    if ( 1e-15 > X * geometry.k * geometry.depth ) {
        return geometry.I0;
    }
    return (geometry.I0 / (geometry.k * X * geometry.depth)) * (1.0 - exp(-geometry.k * X * geometry.depth));
}
