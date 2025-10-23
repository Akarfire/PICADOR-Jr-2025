#include "PicadorJrCore.h"
#include "Module.h"

#include <string> 
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
    for (size_t i = 0; i < modules.size(); i++)
    {
        ModuleExecutionStatus status = modules[i]->onBegin();

        // Remove module from executing, if it has returned an error
        if (status == ModuleExecutionStatus::Error)
            modules.erase(modules.begin() + i);
    }

    currentTime = 0;

    // Main simulation loop
    while (currentTime < timeDomainBound)
    {
        // Running updates on modules
        for (size_t i = 0; i < modules.size(); i++)
        {
            ModuleExecutionStatus status = modules[i]->onUpdate();

            // Remove module from executing, if it has returned an error
            if (status == ModuleExecutionStatus::Error)
                modules.erase(modules.begin() + i);
        }

        currentTime += timeDelta;
    }

    // Running onEndRun on modules
    for (size_t i = 0; i < modules.size(); i++)
    {
        ModuleExecutionStatus status = modules[i]->onEnd();
        
        //if (status == ModuleExecutionStatus::Error)
    }

    return 0;
}