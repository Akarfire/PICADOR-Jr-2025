#include <gtest.h>

#include "PicadorJrCore.h"
#include "StaticField.h"
#include "ParticleGrid.h"
#include "Module_ParticleGenerator.h"


TEST(ParticleGenerator, validConstantDensityOnMainGrid)
{
    // Initializing core
    ParticleGrid particleGrid(4, 4, 1, 1, Vector3::Zero, 0);
    StaticField staticField = StaticField(FieldData());
    PicadorJrCore core(&staticField, &particleGrid, 1, 1);

    // Particle generator

    ParticleGenerator particleGenerator(&core, 1);

    ParticleGenerator::ParticleGenerationProfile profile;
    profile.sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);
    profile.particlePerCellDensityFunction = [](Vector3) { return 10; };

    core.insertModule(&particleGenerator);

    core.run();

    // Checking results
    ParticleGrid* particleGrid = core.getParticleGrid();

    for (GRID_INDEX i = 0; i < 3; i++)
        for (GRID_INDEX j = 0; j < 3; j++)
            EXPECT_EQ(10, particleGrid->getParticlesInCell(i, j).size());
}
