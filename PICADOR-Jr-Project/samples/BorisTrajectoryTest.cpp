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
    // SIMULATION PARAMETERS

    size_t numInterations = 1100;
    double timeStep = 2e-12;

    double spaceStep = 11.9917;
    // spaceStep = Constants::SpeedOfLight * timeStep * 2;

    Vector3 E = Vector3::VectorMaskXY.normalized() * 100;
    Vector3 B = Vector3(0, 0, 100);
    
    std::vector<Particle> particles = 
    {
        Particle(Constants::ElectronMass,   Constants::ElectronCharge,        Vector3::Zero,                Vector3(Constants::ElectronMass * 3e8, 0, 0)),
        Particle(Constants::ElectronMass,   -1 * Constants::ElectronCharge,   Vector3(spaceStep, 0, 0),     Vector3(Constants::ElectronMass * 3e8, 0, 0))
    };

    std::string outputFileName = "./tr.txt";
    //"D:/Projects/PICADOR-Jr-2025/Visualization/Trajectory/data/tr.txt";
    

    // SIMULATION SETUP

    // Initializing static field

    FieldData staticFieldData;
    staticFieldData.E = E;
    staticFieldData.B = B;
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    // Initializing particle grid

    // Initializing particle grid
    ParticleGrid particleGrid(9, 9, spaceStep, spaceStep, Vector3(-4.5 * spaceStep, -4.5 * spaceStep), 1);

    // Adding particles to the grid
    for (size_t i = 0 ; i < particles.size(); i++)
    {
        particles[i].trackingID = (unsigned short int)i;
        std::pair<GRID_INDEX, GRID_INDEX> cell = particleGrid.getCell(particles[i].location);
        particleGrid.editParticlesInCell(cell.first, cell.second).push_back(particles[i]);
    }

    // Initializing core
    PicadorJrCore core(&staticField, &particleGrid, timeStep, numInterations);

    // Adding modules

    // Particle Solver module
    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    // Particle Loop Edge Condition module
    ParticleLoopEdgeCondition loopCondition(&core);
    core.insertModule(&loopCondition);

    // Data Sampling module
    DataSampler dataSampler(&core);

    dataSampler.sampleInterval = 1;
    dataSampler.sampleParticleLocations = true;
    dataSampler.sampleParticleVelocities = false;
    dataSampler.sampleParticleCells = false;
    dataSampler.writeParticleGridParameters = true;
    dataSampler.outputFileName = outputFileName;

    core.insertModule(&dataSampler);

    // RUN SIMULATION
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