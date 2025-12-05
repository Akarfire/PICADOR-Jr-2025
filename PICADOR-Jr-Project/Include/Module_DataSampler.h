
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
        std::vector<std::vector<std::pair<unsigned short, Vector3>>> particleLocations;
        std::vector<std::vector<std::pair<unsigned short, Vector3>>> particleVelocities;
        std::vector<std::vector<std::pair<unsigned short, std::pair<GRID_INDEX, GRID_INDEX>>>> particleCells;
        std::vector< std::vector<std::pair<Vector3, double>> > particleDensity;

        // Field data
        std::vector< std::vector<std::pair<Vector3, FieldData>> > fieldData;
    };

    struct GridSamplingParameters
    {
        Vector3 samplingOrigin = Vector3::Zero;

        size_t samplingResolutionX = 1;
        size_t samplingResolutionY = 1;

        double samplingStepX = 0.1;
        double samplingStepY = 0.1;
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
    bool autoParticleTrackingIDs = true;

    bool sampleParticleLocations = true;
    bool sampleParticleVelocities = true;
    bool sampleParticleCells = false;

    bool samplePartcileDensity = false;
    GridSamplingParameters particleDensitySamplingParameters = GridSamplingParameters();

    // Field data sampling options
    bool sampleFieldData = false;
    GridSamplingParameters fieldSamplingParameters = GridSamplingParameters();


    // Additional data

    std::vector<std::string> additionalDataFlags;

    // Grid data sampling options
    bool writeParticleGridParameters = false;

    // Output file name for sampled data (no file output if empty)
    std::string outputFileName = "";
};