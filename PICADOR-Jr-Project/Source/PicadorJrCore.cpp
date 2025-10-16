#include "PicadorJrCore.h"
#include "Module.h"

#include <stdexcept>

// Initializes modules and loads data
PicadorJrCore::PicadorJrCore(/* ... */)
{
    // TO DO: Loading config and initial data from file

    // !!!PLACE-HOLDER VALUES!!!
    fieldGrid = new FieldGrid(100, 100, 10e-3, 10e-3, Vector3::Zero);
    particleGrid = new ParticleGrid(100, 100, 10e-3, 10e-3, Vector3::Zero);
}

PicadorJrCore::~PicadorJrCore()
{
    delete fieldGrid;
    delete particleGrid;
}


// Adds a new module ton the execution list
void PicadorJrCore::insertModule(Module* module, int position)
{
    // Appening
    if (position == -1)
        return modules.push_back(module);
    
    // Error checking
    if (position < 0 || position > (int)(modules.size())) throw(std::runtime_error("Invalid module insertion position!"));

    // Insertion
    modules.insert(modules.begin() + position, module);
}

// Runs calculations
int PicadorJrCore::run()
{
    currentTime = 0;

    // Main calculation loop
    while (currentTime < timeDomainBound)
    {
        // Running modules
        for (auto module : modules)
        {
            ModuleExecutionStatus status = module->runModule();
        }

        currentTime += timeDelta;
    }

    return 0;
}