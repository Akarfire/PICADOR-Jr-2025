#pragma once

#include "Vector3.h"
#include "Constants.h"

enum class ParticleType {Photon, Proton, Electron, Positron};

struct Particle
{
	ParticleType type;

	double mass, charge;

	Vector3 location;
	Vector3 impulse;

	bool transferFlag;

	Particle(double inMass, double inCharge, const Vector3& initialLocation, const Vector3 initialImpulse): mass(inMass), charge(inCharge), location(initialLocation), impulse(initialImpulse) {}

	Vector3 getVelocity() const
	{
		return impulse / (mass * sqrt(1 + (impulse.sizeSquared() / (mass * mass * Constants::SpeedOfLight * Constants::SpeedOfLight))));
	}
};