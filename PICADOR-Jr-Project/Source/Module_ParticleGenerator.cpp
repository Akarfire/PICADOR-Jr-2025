#include "Module_ParticleGenerator.h"


// Generates particles for a single cell and appends them to outParticles
void ParticleGenerator::generateParticlesForCell(std::vector<Particle>& outParticles)
{

}

// Generates initial velocity for a particle
Vector3 ParticleGenerator::generateParticleVelocity()
{
    return Vector3::Zero;
}

// Called by the core before entering the simulation loop
ModuleExecutionStatus ParticleGenerator::onBegin()
{
    ParticleGrid* particleGrid = core->getParticleGrid();

    // Generating particles for each cell
    for (GRID_INDEX i = 0; i < (GRID_INDEX)particleGrid->getResolutionY() - 1; i++)
        for (GRID_INDEX j = 0; j < (GRID_INDEX)particleGrid->getResolutionX() - 1; j++)
        {
            std::vector<Particle>& cellParticles = particleGrid->editParticlesInCell(i, j);

            generateParticlesForCell(cellParticles);
        }

    return ModuleExecutionStatus::Success;
}

