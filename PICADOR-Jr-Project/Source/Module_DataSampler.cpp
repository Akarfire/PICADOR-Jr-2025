#include "Module_DataSampler.h"

#include <fstream>
#include <iostream>

ModuleExecutionStatus DataSampler::onBegin()
{
    iterationCounter = sampleInterval; // So that first update samples data

    // Auto particle tracking id assignment
    ParticleGrid* particleGrid = core->getParticleGrid();

    unsigned short currentID = 1;
    for (GRID_INDEX i = 0; i < particleGrid->getResolutionX() - 1; i++)
        for (GRID_INDEX j = 0; j < particleGrid->getResolutionY() - 1; j++)
            for (Particle& particle : particleGrid->editParticlesInCell(i, j))
                if (particle.trackingID == 0 && autoParticleTrackingIDs)
                    particle.trackingID = currentID++;

    return ModuleExecutionStatus::Success;
}

ModuleExecutionStatus DataSampler::onUpdate()
{
    if (iterationCounter % sampleInterval == 0)
    {
        std::cout << "Iteration: " << core->getCurrentIteration() << std::endl;
        sampledData.iterations.push_back(core->getCurrentIteration());

        // Sampling particle data
        if (sampleParticleLocations || sampleParticleVelocities || sampleParticleCells)
        {
            ParticleGrid* particleGrid = core->getParticleGrid();

            if (sampleParticleLocations)
                sampledData.particleLocations.push_back(std::vector<std::pair<unsigned short, Vector3>>());
            if (sampleParticleVelocities)
                sampledData.particleVelocities.push_back(std::vector<std::pair<unsigned short, Vector3>>());
            if (sampleParticleCells)
                sampledData.particleCells.push_back(std::vector<std::pair<unsigned short, std::pair<GRID_INDEX, GRID_INDEX>>>());

            for (GRID_INDEX i = 0; i < particleGrid->getResolutionX() - 1; i++)
                for (GRID_INDEX j = 0; j < particleGrid->getResolutionY() - 1; j++)
                    for (const Particle& particle : particleGrid->getParticlesInCell(i, j))
                    {
                        if (sampleParticleLocations)
                            sampledData.particleLocations[sampledData.size].push_back({particle.trackingID, particle.location});
                        if (sampleParticleVelocities)
                            sampledData.particleVelocities[sampledData.size].push_back({particle.trackingID, particle.getVelocity()});
                        if (sampleParticleCells)
                            sampledData.particleCells[sampledData.size].push_back({particle.trackingID, { i, j }});
                    }
        }

        if (samplePartcileDensity)
        {
            ParticleGrid* particleGrid = core->getParticleGrid();
            
            sampledData.particleDensity.push_back({});
            
            for (size_t i = 0; i < fieldSamplingParameters.samplingResolutionX; i++)
                for (size_t j = 0; j < fieldSamplingParameters.samplingResolutionY; j++)
                {
                    Vector3 sampleLocation = fieldSamplingParameters.samplingOrigin + Vector3(  i * fieldSamplingParameters.samplingStepX,
                                                                                                j * fieldSamplingParameters.samplingStepY);
                    std::pair<GRID_INDEX, GRID_INDEX> cell = particleGrid->getCell(sampleLocation);
                    
                    double numParticles = 0;
                    for (const Particle& particle : particleGrid->getParticlesInCell(cell.first, cell.second))
                        numParticles += particle.weight;

                    sampledData.particleDensity[sampledData.size].push_back( {sampleLocation, numParticles} );
                }
        }

        // Sampling field data
        if (sampleFieldData)
        {
            FieldContainer* fieldContainer = core->getFieldContainer();

            sampledData.fieldData.push_back({});

            for (size_t i = 0; i < fieldSamplingParameters.samplingResolutionX; i++)
                for (size_t j = 0; j < fieldSamplingParameters.samplingResolutionY; j++)
                {
                    Vector3 sampleLocation = fieldSamplingParameters.samplingOrigin + Vector3(  i * fieldSamplingParameters.samplingStepX,
                                                                                                j * fieldSamplingParameters.samplingStepY);

                    sampledData.fieldData[sampledData.size].push_back( {sampleLocation, fieldContainer->getFieldsAt(sampleLocation)} );
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
        if (!additionalDataFlags.empty())
            for (auto& flag : additionalDataFlags)
                outFile << "#" << flag << "\n";

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
                for (const auto& entry : sampledData.particleLocations[i])
                    outFile << entry.first << " | Location: " << entry.second.x << ", " <<  entry.second.y << ", " <<  entry.second.z << "\n";
            }
            if (sampleParticleVelocities)
            {
                for (const auto& entry : sampledData.particleVelocities[i])
                    outFile << entry.first << " | Velocity: " << entry.second.x << ", " << entry.second.y << ", " << entry.second.z << "\n";
            }
            if (sampleParticleCells)
            {
                for (const auto& entry : sampledData.particleCells[i])
                    outFile << entry.first << " | Cell: " << entry.second.first << ", " << entry.second.second << "\n";
            }
            if (samplePartcileDensity)
            {
                outFile << "   Particle Density: " << std::endl;
                for (const auto& entry : sampledData.particleDensity[i])
                    outFile << entry.first.x << ", " << entry.first.y << " : " << entry.second << std::endl;
            }
            if (sampleFieldData)
            {
                outFile << "   Field Data: " << std::endl;
                for (const auto& entry : sampledData.fieldData[i])
                {
                    outFile << "E: " << entry.first.x << ", " << entry.first.y << " : " << entry.second.E.x << ", " << entry.second.E.y << ", " << entry.second.E.z << std::endl;
                    outFile << "B: " << entry.first.x << ", " << entry.first.y << " : " << entry.second.B.x << ", " << entry.second.B.y << ", " << entry.second.B.z << std::endl;
                    outFile << "J: " << entry.first.x << ", " << entry.first.y << " : " << entry.second.J.x << ", " << entry.second.J.y << ", " << entry.second.J.z << std::endl;
                }
            }

            outFile << "\n";
        }

        outFile.close();
    }

    return ModuleExecutionStatus::Success;
}