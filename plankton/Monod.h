#pragma once

struct MonodState {
    double X; // biomass concentration
    double S; // nutrient concentration
};

struct MonodParameters {
    double Ks;
    double mu_max;
    double Yx_s;
    double dt;
};

double Monod(double S, double Ks, double mu_max);

MonodState eulerStep(const MonodState& state, const MonodParameters& params);
