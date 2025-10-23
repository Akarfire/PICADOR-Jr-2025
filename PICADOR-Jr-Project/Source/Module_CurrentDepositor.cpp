#include "Module_CurrentDepositor.h"

ModuleExecutionStatus CurrentDepositor::onBegin() 
{
    // Casting core's field container to FieldGrid
    fieldGrid = dynamic_cast<FieldGrid*>(core->getFieldContainer());

    return ModuleExecutionStatus::Success;
}

ModuleExecutionStatus CurrentDepositor::onUpdate()
{
    if (fieldGrid)
    {
        return ModuleExecutionStatus::Success;
    }

    return ModuleExecutionStatus::Error;
}
