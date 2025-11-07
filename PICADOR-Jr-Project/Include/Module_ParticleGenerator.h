#pragma once

#include "Module.h"
#include "PicadorJrCore.h"

class ParticleGenerator : public Module
{
public:

    struct ParticleGenerationProfile
    {
        // Sample particle to copy properties from, electron by default
        Particle sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);

        // Number of particles generated per cell
        double particleDensityPerCell = 10.0; 

        // Mean velocity of generated particles
        double velocityMean = 0.0; 
        
        // Standard deviation of generated particle velocities
        double velocityStandartDeviation = 0.0; 
    };

protected:

    // List of generation profiles
    std::vector<ParticleGenerationProfile> generationProfiles;

    // Generates particles for a single cell and appends them to outParticles
    void generateParticlesForCell(std::vector<Particle>& outParticles);

    // Generates initial velocity for a particle
    Vector3 generateParticleVelocity();

public:
    ParticleGenerator(PicadorJrCore* core_): Module(core_) {}

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