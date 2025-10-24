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
    std::vector<double> deltaTimes;
    std::vector<double> locationErrors;
    std::vector<double> velocityErrors;


    // Initializing static field

    double EZero = 1;

    FieldData staticFieldData;
    staticFieldData.E = Vector3(EZero, 0, 0);
    staticFieldData.B = Vector3::Zero;
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    for (int d = 1; d < 100; d++)
    {    
        // Initializing particle grid
        ParticleGrid particleGrid(2, 2, 2000, 2000, Vector3(-1000, -1000), 0);

        // Adding single particle to the grid
        Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3(0, 0, 0), Vector3::Zero);
        particleGrid.editParticlesInCell(0, 0).push_back(testParticle);

        size_t NumInterations = 100 * d;
        double timeStep = abs(Constants::ElectronMass * Constants::SpeedOfLight 
            / (Constants::ElectronCharge * EZero * NumInterations));

        // Initializing core
        PicadorJrCore core(&staticField, &particleGrid, timeStep, NumInterations);

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

        deltaTimes.push_back(timeStep);
        locationErrors.push_back(xError);
        velocityErrors.push_back(vError);
    }
    
    // Writing data into file
    std::ofstream outputFile;
    outputFile.open("./BorisConvergence.txt", std::ios::out);
    
    for (size_t i = 0; i < deltaTimes.size(); i++)
        outputFile << deltaTimes[i] << ", " << locationErrors[i] << ", " << velocityErrors[i] << std::endl;

    outputFile.close();

    return 0;
}