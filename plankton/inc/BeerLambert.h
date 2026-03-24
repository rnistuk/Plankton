#pragma once
#include "ReactorGeometry.h"

double beerLambert(double depth, const ReactorGeometry& geometry, double X);

double depthAveragedIrradiance(const ReactorGeometry& geometry, double X);
