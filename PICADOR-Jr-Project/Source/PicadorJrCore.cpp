#include "PicadorJrCore.h"
#include "Module.h"

#include <stdexcept>

// Initializes modules and loads data
PicadorJrCore::PicadorJrCore(FieldContainer* fieldContainer_, ParticleGrid* particleGrid_)
{
    fieldContainer = fieldContainer_;
    particleGrid = particleGrid_;

    // TO DO: Loading config and initial data from file
}

PicadorJrCore::~PicadorJrCore()
{
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
    // Running onBeginRun on modules
    for (auto module : modules)
        ModuleExecutionStatus status = module->onBegin();

    currentTime = 0;

    // Main simulation loop
    while (currentTime < timeDomainBound)
    {
        // Running updates on modules
        for (auto module : modules)
            ModuleExecutionStatus status = module->onUpdate();

        currentTime += timeDelta;
    }

    // Running onEndRun on modules
    for (auto module : modules)
        ModuleExecutionStatus status = module->onEnd();

    return 0;
}