#include "Constants.h"

#include "PicadorJrCore.h"
#include "StaticField.h"
#include "ParticleGrid.h"
#include "Module_ParticleSolver.h"
#include "Module_ParticleLoopEdgeCondition.h"

#include <gtest.h>


TEST(ParticleSolver, RelativisticAccelerationInStaticFieldWithLooping)
{
    // Initializing static field

    double EZero = 10;

    FieldData staticFieldData;
    staticFieldData.E = Vector3(EZero, 0, 0);
    staticFieldData.B = Vector3::Zero;
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    // Calculating time step
    size_t NumInterations = 2000;
    double timeStep = abs(Constants::ElectronMass * Constants::SpeedOfLight 
        / (Constants::ElectronCharge * EZero * NumInterations));

    // Calculating space step
    double spaceStep = 2 * timeStep / (Constants::SpeedOfLight - 1e4);

    // Initializing particle grid
    ParticleGrid particleGrid(4, 4, spaceStep, spaceStep, Vector3(-0.5, -0.5), 1);

    // Adding single particle to the grid
    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3(0, 0, 0), Vector3::Zero);
    particleGrid.editParticlesInCell(0, 0).push_back(testParticle);

    // Initializing core
    PicadorJrCore core(&staticField, &particleGrid, timeStep, NumInterations);

    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    ParticleLoopEdgeCondition loopCondition(&core);
    core.insertModule(&loopCondition);

    core.run();

    // Checking results
    auto& particles = core.getParticleGrid()->getParticlesInCell(0, 0);

    ASSERT_EQ(1, particles.size());

    double finalX = (sqrt(2) - 1) * Constants::ElectronMass * Constants::SpeedOfLight * Constants::SpeedOfLight / (Constants::ElectronCharge * EZero);
    
    double finalImpulseX = -1 * Constants::ElectronMass * Constants::SpeedOfLight;

    EXPECT_NEAR(finalX, particles[0].location.x, 1e-1);
    EXPECT_NEAR(0, particles[0].location.y, 1e-1);
    EXPECT_NEAR(0, particles[0].location.z, 1e-1);

    EXPECT_NEAR(finalImpulseX, particles[0].impulse.x, 1e-25);
    EXPECT_NEAR(0, particles[0].impulse.y, 1e-25);
    EXPECT_NEAR(0, particles[0].impulse.z, 1e-25);
}
