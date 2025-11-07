#include "PicadorJrCore.h"

#include "StaticField.h"
#include "Module_ParticleSolver.h"
#include "Module_ParticleLoopEdgeCondition.h"
#include "Module_DataSampler.h"

#include "FieldGrid.h"
#include "ParticleGrid.h"

#include "Constants.h"

#include <iostream>


int main()
{
    size_t numInterations = 1100;
    double timeStep = 2e-12;
    
    // Initializing static field

    double BZero = 0;

    double PZero = Constants::ElectronMass * 3e8;

    FieldData staticFieldData;
    staticFieldData.E = Vector3::VectorMaskXY.normalized() * 100;
    staticFieldData.B = Vector3(0, 0, BZero);
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    // Initializing particle grid
    // Calculating space step

    // double spaceStep = Constants::SpeedOfLight * timeStep * 2;
    // spaceStep *= 1.0;

    double spaceStep = 11.9917;

    // Initializing particle grid
    ParticleGrid particleGrid(9, 9, spaceStep, spaceStep, Vector3(-4.5 * spaceStep, -4.5 * spaceStep), 1);

    // Adding single particle to the grid
    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3::Zero, Vector3(PZero, 0, 0));
    testParticle.trackingID = 1;
    particleGrid.editParticlesInCell(4, 4).push_back(testParticle);

    Particle testParticle_2(Constants::ElectronMass, -1 * Constants::ElectronCharge, Vector3(spaceStep, 0, 0), Vector3(PZero, 0, 0));
    testParticle_2.trackingID = 2;
    particleGrid.editParticlesInCell(4, 4).push_back(testParticle_2);

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
    dataSampler.outputFileName = "./tr.txt";

    core.insertModule(&dataSampler);

    try
    {
        core.run();
    }

    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    return 0;
}