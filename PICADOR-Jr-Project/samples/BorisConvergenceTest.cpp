#include <iostream>
#include <fstream>
#include <string>

#include "PicadorJrCore.h"

#include "StaticField.h"
#include "Module_ParticleSolver.h"

#include "FieldGrid.h"
#include "ParticleGrid.h"

#include "Constants.h"


// Tests
void AccelerationTestX(std::vector<double>& outDeltaTimes, std::vector<double>& outLocationErrors, std::vector<double>& outVelocityErrors, size_t iterations);
void AccelerationTestY(std::vector<double>& outDeltaTimes, std::vector<double>& outLocationErrors, std::vector<double>& outVelocityErrors, size_t iterations);
void OscilationTest(std::vector<double>& outDeltaTimes, std::vector<double>& outLocationErrors, std::vector<double>& outVelocityErrors, size_t iterations);

int main()
{
    std::vector<double> deltaTimes;
    std::vector<double> locationErrors;
    std::vector<double> velocityErrors;

    std::cout << "Running" << std::endl;

    size_t maxIterations = 10000;
    size_t step = 20;
    size_t scale = 2;
    std::string fileName = "BorisO.txt";

    for (size_t d = 100; d < maxIterations; d += step)
    {  
        //AccelerationTestX(deltaTimes, locationErrors, velocityErrors, d * scale);
        //AccelerationTestY(deltaTimes, locationErrors, velocityErrors, d * scale);
        OscilationTest(deltaTimes, locationErrors, velocityErrors, d * scale);
        std::cout << "Progress: " << pow((double)d / maxIterations, 2) * 100 << "%" << std::endl;
    }

    std::cout << "Writing data" << std::endl;

    // Writing data into file
    std::ofstream outputFile;
    outputFile.open("./" + fileName, std::ios::out);
    
    outputFile << "#DeltaTime, Location Error, Velocity Error" << std::endl << "/InverseX" << std::endl;

    for (size_t i = 0; i < deltaTimes.size(); i++)
        outputFile << deltaTimes[i] << ", " << locationErrors[i] << ", " << velocityErrors[i] << std::endl;

    outputFile.close();

    std::cout << "Complete" << std::endl;

    return 0;
}


void AccelerationTestX(std::vector<double>& outDeltaTimes, std::vector<double>& outLocationErrors, std::vector<double>& outVelocityErrors, size_t iterations)
{
    // Initializing static field

    double EZero = 1;

    FieldData staticFieldData;
    staticFieldData.E = Vector3(EZero, 0, 0);
    staticFieldData.B = Vector3::Zero;
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);
    
    // Initializing particle grid
    ParticleGrid particleGrid(2, 2, 2000, 2000, Vector3(-1000, -1000), 0);

    // Adding single particle to the grid
    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3(0, 0, 0), Vector3::Zero);
    particleGrid.editParticlesInCell(0, 0).push_back(testParticle);

    size_t numInterations = iterations;
    double timeStep = abs(Constants::ElectronMass * Constants::SpeedOfLight 
        / (Constants::ElectronCharge * EZero * numInterations));

    // Initializing core
    PicadorJrCore core(&staticField, &particleGrid, timeStep, numInterations);

    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    core.run();

    // Checking results
    auto& particles = core.getParticleGrid()->getParticlesInCell(0, 0);

    double finalX = (sqrt(2) - 1) * Constants::ElectronMass * Constants::SpeedOfLight * Constants::SpeedOfLight / (Constants::ElectronCharge * EZero);
    
    double finalImpulseX = Constants::ElectronMass * Constants::SpeedOfLight;
    double finalVelocityX = -1 * finalImpulseX / (Constants::ElectronMass * sqrt(1 + (finalImpulseX * finalImpulseX / ((Constants::ElectronMass * Constants::SpeedOfLight) 
                    * (Constants::ElectronMass * Constants::SpeedOfLight)))));;

    outDeltaTimes.push_back(timeStep);
    outLocationErrors.push_back((Vector3(finalX, 0, 0) - particles[0].location).size());
    outVelocityErrors.push_back((Vector3(finalVelocityX, 0, 0) - particles[0].getVelocity()).size());
}

void AccelerationTestY(std::vector<double>& outDeltaTimes, std::vector<double>& outLocationErrors, std::vector<double>& outVelocityErrors, size_t iterations)
{
    // Initializing static field

    double EZero = 1;

    FieldData staticFieldData;
    staticFieldData.E = Vector3(0, EZero, 0);
    staticFieldData.B = Vector3::Zero;
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);
    
    // Initializing particle grid
    ParticleGrid particleGrid(2, 2, 2000, 2000, Vector3(-1000, -1000), 0);

    // Adding single particle to the grid
    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3(0, 0, 0), Vector3::Zero);
    particleGrid.editParticlesInCell(0, 0).push_back(testParticle);

    size_t numInterations = iterations;
    double timeStep = abs(Constants::ElectronMass * Constants::SpeedOfLight 
        / (Constants::ElectronCharge * EZero * numInterations));

    // Initializing core
    PicadorJrCore core(&staticField, &particleGrid, timeStep, numInterations);

    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    core.run();

    // Checking results
    auto& particles = core.getParticleGrid()->getParticlesInCell(0, 0);

    double finalX = (sqrt(2) - 1) * Constants::ElectronMass * Constants::SpeedOfLight * Constants::SpeedOfLight / (Constants::ElectronCharge * EZero);
    
    double finalImpulseY = Constants::ElectronMass * Constants::SpeedOfLight;
    double finalVelocityY = -1 * finalImpulseY / (Constants::ElectronMass * sqrt(1 + (finalImpulseY * finalImpulseY / ((Constants::ElectronMass * Constants::SpeedOfLight) 
                    * (Constants::ElectronMass * Constants::SpeedOfLight)))));;

    outDeltaTimes.push_back(timeStep);
    outLocationErrors.push_back((Vector3(0, finalX, 0) - particles[0].location).size());
    outVelocityErrors.push_back((Vector3(0, finalVelocityY, 0) - particles[0].getVelocity()).size());
}


void OscilationTest(std::vector<double>& outDeltaTimes, std::vector<double>& outLocationErrors, std::vector<double>& outVelocityErrors, size_t iterations)
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

    size_t numInterations = iterations;
    double timeStep = Constants::PI * Constants::ElectronMass * Constants::SpeedOfLight / (abs(Constants::ElectronCharge) * BZero * numInterations) 
                        * sqrt(1 + pow(PZero / (Constants::ElectronMass * Constants::SpeedOfLight), 2));

    // Initializing core
    PicadorJrCore core(&staticField, &particleGrid, timeStep, numInterations);

    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    core.run();

    // Checking results
    auto& particles = core.getParticleGrid()->getParticlesInCell(0, 0);

    double finalY = -2 * PZero * Constants::SpeedOfLight / (Constants::ElectronCharge * BZero);
    
    double finalImpulseX = -PZero;
    double finalVelocityX = finalImpulseX / (Constants::ElectronMass * sqrt(1 + (finalImpulseX * finalImpulseX / ((Constants::ElectronMass * Constants::SpeedOfLight) 
                    * (Constants::ElectronMass * Constants::SpeedOfLight)))));;

    outDeltaTimes.push_back(timeStep);
    outLocationErrors.push_back((Vector3(0, finalY, 0) - particles[0].location).size());
    outVelocityErrors.push_back((Vector3(finalVelocityX, 0, 0) - particles[0].getVelocity()).size());
}