#include "Module_ParticleSolver.h"
#include "ParticleGrid.h"
#include "PicadorJrCore.h"
#include "Constants.h"

// Calculates new particle velocity based on the provided field value, using Boris's Method
Vector3 ParticleSolver::CalculateNewParticleVelocity(const Particle& particle, const FieldData& field, double timeDelta)
{
    Vector3 p_old = particle.velocity * particle.mass * Constants::SpeedOfLight 
                    / (sqrt(Constants::SpeedOfLight * Constants::SpeedOfLight - particle.velocity.sizeSquared()));

    Vector3 u_old = p_old / (particle.mass * Constants::SpeedOfLight);
    Vector3 EMult = field.E * (particle.charge * timeDelta / (2 * particle.mass * Constants::SpeedOfLight));
    Vector3 u_minus = u_old + EMult;

    double gamma_old = sqrt(1 + u_old.sizeSquared());

    Vector3 t = field.B * (particle.charge / (2 * gamma_old * particle.mass * Constants::SpeedOfLight));
    Vector3 u_mark = u_minus + u_minus.crossProduct(t);
    Vector3 s = t * (2 / (1 + t.sizeSquared()));
    Vector3 u_plus = u_minus + u_mark.crossProduct(s);

    Vector3 u_new = u_plus + EMult;
    Vector3 p_new = u_new * particle.mass * Constants::SpeedOfLight;

    
    Vector3 newVelocity = p_new / (particle.mass * sqrt(1 + (p_new.sizeSquared() / ((particle.mass * Constants::SpeedOfLight) 
                    * (particle.mass * Constants::SpeedOfLight)))));

    return newVelocity;
}

// Called on every iteration of the simulation loop
ModuleExecutionStatus ParticleSolver::onUpdate() 
{
    ParticleGrid* particleGrid = core->getParticleGrid();

    // Cell loop
    for (size_t cell_i = 0; cell_i < particleGrid->getResolutionY(); cell_i++)
        for (size_t cell_j = 0; cell_j < particleGrid->getResolutionX(); cell_j++)
        {
            std::vector<Particle>& particles = particleGrid->editParticlesInCell(cell_i, cell_j);

            // Particle loop
            for (size_t p = 0; p < particles.size(); p++)
            {
                // Fetching field data
                FieldData field = core->getFieldContainer()->getFieldsAt(particles[p].location);
                
                // Calclulating new velocity
                Vector3 newVelocity = CalculateNewParticleVelocity(particles[p], field, core->getTimeDelta());             

                // Updating particle location and velocity
                particles[p].location = particles[p].location + newVelocity * core->getTimeDelta();
                particles[p].velocity = newVelocity;
            }
        }

    return ModuleExecutionStatus::Success;
}