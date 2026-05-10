#include "BeerLambert.h"

#include <cmath>

double beerLambert(const double depth, const ReactorGeometry& geometry, const double X) {
    return geometry.I0 * exp(-geometry.k * X * depth);
}

double depthAveragedIrradiance(const ReactorGeometry& geometry, const double X) {
    double t = geometry.k * X * geometry.depth;
    if ( t < 1e-15 ) {
        return geometry.I0;
    }

    return (geometry.I0 / t) * (1.0 - exp(-t));
}
