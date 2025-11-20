#include "Module_ParticleGenerator.h"
#include <random>

// Generates particles for a single cell and appends them to outParticles
void ParticleGenerator::generateParticlesForCell(std::vector<Particle>& outParticles, GRID_INDEX cell_i, GRID_INDEX cell_j)
{
    ParticleGrid* particleGrid = core->getParticleGrid();

    Vector3 cellLocation = Vector3( cell_j * particleGrid->getDeltaX() + particleGrid->getOrigin().x,
                                    cell_i * particleGrid->getDeltaY() + particleGrid->getOrigin().y );

    // Genering particles from all profiles
    for (auto& profile : generationProfiles)
    {
        Vector3 cellSampleLocation = cellLocation + Vector3(particleGrid->getDeltaX(),  particleGrid->getDeltaY()) * 0.5;

        // Calculating particle number for this cell
        size_t numParticles =   profile.particleDensityFunction(cellSampleLocation)
                                * particleGrid->getDeltaX()
                                * particleGrid->getDeltaY() 
                                / profile.particleFactorFunction(cellSampleLocation);

        // Generating individual particles
        for (size_t i = 0; i < numParticles; i++)
        {
            Vector3 particleLocation = cellLocation + Vector3(    particleGrid->getDeltaX() * ((double)(rand()) / RAND_MAX),
                                                                particleGrid->getDeltaY() * ((double)(rand()) / RAND_MAX));
                                                             
            double speedMean = 0.0;
            double speedStandartDeviation = sqrt(profile.temperatureFunction(particleLocation) / profile.sampleParticle.mass);

            std::normal_distribution<> velocityDistributionX(speedMean, speedStandartDeviation);
            std::normal_distribution<> velocityDistributionY(speedMean, speedStandartDeviation);

            Vector3 particleVelocity = Vector3(velocityDistributionX(randomEngine), velocityDistributionY(randomEngine), 0);

            // Creating particle
            Particle newParticle = profile.sampleParticle;
            newParticle.location = particleLocation;
            newParticle.impulse = Particle::convertVelocityToImpulse(particleVelocity, newParticle.mass);
            newParticle.weight = profile.particleFactorFunction(particleLocation);

            // Adding particle to the grid
            outParticles.push_back(newParticle);
        }
    }
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

            generateParticlesForCell(cellParticles, i, j);
        }

    return ModuleExecutionStatus::Success;
}

