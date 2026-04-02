#pragma once

struct SimulationRecord {
    double X;
    double S;
    double I_avg; // depth-averaged irradiance (m^-2 s^-1)

    SimulationRecord(double X, double S, double I_avg) : X(X), S(S), I_avg(I_avg) {}
};