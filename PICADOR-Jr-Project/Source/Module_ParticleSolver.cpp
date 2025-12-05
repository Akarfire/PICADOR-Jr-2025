#include "Module_ParticleSolver.h"
#include "ParticleGrid.h"
#include "PicadorJrCore.h"
#include "Constants.h"

#include <iostream>

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
    for (size_t cell_i = 0; cell_i < particleGrid->getResolutionX() - 1; cell_i++)
        for (size_t cell_j = 0; cell_j < particleGrid->getResolutionY() - 1; cell_j++)
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

                if (newVelocity.sizeSquared() > Constants::SpeedOfLight * Constants::SpeedOfLight)
                    throw(std::runtime_error("Particle " + std::to_string(p) + " at " + std::to_string(cell_i) + ", " + std::to_string(cell_j) + " exceeded speed of light: " + std::to_string(newVelocity.size())));

                if (abs(newVelocity.x * core->getTimeDelta()) > particleGrid->getDeltaX() || abs(newVelocity.y * core->getTimeDelta()) > particleGrid->getDeltaY())
                    throw(std::runtime_error("Long Jump Detected!"));

                // Updating particle location and velocity
                Vector3 oldLocation = particles[p].location;
                particles[p].location = (particles[p].location + newVelocity * core->getTimeDelta()) * Vector3::VectorMaskXY;

                // Checking if the particle has left it's cell       
                std::pair<GRID_INDEX, GRID_INDEX> newCell = particleGrid->getCell(particles[p].location);
                if (newCell.first != cell_i || newCell.second != cell_j)
                {
                    particles[p].transferFlag = true;
                }
            }
        }

    // Particle transfer loop
    for (size_t cell_i = 0; cell_i < particleGrid->getResolutionX() - 1; cell_i++)
        for (size_t cell_j = 0; cell_j < particleGrid->getResolutionY() - 1; cell_j++)
        {
            std::vector<Particle>& particles = particleGrid->editParticlesInCell(cell_i, cell_j);

            for (int p = particles.size() - 1; p >= 0; p--)
                if (particles[p].transferFlag)
                {
                    particles[p].transferFlag = false;

                    std::pair<GRID_INDEX, GRID_INDEX> newCell = particleGrid->getCell(particles[p].location);

                    particleGrid->particleCellTransfer(p, cell_i, cell_j, newCell.first, newCell.second);

                    if (abs((int)(cell_i - newCell.first)) > 1 || abs((int)(cell_j - newCell.second)) > 1)
                        throw(std::runtime_error("Long Jump Detected: " + std::to_string(cell_i) + ", " + std::to_string(cell_j) + " -> " + std::to_string(newCell.first) + ", " + std::to_string(newCell.second)));
                }
        }

    return ModuleExecutionStatus::Success;
}