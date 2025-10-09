#include "Module.h"

class FieldIntegrator : public Module
{
public:
    FieldIntegrator(PicadorJrCore* core_): Module(core_) {}

    virtual ModuleExecutionStatus runModule() override
    {
        return ModuleExecutionStatus::Success;
    }
};