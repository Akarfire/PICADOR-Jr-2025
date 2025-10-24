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
    ParticleGrid particleGrid(2, 2, 200, 200, Vector3(-100, -100), 0);

    // Adding single particle to the grid
    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3(0, 0, 0), Vector3::Zero);
    particleGrid.editParticlesInCell(0, 0).push_back(testParticle);

    size_t NumInterations = 100;
    double timeStep = abs(Constants::ElectronMass * Constants::SpeedOfLight / (Constants::ElectronCharge * EZero * NumInterations));

    // Initializing core
    PicadorJrCore core(&staticField, &particleGrid, timeStep, timeStep * 100);

    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    core.run();

    // Checking results
    auto& particles = core.getParticleGrid()->getParticlesInCell(0, 0);

    ASSERT_EQ(1, particles.size());

    double finalX = (sqrt(2) - 1) * Constants::ElectronMass * Constants::SpeedOfLight * Constants::SpeedOfLight / (Constants::ElectronCharge * EZero);
    
    double finalImpulseX = Constants::ElectronMass * Constants::SpeedOfLight;
    double finalVelocityX = finalImpulseX / (Constants::ElectronMass * sqrt(1 + (finalImpulseX * finalImpulseX / ((Constants::ElectronMass * Constants::SpeedOfLight) 
                    * (Constants::ElectronMass * Constants::SpeedOfLight)))));;

    EXPECT_DOUBLE_EQ(finalX, particles[0].location.x);
    EXPECT_DOUBLE_EQ(0, particles[0].location.y);

    EXPECT_DOUBLE_EQ(finalVelocityX, particles[0].velocity.x);
    EXPECT_DOUBLE_EQ(0, particles[0].velocity.y);
    EXPECT_DOUBLE_EQ(0, particles[0].velocity.z);
}