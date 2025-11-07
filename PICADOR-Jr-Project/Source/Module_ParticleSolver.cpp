#include "Module_ParticleSolver.h"
#include "ParticleGrid.h"
#include "PicadorJrCore.h"
#include "Constants.h"

// Calculates new particle velocity based on the provided field value, using Boris's Method
Vector3 ParticleSolver::CalculateNewParticleImpulse(const Particle& particle, const FieldData& field, double timeDelta)
{
    Vector3 p_old = particle.impulse;

    Vector3 u_old = p_old / (particle.mass * Constants::SpeedOfLight);
    Vector3 EMult = field.E * (particle.charge * timeDelta / (2.0 * particle.mass * Constants::SpeedOfLight));
    Vector3 u_minus = u_old + EMult;

    double gamma_old = sqrt(1.0 + u_old.sizeSquared());

    Vector3 t = field.B * (particle.charge * timeDelta / (2.0 * gamma_old * particle.mass * Constants::SpeedOfLight));
    Vector3 u_mark = u_minus + u_minus.crossProduct(t);
    Vector3 s = t * (2.0 / (1.0 + t.sizeSquared()));
    Vector3 u_plus = u_minus + u_mark.crossProduct(s);

    Vector3 u_new = u_plus + EMult;
    Vector3 p_new = u_new * particle.mass * Constants::SpeedOfLight;

    return p_new;
}

// Called on every iteration of the simulation loop
ModuleExecutionStatus ParticleSolver::onUpdate() 
{
    ParticleGrid* particleGrid = core->getParticleGrid();

    // Cell loop
    for (size_t cell_i = 0; cell_i < particleGrid->getResolutionY() - 1; cell_i++)
        for (size_t cell_j = 0; cell_j < particleGrid->getResolutionX() - 1; cell_j++)
        {
            std::vector<Particle>& particles = particleGrid->editParticlesInCell(cell_i, cell_j);

            // Particle loop
            for (size_t p = 0; p < particles.size(); p++)
            {

                // Fetching field data
                FieldData field = core->getFieldContainer()->getFieldsAt(particles[p].location);
                
                // Calclulating new velocity
                Vector3 newImpulse = CalculateNewParticleImpulse(particles[p], field, core->getTimeDelta());
                
                particles[p].impulse = newImpulse;

                Vector3 newVelocity = particles[p].getVelocity();

                // if (newVelocity.sizeSquared() > Constants::SpeedOfLight * Constants::SpeedOfLight)
                //     throw(std::runtime_error("Exceeded speed of light!"));

                // Updating particle location and velocity
                particles[p].location = (particles[p].location + newVelocity * core->getTimeDelta()) * Vector3::VectorMaskXY;

                // Checking if the particle has left it's cell
                Vector3 particleLocation = particles[p].location;
                Vector3 cellLocation = particleGrid->getOrigin() + Vector3(cell_j * particleGrid->getDeltaX(), cell_i * particleGrid->getDeltaY());

                if (    particleLocation.x < cellLocation.x 
                    ||  particleLocation.y < cellLocation.y 
                    ||  particleLocation.x >= cellLocation.x + particleGrid->getDeltaX()
                    ||  particleLocation.y >= cellLocation.y + particleGrid->getDeltaY())
                {
                    particles[p].transferFlag = true;
                }
            }
        }

    // Particle transfer loop
    for (size_t cell_i = 0; cell_i < particleGrid->getResolutionY() - 1; cell_i++)
        for (size_t cell_j = 0; cell_j < particleGrid->getResolutionX() - 1; cell_j++)
        {
            std::vector<Particle>& particles = particleGrid->editParticlesInCell(cell_i, cell_j);

            for (size_t p = 0; p < particles.size(); p++)
                if (particles[p].transferFlag)
                {
                    particles[p].transferFlag = false;

                    std::pair<GRID_INDEX, GRID_INDEX> newCell = particleGrid->getCell(particles[p].location);

                    particleGrid->particleCellTransfer(p, cell_i, cell_j, newCell.first, newCell.second);
                }
        }

    return ModuleExecutionStatus::Success;
}