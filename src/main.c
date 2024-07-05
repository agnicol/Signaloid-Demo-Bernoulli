#include "utilities.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef DEBUG
#include <uxhw.h>
#endif

// Constants from https://en.wikipedia.org/wiki/Density_of_air
const double g = 9.80665;       // Acceleration due to gravity (m/s^2)
const double p0 = 101325;       // Sea level standard pressure (Pa)
const double T0 = 288.15;       // Sea level standard temperature (K)
const double L = 0.0065;        // Temperature lapse rate (K/m)
const double R_ideal = 8.31446; // Universal gas constant
const double M_air = 0.0289652; // Molar mass of dry air (kg/mol)

// Calculate air density based on elevation
double calculate_air_density(double elevation_m) {
  double temperature_K;
  temperature_K = T0 - L * elevation_m;
  printf("Temperature (K): %.2f\n", temperature_K);
  double pressure_Pa;
  pressure_Pa =
      p0 * pow((1 - (L * elevation_m) / T0), ((g * M_air) / (R_ideal * L)));
  printf("Pressure (Pa): %.2f\n", pressure_Pa);

  return ((pressure_Pa * M_air) / (R_ideal * temperature_K));
}

// Calculate lift coefficient based on angle of attack (linear approximation)
double calculate_lift_coefficient(double AoA_deg) {
  double CL0; // Lift coefficient at zero AoA
#ifdef DEBUG
  CL0 = 0.2;
#else
  // Assume a cambered aerofoil with a CL0 value of between 0.2 and 0.6
  CL0 = UxHwDoubleUniformDist(0.2, 0.6);
#endif
  double slope; // Lift curve slope (linear variation)
#ifdef DEBUG
  slope = 0.1;
#else
  // Assume linear slope is somewhere between 0.1 and 0.2
  slope = UxHwDoubleUniformDist(0.1, 0.2);
#endif
  double AoA_rad = AoA_deg * M_PI / 180.0;
  return CL0 + slope * AoA_rad;
}

int main(int argc, char *argv[]) {
  CommandLineArguments arguments;

  double elevation_m; // Elevation above sea level in meters
#ifdef DEBUG
  elevation_m = 1000.0;
#else
  // Assume 0 to 10km operation
  elevation_m = UxHwDoubleUniformDist(0.0, 10000.0);
#endif
  printf("Elevation (meters): %.2f\n", elevation_m);

  double AoA_deg; // Angle of attack in degrees
#ifdef DEBUG
  AoA_deg = 5.0;
#else
  // Assume a safe (i.e. non-stall) AoA of -5 to +10 degrees
  AoA_deg = UxHwDoubleUniformDist(-5.0, 10.0);
#endif
  printf("Angle of attack (degrees): %.2f\n", AoA_deg);

  double air_density = calculate_air_density(elevation_m);
  printf("Air density: %.2f\n", air_density);
  double CL = calculate_lift_coefficient(AoA_deg);
  printf("Lift Coefficient: %.2f\n", CL);

  // Calculate lift force
  double lift_force = 0.5 * air_density * arguments.airspeed *
                      arguments.airspeed * arguments.wing_area * CL;

  printf("Air speed: %.2f m/s\n", arguments.airspeed);
  printf("Wing area: %.2f m^2\n", arguments.wing_area);
  printf("Lift Force: %.2f N\n", lift_force);

  return 0;
}
