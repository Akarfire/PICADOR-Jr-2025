
#pragma once

#include "Module.h"
#include "PicadorJrCore.h"
#include "Vector3.h"
#include "Grid.h"

#include <vector>
#include <set>
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
        std::vector < float > fieldEnergy;
    };

    struct GridSamplingParameters
    {
        Vector3 samplingOrigin = Vector3::Zero;

        size_t samplingResolutionX = 1;
        size_t samplingResolutionY = 1;

        double samplingStepX = 0.1;
        double samplingStepY = 0.1;
    };

    struct FieldSamplingParameters
    {
        bool sampleE = true;
        bool sampleE_x = true;
        bool sampleE_y = true;
        bool sampleE_z = true;

        bool sampleB = true;
        bool sampleB_x = true;
        bool sampleB_y = true;
        bool sampleB_z = true;

        bool sampleJ = true;
        bool sampleJ_x = true;
        bool sampleJ_y = true;
        bool sampleJ_z = true;
    };

protected:

    // Internal iteration counter
    size_t iterationCounter = 0;

    // Internal storage for sampled data
    SampleData sampledData;

    // Writes sampled data into a file
    void writeDataToFile(std::string fileName);    

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
    
    bool sampleOnlySpecificIterations = false;
    std::set<size_t> specificIterations;

    // Particle data sampling options
    bool autoParticleTrackingIDs = true;

    bool sampleParticleLocations = true;
    bool sampleParticleVelocities = true;
    bool sampleParticleCells = false;

    bool samplePartcileDensity = false;
    GridSamplingParameters particleDensitySamplingParameters = GridSamplingParameters();

    // Example particle trace
    bool traceExampleParticle = false;
    int exampleParticlePickingSeed = 0;
    unsigned short int exampleTraceParticleTrackingID = 1;

    // Field data sampling options
    bool sampleFieldData = false;
    bool sampleFieldE = true;
    bool sampleFieldB = true;
    bool sampleFieldJ = true;

    bool sampleFieldEnergy = false;

    GridSamplingParameters fieldSamplingParameters = GridSamplingParameters();
    //FieldSamplingParameters

    // Additional data

    std::vector<std::string> additionalDataFlags;

    // Grid data sampling options
    bool writeParticleGridParameters = false;

    // Output file name for sampled data (no file output if empty)
    bool fileForEveryIteration = false;
    std::string outputFileName = "";
    std::string perIterationOutputFileFormat = ".txt";
};