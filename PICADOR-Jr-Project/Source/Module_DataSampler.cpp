#include "Module_DataSampler.h"

#include <fstream>
#include <iostream>

ModuleExecutionStatus DataSampler::onBegin()
{
    iterationCounter = sampleInterval; // So that first update samples data

    return ModuleExecutionStatus::Success;
}

ModuleExecutionStatus DataSampler::onUpdate()
{
    if (iterationCounter % sampleInterval == 0)
    {
        sampledData.iterations.push_back(core->getCurrentIteration());

        if (sampleParticleLocations || sampleParticleVelocities || sampleParticleCells)
        {
            ParticleGrid* particleGrid = core->getParticleGrid();

            if (sampleParticleLocations)
                sampledData.particleLocations.push_back(std::vector<Vector3>());
            if (sampleParticleVelocities)
                sampledData.particleVelocities.push_back(std::vector<Vector3>());
            if (sampleParticleCells)
                sampledData.particleCells.push_back(std::vector<std::pair<GRID_INDEX, GRID_INDEX>>());


            for (GRID_INDEX i = 0; i < particleGrid->getResolutionY() - 1; i++)
                for (GRID_INDEX j = 0; j < particleGrid->getResolutionX() - 1; j++)
                    for (const Particle& particle : particleGrid->getParticlesInCell(i, j))
                    {
                        if (sampleParticleLocations)
                            sampledData.particleLocations[sampledData.size].push_back(particle.location);
                        if (sampleParticleVelocities)
                            sampledData.particleVelocities[sampledData.size].push_back(particle.getVelocity());
                        if (sampleParticleCells)
                            sampledData.particleCells[sampledData.size].push_back({ i, j });
                    }
        }

        sampledData.size++;
    }

    iterationCounter++;
    return ModuleExecutionStatus::Success;
}

ModuleExecutionStatus DataSampler::onEnd()
{
    // If output file name is specified, write sampled data to file
    if (!outputFileName.empty())
    {
        std::ofstream outFile(outputFileName, std::ios::out);

        if (!outFile.is_open())
            return ModuleExecutionStatus::Error;

        // Additional data

        if (writeParticleGridParameters)
        {
            ParticleGrid* particleGrid = core->getParticleGrid();
            outFile << "Particle Grid Parameters:\n";
            outFile << "Resolution X: " << particleGrid->getResolutionX() << "\n";
            outFile << "Resolution Y: " << particleGrid->getResolutionY() << "\n";
            outFile << "Delta X: " << particleGrid->getDeltaX() << "\n";
            outFile << "Delta Y: " << particleGrid->getDeltaY() << "\n";
            outFile << "Origin: " << particleGrid->getOrigin().x << ", " << particleGrid->getOrigin().y << ", " << particleGrid->getOrigin().z << "\n\n";
        }

        // Main sampling data
        for (size_t i = 0; i < sampledData.size; i++)
        {
            outFile << "Iteration: " << sampledData.iterations[i] << "\n";
            if (sampleParticleLocations)
            {
                for (const Vector3& loc : sampledData.particleLocations[i])
                    outFile << "Location: " << loc.x << ", " << loc.y << ", " << loc.z << "\n";
            }
            if (sampleParticleVelocities)
            {
                for (const Vector3& vel : sampledData.particleVelocities[i])
                    outFile << "Velocity: " << vel.x << ", " << vel.y << ", " << vel.z << "\n";
            }
            if (sampleParticleCells)
            {
                for (const auto& cell : sampledData.particleCells[i])
                    outFile << "Cell: " << cell.first << ", " << cell.second << "\n";
            }
            outFile << "\n";
        }

        outFile.close();
    }

    return ModuleExecutionStatus::Success;
}