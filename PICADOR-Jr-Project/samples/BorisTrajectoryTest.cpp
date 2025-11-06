#include "PicadorJrCore.h"

#include "StaticField.h"
#include "Module_ParticleSolver.h"
#include "Module_ParticleLoopEdgeCondition.h"
#include "Module_DataSampler.h"

#include "FieldGrid.h"
#include "ParticleGrid.h"

#include "Constants.h"


int main()
{
    size_t numInterations = 1500;
    double timeStep = 2e-10;


    // Initializing static field

    double BZero = 10;

    double PZero = 10e-19;

    FieldData staticFieldData;
    staticFieldData.E = Vector3::One * 1e-1;
    staticFieldData.B = Vector3(0, 0, BZero);
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    // Initializing particle grid
    // Calculating space step
    double spaceStep = Constants::SpeedOfLight * timeStep * 2.0;
    spaceStep *= 1.0;

    // Initializing particle grid
    ParticleGrid particleGrid(9, 9, spaceStep, spaceStep, Vector3(-4.5 * spaceStep, -4.5 * spaceStep), 1);

    // Adding single particle to the grid
    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3::Zero, Vector3(PZero, 0, 0));
    testParticle.trackingID = 1;
    particleGrid.editParticlesInCell(0, 0).push_back(testParticle);

    Particle testParticle_2(Constants::ElectronMass, -1 * Constants::ElectronCharge, Vector3::One * 20, Vector3(PZero, 0, 0));
    testParticle_2.trackingID = 2;
    particleGrid.editParticlesInCell(0, 0).push_back(testParticle_2);

    // Initializing core
    PicadorJrCore core(&staticField, &particleGrid, timeStep, numInterations);

    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    ParticleLoopEdgeCondition loopCondition(&core);
    core.insertModule(&loopCondition);

    // Data Sampling module
    DataSampler dataSampler(&core);

    dataSampler.sampleInterval = 1;
    dataSampler.sampleParticleLocations = true;
    dataSampler.sampleParticleVelocities = false;
    dataSampler.sampleParticleCells = false;
    dataSampler.writeParticleGridParameters = true;
    dataSampler.outputFileName = "./2_Particles.txt";

    core.insertModule(&dataSampler);

    core.run();

    return 0;
}