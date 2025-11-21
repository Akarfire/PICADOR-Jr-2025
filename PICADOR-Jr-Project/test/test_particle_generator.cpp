#include <gtest.h>

#include "PicadorJrCore.h"
#include "StaticField.h"
#include "ParticleGrid.h"
#include "Module_ParticleGenerator.h"


TEST(ParticleGenerator, correctConstantDensityOnMainGridSingleProfile)
{
    // Initializing core
    ParticleGrid particleGrid(4, 4, 1, 1, Vector3::Zero, 0);
    StaticField staticField = StaticField(FieldData());
    PicadorJrCore core(&staticField, &particleGrid, 1, 1);

    // Particle generator

    ParticleGenerator particleGenerator(&core, 1);

    ParticleGenerator::ParticleGenerationProfile profile;
    profile.sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);
    profile.particleDensityFunction = [](Vector3) { return 10; };

    particleGenerator.addGenerationProfile(profile);
    core.insertModule(&particleGenerator);

    core.run();

    // Checking results
    for (GRID_INDEX i = 0; i < 3; i++)
        for (GRID_INDEX j = 0; j < 3; j++)
            EXPECT_EQ(10, particleGrid.getParticlesInCell(i, j).size());
}

TEST(ParticleGenerator, correctVariedDensityOnMainGridSingleProfile)
{
    // Initializing core
    ParticleGrid particleGrid(4, 4, 1, 1, Vector3::Zero, 0);
    StaticField staticField = StaticField(FieldData());
    PicadorJrCore core(&staticField, &particleGrid, 1, 1);

    // Particle generator

    ParticleGenerator particleGenerator(&core, 1);

    ParticleGenerator::ParticleGenerationProfile profile;
    profile.sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);
    profile.particleDensityFunction = [](Vector3 location) { return std::floor(location.x + location.y); };

    particleGenerator.addGenerationProfile(profile);
    core.insertModule(&particleGenerator);

    core.run();

    // Checking results
    for (GRID_INDEX i = 0; i < 3; i++)
        for (GRID_INDEX j = 0; j < 3; j++)
            EXPECT_EQ(std::floor(i + 0.5 + j + 0.5), particleGrid.getParticlesInCell(i, j).size());
}

TEST(ParticleGenerator, noParticlesGeneratedInThePadding)
{
    // Initializing core
    ParticleGrid particleGrid(4, 4, 1, 1, Vector3::Zero, 1);
    StaticField staticField = StaticField(FieldData());
    PicadorJrCore core(&staticField, &particleGrid, 1, 1);

    // Particle generator

    ParticleGenerator particleGenerator(&core, 1);

    ParticleGenerator::ParticleGenerationProfile profile;
    profile.sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);
    profile.particleDensityFunction = [](Vector3 location) { return std::floor(location.x + location.y); };

    particleGenerator.addGenerationProfile(profile);
    core.insertModule(&particleGenerator);

    core.run();

    // Checking results
    for (GRID_INDEX i = -1; i < 4; i++)
        for (GRID_INDEX j = -1; j < 4; j++)
            if (!(i >= 0 && i <= 2 && j >= 0 && j <= 2))
                EXPECT_EQ(0, particleGrid.getParticlesInCell(i, j).size());
}

TEST(ParticleGenerator, constantParticleFactorHandledCorrectly)
{
    // Initializing core
    ParticleGrid particleGrid(4, 4, 1, 1, Vector3::Zero, 0);
    StaticField staticField = StaticField(FieldData());
    PicadorJrCore core(&staticField, &particleGrid, 1, 1);

    // Particle generator

    ParticleGenerator particleGenerator(&core, 1);

    ParticleGenerator::ParticleGenerationProfile profile;
    profile.sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);
    profile.particleDensityFunction = [](Vector3 location) { return std::floor(location.x + location.y); };
    profile.particleFactorFunction = [](Vector3) { return 2; };

    particleGenerator.addGenerationProfile(profile);
    core.insertModule(&particleGenerator);

    core.run();

    // Checking results
    for (GRID_INDEX i = 0; i < 3; i++)
        for (GRID_INDEX j = 0; j < 3; j++)
            EXPECT_EQ(static_cast<int>(std::floor(i + 0.5 + j + 0.5)) / 2, particleGrid.getParticlesInCell(i, j).size());
}

TEST(ParticleGenerator, variedParticleFactorHandledCorrectly)
{
    // Initializing core
    ParticleGrid particleGrid(4, 4, 1, 1, Vector3::Zero, 0);
    StaticField staticField = StaticField(FieldData());
    PicadorJrCore core(&staticField, &particleGrid, 1, 1);

    // Particle generator

    ParticleGenerator particleGenerator(&core, 1);

    ParticleGenerator::ParticleGenerationProfile profile;
    profile.sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);
    profile.particleDensityFunction = [](Vector3 location) { return std::floor(location.x + location.y); };
    profile.particleFactorFunction = [](Vector3 location) { return std::max(static_cast<int>(std::floor((location.x + location.y) * 0.5)), 1); };

    particleGenerator.addGenerationProfile(profile);
    core.insertModule(&particleGenerator);

    core.run();

    // Checking results
    for (GRID_INDEX i = 0; i < 3; i++)
        for (GRID_INDEX j = 0; j < 3; j++)
            EXPECT_EQ(  static_cast<int>(std::floor(i + j + 1)) 
                        / std::max( static_cast<int>( std::floor((i + j + 1) * 0.5) ), 1), 
                        particleGrid.getParticlesInCell(i, j).size());
}

TEST(ParticleGenerator, lowTemperatureVelocityIsNearZero)
{
    // Initializing core
    ParticleGrid particleGrid(4, 4, 1, 1, Vector3::Zero, 0);
    StaticField staticField = StaticField(FieldData());
    PicadorJrCore core(&staticField, &particleGrid, 1, 1);

    // Particle generator

    ParticleGenerator particleGenerator(&core, 1);

    ParticleGenerator::ParticleGenerationProfile profile;
    profile.sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);
    profile.particleDensityFunction = [](Vector3) { return 10; };
    profile.temperatureFunction = [](Vector3) { return 1e-40; };

    particleGenerator.addGenerationProfile(profile);
    core.insertModule(&particleGenerator);

    core.run();

    // Checking results
    for (GRID_INDEX i = 0; i < 3; i++)
        for (GRID_INDEX j = 0; j < 3; j++)
            for (auto& particle : particleGrid.getParticlesInCell(i, j))
                EXPECT_NEAR(0.0, particle.getVelocity().size(), 0.001);
}