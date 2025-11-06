
#pragma once

#include "Module.h"
#include "PicadorJrCore.h"
#include "Vector3.h"
#include "Grid.h"

#include <vector>
#include <string>

class DataSampler : public Module
{
public:

    // Internal structure storing sampled data for each iteration
    struct SampleData
    {
        size_t size = 0;

        std::vector<size_t> iterations;

        // Particle data
        std::vector<std::vector<Vector3>> particleLocations;
        std::vector<std::vector<Vector3>> particleVelocities;
        std::vector<std::vector<std::pair<GRID_INDEX, GRID_INDEX>>> particleCells;
    };

protected:

    // Internal iteration counter
    size_t iterationCounter = 0;

    // Internal storage for sampled data
    SampleData sampledData;

public:
    DataSampler(PicadorJrCore* core_): Module(core_) {}

    // Called by the core before entering the simulation loop
    virtual ModuleExecutionStatus onBegin() override;

    // Called on every iteration of the simulation loop
    virtual ModuleExecutionStatus onUpdate() override;

    // Called after the core has exited the simulation loop
    virtual ModuleExecutionStatus onEnd() override;


    // Accessor for sampled data
    const SampleData& getSampledData() const { return sampledData; }

    // PARAMETERS
    
    // How often to sample data (in iterations)
    size_t sampleInterval = 1;

    // Particle data sampling options
    bool sampleParticleLocations = true;
    bool sampleParticleVelocities = true;
    bool sampleParticleCells = false;

    
    // Additional data

    // Grid data sampling options
    bool writeParticleGridParameters = false;

    // Output file name for sampled data (no file output if empty)
    std::string outputFileName = "";
};