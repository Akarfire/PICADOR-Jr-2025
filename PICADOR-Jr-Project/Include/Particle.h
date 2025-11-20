#pragma once

#include "Vector3.h"
#include "Constants.h"

enum class ParticleType {Photon, Proton, Electron, Positron};

struct Particle
{
	// Type of the particle (for complex interactions)
	ParticleType type;

	// Number of physical particles represented by this 'virtual' particle
	size_t weight;

	// Mass of a single particle
	double mass;
	// Charge of a single particle
	double charge;

	// Current location in world space
	Vector3 location;
	// Current impulse in world space
	Vector3 impulse;

	// Special flag, used for transferning particles between ParticleGrid cells
	bool transferFlag;
	
	// Optional tracking ID for particle identification during visualization or data sampling
	unsigned short int trackingID = 0;

	Particle(double mass_, double charge_, const Vector3& initialLocation, const Vector3 initialImpulse, size_t weight_ = 1): 
		mass(mass_), charge(charge_), location(initialLocation), impulse(initialImpulse), weight(weight_) {}

	Particle(const ParticleType& type_, double mass_, double charge_, size_t weight_ = 1, const Vector3& initialLocation = Vector3::Zero, const Vector3 initialImpulse = Vector3::Zero): 
		type(type_), mass(mass_), charge(charge_), location(initialLocation), impulse(initialImpulse), weight(weight_) {}

	// Calculate and return particle velocity based on its impulse and mass
	Vector3 getVelocity() const;

	// Converts velocity to impulse based on mass
	static Vector3 convertVelocityToImpulse(const Vector3& velocity, double mass);
};