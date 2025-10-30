#include <gtest.h>

#include "PicadorJrCore.h"
#include "Module_ParticleSolver.h"
#include "StaticField.h"
#include "Constants.h"

TEST(ParticleSolver, RelativisticAccelerationInStaticField)
{
    // Initializing static field

    double EZero = 10;

    FieldData staticFieldData;
    staticFieldData.E = Vector3(EZero, 0, 0);
    staticFieldData.B = Vector3::Zero;
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    // Initializing particle grid
    ParticleGrid particleGrid(2, 2, 400, 400, Vector3(-200, -200), 0);

    // Adding single particle to the grid
    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3(0, 0, 0), Vector3::Zero);
    particleGrid.editParticlesInCell(0, 0).push_back(testParticle);

    size_t NumInterations = 2000;
    double timeStep = abs(Constants::ElectronMass * Constants::SpeedOfLight 
        / (Constants::ElectronCharge * EZero * NumInterations));

    // Initializing core
    PicadorJrCore core(&staticField, &particleGrid, timeStep, NumInterations);

    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    core.run();

    // Checking results
    auto& particles = core.getParticleGrid()->getParticlesInCell(0, 0);

    ASSERT_EQ(1, particles.size());

    double finalX = (sqrt(2) - 1) * Constants::ElectronMass * Constants::SpeedOfLight * Constants::SpeedOfLight / (Constants::ElectronCharge * EZero);
    
    double finalImpulseX = Constants::ElectronMass * Constants::SpeedOfLight;

    EXPECT_NEAR(finalX, particles[0].location.x, 1e-1);
    EXPECT_NEAR(0, particles[0].location.y, 1e-1);
    EXPECT_NEAR(0, particles[0].location.z, 1e-1);

    EXPECT_NEAR(finalImpulseX, particles[0].impulse.x, 1e-25);
    EXPECT_NEAR(0, particles[0].impulse.y, 1e-25);
    EXPECT_NEAR(0, particles[0].impulse.z, 1e-25);
}


TEST(ParticleSolver, OscilationInStaticField)
{
    // Initializing static field

    double BZero = 100;

    double PZero = 1e-20;

    FieldData staticFieldData;
    staticFieldData.E = Vector3::Zero;
    staticFieldData.B = Vector3(0, 0, BZero);
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    // Initializing particle grid
    ParticleGrid particleGrid(2, 2, 400, 400, Vector3(-200, -200), 0);

    // Adding single particle to the grid
    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3::Zero, Vector3(PZero, 0, 0));
    particleGrid.editParticlesInCell(0, 0).push_back(testParticle);

    size_t NumInterations = 100;
    double timeStep = Constants::PI * Constants::ElectronMass * Constants::SpeedOfLight / (abs(Constants::ElectronCharge) * BZero * NumInterations) 
                        * sqrt(1 + pow(PZero / (Constants::ElectronMass * Constants::SpeedOfLight), 2));

    // Initializing core
    PicadorJrCore core(&staticField, &particleGrid, timeStep, NumInterations);

    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    core.run();

    // Checking results
    auto& particles = core.getParticleGrid()->getParticlesInCell(0, 0);

    ASSERT_EQ(1, particles.size());

    double finalY = -2 * PZero * Constants::SpeedOfLight / (Constants::ElectronCharge * BZero);
    
    double finalImpulseX = -PZero;

    EXPECT_NEAR(0, particles[0].location.x, 1e-1);
    EXPECT_NEAR(finalY, particles[0].location.y, 1e-1);
    EXPECT_NEAR(0, particles[0].location.z, 1e-1);

    EXPECT_NEAR(finalImpulseX, particles[0].impulse.x, 1e-25);
    EXPECT_NEAR(0, particles[0].impulse.y, 1e-25);
    EXPECT_NEAR(0, particles[0].impulse.z, 1e-25);
}