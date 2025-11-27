#include "Module_FieldGenerator.h"

#include "FieldGrid.h"

// Called by the core before entering the simulation loop
ModuleExecutionStatus FieldGenerator::onBegin()
{
    // This module only works with FieldGrid
    FieldGrid* fieldGrid = dynamic_cast<FieldGrid*>(core->getFieldContainer());
    if (!fieldGrid) return ModuleExecutionStatus::Error;

    // Generating field data for each node
    for (GRID_INDEX i = 0; i < fieldGrid->getResolutionX(); i++)
        for(GRID_INDEX j = 0; j < fieldGrid->getResolutionY(); j++)
        {
            Vector3 location = fieldGrid->getOrigin() + Vector3(j * fieldGrid->getDeltaX(), i * fieldGrid->getDeltaY());

            FieldData generatedFieldData = FieldData();
            generatedFieldData.E = E_Function(location);
            generatedFieldData.B = B_Function(location);
            generatedFieldData.J = J_Function(location);

            fieldGrid->getNodeAt(i, j) = generatedFieldData;
        }
}
