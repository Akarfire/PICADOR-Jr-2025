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
	
	// Optional tracking ID for particle identification during visualization or data sampling
	unsigned short int trackingID = 0;

	Particle(double inMass, double inCharge, const Vector3& initialLocation, const Vector3 initialImpulse): mass(inMass), charge(inCharge), location(initialLocation), impulse(initialImpulse) {}
	Particle(const ParticleType& inType, double inMass, double inCharge, const Vector3& initialLocation = Vector3::Zero, const Vector3 initialImpulse = Vector3::Zero): type(inType), mass(inMass), charge(inCharge), location(initialLocation), impulse(initialImpulse) {}

	// Calculate and return particle velocity based on its impulse and mass
	Vector3 getVelocity() const
	{
		return impulse / (mass * sqrt(1 + (impulse.sizeSquared() / (mass * mass * Constants::SpeedOfLight * Constants::SpeedOfLight))));
	}
};