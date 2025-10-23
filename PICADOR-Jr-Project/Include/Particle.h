#pragma once

#include "Vector3.h"

enum class ParticleType {Photon, Proton, Electron, Positron};

struct Particle
{
	ParticleType type;

	double mass, charge;

	Vector3 location;
	Vector3 velocity;

	bool transferFlag;

	Particle(double inMass, double inCharge, const Vector3& initialLocation, const Vector3 initialVelocity): mass(inMass), charge(inCharge), location(initialLocation), velocity(initialVelocity) {}
};