#pragma once

#include "Module.h"
#include "PicadorJrCore.h"

#include <functional>
#include <random>

class ParticleGenerator : public Module
{
public:

    struct ParticleGenerationProfile
    {
        // Sample particle to copy properties from, electron by default
        Particle sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);

        // Number of particles generated per cell ( f(cell location) = number of particles in this cell )
        std::function<size_t(Vector3)> particlePerCellDensityFunction = [](Vector3) { return 0; }; 

        // Mean velocity of generated particles ( f(location) = velocity mean )
        std::function<Vector3(Vector3)> velocityMeanFunction = [](Vector3) { return Vector3::Zero; }; 
        
        // Standard deviation of generated particle velocities ( f(location) = velocity standart deviation )
        std::function<Vector3(Vector3)> velocityStandartDeviationFunction = [](Vector3) { return Vector3::Zero; };
    };

protected:

    // Random seed
    int seed;

    // Random engine used for normal destribution
    std::mt19937 randomEngine;

    // List of generation profiles
    std::vector<ParticleGenerationProfile> generationProfiles;

    // Generates particles for a single cell and appends them to outParticles
    void generateParticlesForCell(std::vector<Particle>& outParticles, GRID_INDEX cell_i, GRID_INDEX cell_j);

    // Generates initial velocity for a particle
    Vector3 generateParticleVelocity();

public:
    ParticleGenerator(PicadorJrCore* core_, int seed_): Module(core_), seed(seed_), randomEngine(seed) { srand(seed); }

    // Called by the core before entering the simulation loop
    virtual ModuleExecutionStatus onBegin() override;

    // Called on every iteration of the simulation loop
    virtual ModuleExecutionStatus onUpdate() override { return ModuleExecutionStatus::Success; };

    // Called after the core has exited the simulation loop
    virtual ModuleExecutionStatus onEnd() override { return ModuleExecutionStatus::Success; }


    // INTERFACE

    // Specifies different generation profiles
    void addGenerationProfile(const ParticleGenerationProfile& profile) { generationProfiles.push_back(profile); }
    
};