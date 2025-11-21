#include "Particle.h"
#include <math.h>

// Calculate and return particle velocity based on its impulse and mass
Vector3 Particle::getVelocity() const
{
    return convertImpulseToVelocity(impulse, mass);
}

// Converts velocity to impulse based on mass
Vector3 Particle::convertImpulseToVelocity(const Vector3& impulse, double mass)
{
    return impulse / (mass * sqrt(1 + (impulse.sizeSquared() / (mass * mass * Constants::SpeedOfLight * Constants::SpeedOfLight))));
}

// Converts velocity to impulse based on mass
Vector3 Particle::convertVelocityToImpulse(const Vector3& velocity, double mass)
{
    return velocity * mass * Constants::SpeedOfLight / sqrt(Constants::SpeedOfLight * Constants::SpeedOfLight - velocity.sizeSquared());
}