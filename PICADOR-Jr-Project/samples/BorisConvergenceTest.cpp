#include <iostream>
#include <fstream>

#include "PicadorJrCore.h"

#include "StaticField.h"
#include "Module_ParticleSolver.h"

#include "FieldGrid.h"
#include "ParticleGrid.h"

#include "Constants.h"

int main()
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
    double timeStep = abs(Constants::ElectronMass * Constants::SpeedOfLight 
        / (Constants::ElectronCharge * EZero * NumInterations));

    // Initializing core
    PicadorJrCore core(&staticField, &particleGrid, timeStep, timeStep * 100);

    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    core.run();

    // Checking results
    auto& particles = core.getParticleGrid()->getParticlesInCell(0, 0);

    double finalX = (sqrt(2) - 1) * Constants::ElectronMass * Constants::SpeedOfLight * Constants::SpeedOfLight / (Constants::ElectronCharge * EZero);
    
    double finalImpulseX = Constants::ElectronMass * Constants::SpeedOfLight;
    double finalVelocityX = -1 * finalImpulseX / (Constants::ElectronMass * sqrt(1 + (finalImpulseX * finalImpulseX / ((Constants::ElectronMass * Constants::SpeedOfLight) 
                    * (Constants::ElectronMass * Constants::SpeedOfLight)))));;

    double xError = abs(finalX - particles[0].location.x);
    double vError = abs(finalVelocityX - particles[0].velocity.x);
    
    // Writing data into file
    std::ofstream outputFile;
    outputFile.open("./BorisConvergence.txt", std::ios::out);

    outputFile << NumInterations << std::endl;
    outputFile << xError << std::endl;
    outputFile << vError << std::endl;

    outputFile.close();

    return 0;
}