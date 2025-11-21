#include "PicadorJrCore.h"
#include "StaticField.h"
#include "ParticleGrid.h"
#include "Module_ParticleGenerator.h"
#include "Module_DataSampler.h"

#include <string>
#include <fstream>
#include <windows.h>
#include <shellapi.h>
#include <math.h>

int main()
{
    ParticleGenerator::ParticleGenerationProfile profile;

    profile.sampleParticle = Particle(ParticleType::Electron, Constants::ElectronMass, Constants::ElectronCharge);

    profile.particleDensityFunction = [](Vector3 location ) { return ( max(sin(location.x) * 3 + sin(location.y) * 3, 0) ); };
    profile.temperatureFunction = [](Vector3) { return 1e-20; };

    std::string speedOutputFileName = "speedGeneratorData.txt";

    // Output file for automated location (trajectory) visualization
    std::string locationOutputFileName = "../../Visualization/Trajectory/Automated/particle_trajectories_auto_vis.txt";


    // Initializing core
    ParticleGrid particleGrid(50, 50, 1, 1, Vector3::Zero, 0);
    StaticField staticField = StaticField(FieldData());
    PicadorJrCore core(&staticField, &particleGrid, 1, 1);

    // Particle generator
    ParticleGenerator particleGenerator(&core, 1);
    particleGenerator.addGenerationProfile(profile);
    core.insertModule(&particleGenerator);

    DataSampler dataSampler(&core);
    dataSampler.sampleParticleLocations = true;
    dataSampler.sampleParticleVelocities = false;
    dataSampler.additionalDataFlags.push_back("OnlyInitialDistribution");
    dataSampler.writeParticleGridParameters = true;
    dataSampler.outputFileName = locationOutputFileName;

    core.insertModule(&dataSampler);

    core.run();

    // Autorun distribution visualization
    std::wstring path = L"..\\..\\Visualization\\Trajectory\\Automated\\AutoRunTrajectoryBuilder_SC";
    ShellExecuteW(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);

    // return 0;
}