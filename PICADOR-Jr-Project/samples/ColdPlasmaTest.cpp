#include "PicadorJrCore.h"

#include "FieldGrid.h"
#include "Module_FieldSolver.h"
#include "Module_FieldGenerator.h"
#include "Module_ParticleSolver.h"
#include "Module_ParticleLoopEdgeCondition.h"
#include "Module_ParticleGenerator.h"
#include "Module_DataSampler.h"
#include "Module_CurrentDepositor.h"

#include "FieldGrid.h"
#include "ParticleGrid.h"

#include "Constants.h"

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <shellapi.h>
#include <string>


// Parameters from experiment description (GLOBAL)
// {

size_t MatrixSize = 64;

double L = 1.0;
double NumPerL_Debay = 0.5;
size_t NumPerPlasmaPeriod = 256;
size_t NumPerCell = 30;
size_t NumPeriods = MatrixSize / (2 * sqrt(2.0) * Constants::PI * NumPerL_Debay);

double SpaceStep = L / MatrixSize;
double L_Debay = SpaceStep * NumPerL_Debay;

double Temp = 1e-2 * Constants::ElectronMass * Constants::SpeedOfLight * Constants::SpeedOfLight;
double Density = Temp / (8 * Constants::PI * Constants::ElectronCharge * L_Debay * Constants::ElectronCharge * L_Debay);
double w_p = sqrt(4.0 * Constants::PI * Constants::ElectronCharge * Constants::ElectronCharge * Density / Constants::ElectronMass);

size_t IterationsBetweenDumps = NumPerPlasmaPeriod / 16;
size_t ParticleFactor = Density * SpaceStep * SpaceStep * SpaceStep / NumPerCell;

double A = 0.05;
double Amp = 2.0 * L * Density * Constants::ElectronCharge * A;
// }

int main()
{
    // SIMULATION PARAMETERS

    size_t numInterations = NumPeriods * NumPerPlasmaPeriod;
    double timeStep = 2.0 * (Constants::PI / w_p) / NumPerPlasmaPeriod;

    Vector3 fieldGridOrigin = Vector3::Zero;
    size_t fieldGridResolutionX = MatrixSize;
    size_t fieldGridResolutionY = 8;
    double fieldGridSpaceStepX = SpaceStep;
    double fieldGridSpaceStepY = SpaceStep;
    size_t fieldGridPadding = 1;

    Vector3 particleGridOrigin = fieldGridOrigin;
    double particleGridSpaceStepX = SpaceStep;
    double particleGridSpaceStepY = SpaceStep;
    size_t particleGridResolutionX = MatrixSize;//fieldGridResolutionX * fieldGridSpaceStepX / particleGridSpaceStepX + 1;
    size_t particleGridResolutionY = 8;//fieldGridResolutionY * fieldGridSpaceStepY / particleGridSpaceStepY + 1;
    size_t particleGridPadding = 1;

    std::cout << particleGridResolutionX << " : " << particleGridResolutionY << "\n";
    std::cout << particleGridSpaceStepX << " : " << particleGridSpaceStepY << "\n";

    // Field generation
    std::function<Vector3(Vector3)> E_Function = [](Vector3 location) { return Vector3(-Amp * cos(2.0 * Constants::PI * location.x / L), 0.0, 0.0); };

    // Particle generation
    ParticleGenerator::ParticleGenerationProfile profile;

    profile.sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);
    profile.particleDensityFunction = [](Vector3 location ) { return Density * (1.0 + A * sin(2.0 * Constants::PI * location.x / L)); };
    profile.temperatureFunction = [](Vector3) { return Temp; };
    profile.particleFactorFunction = [](Vector3) { return ParticleFactor; };

    // Initializing field grid
    FieldGrid fieldGrid(fieldGridResolutionX, fieldGridResolutionY, fieldGridSpaceStepX, fieldGridResolutionY, fieldGridOrigin, fieldGridPadding);

    // Initializing particle grid
    ParticleGrid particleGrid(particleGridResolutionX, particleGridResolutionY, particleGridSpaceStepX, particleGridSpaceStepY, particleGridOrigin, particleGridPadding);
    
    // Output file for automated trajectory visualization
    //std::string outputFileName = "ColdPlasmaTest.txt";//"../../Visualization/Trajectory/Automated/particle_trajectories_auto_vis.txt";
    std::string outputFileName = "../../Visualization/Graphs/Automated/Files/iteration_data";
    

    // SIMULATION SETUP

    // Initializing core
    PicadorJrCore core(&fieldGrid, &particleGrid, timeStep, numInterations);

    // Adding modules

    // Field generator
    FieldGenerator fieldGenerator(&core, E_Function);
    core.insertModule(&fieldGenerator);

    // Particle generator
    ParticleGenerator particleGenerator(&core, 1);
    particleGenerator.addGenerationProfile(profile);
    core.insertModule(&particleGenerator);

    // Field Solver
    FieldSolver fieldSolver(&core);
    core.insertModule(&fieldSolver);

    // Particle Solver
    ParticleSolver particleSolver(&core);
    core.insertModule(&particleSolver);

    // Particle Loop Edge Condition
    ParticleLoopEdgeCondition loopCondition(&core);
    core.insertModule(&loopCondition);

    // Current Depositor
    CurrentDepositor currentDepositor(&core);
    core.insertModule(&currentDepositor);

    // Data Sampling module
    DataSampler dataSampler(&core);

    //dataSampler.additionalDataFlags.push_back("OnlyInitialDistribution");
    dataSampler.sampleInterval = IterationsBetweenDumps;
    dataSampler.sampleOnlySpecificIterations = false;
    dataSampler.specificIterations = {1};
    dataSampler.sampleParticleLocations = false;
    dataSampler.sampleParticleVelocities = false;
    dataSampler.sampleParticleCells = false;
    dataSampler.writeParticleGridParameters = false;

    // Sampling particle density
    dataSampler.samplePartcileDensity = false;
    dataSampler.particleDensitySamplingParameters.samplingOrigin = Vector3::Zero;
    dataSampler.particleDensitySamplingParameters.samplingResolutionX = 100;
    dataSampler.particleDensitySamplingParameters.samplingStepX = 1.0 / dataSampler.particleDensitySamplingParameters.samplingResolutionX;
    dataSampler.particleDensitySamplingParameters.samplingResolutionY = 1;
    dataSampler.particleDensitySamplingParameters.samplingStepY = 0.0;

    // Sampling field data
    dataSampler.sampleFieldData = true;
    dataSampler.fieldSamplingParameters.samplingOrigin = Vector3::Zero;
    dataSampler.fieldSamplingParameters.samplingResolutionX = 100;
    dataSampler.fieldSamplingParameters.samplingStepX = 1.0 / dataSampler.fieldSamplingParameters.samplingResolutionX;
    dataSampler.fieldSamplingParameters.samplingResolutionY = 1;
    dataSampler.fieldSamplingParameters.samplingStepY = 0.0;

    dataSampler.outputFileName = outputFileName;
    dataSampler.fileForEveryIteration = true;

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

    std::cout << "Finished!" << std::endl;

    std::wstring path = L"..\\..\\Visualization\\Graphs\\Automated\\AutomatedRunGraphBuilder_SC";
    ShellExecuteW(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);

    return 0;
}