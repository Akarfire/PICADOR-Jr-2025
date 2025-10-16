#pragma once

#include "Vector3.h"

struct Particle
{
	double mass, charge;

	Vector3 location;
	Vector3 velocity;

	Particle(double inMass, double inCharge, const Vector3& initialLocation, const Vector3 initialVelocity): mass(inMass), charge(inCharge), location(initialLocation), velocity(initialVelocity) {}
};