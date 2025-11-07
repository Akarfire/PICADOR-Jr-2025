#include "Module_FieldSolver.h"

ModuleExecutionStatus FieldSolver::onBegin() 
{
    // Casting core's field container to FieldGrid
    fieldGrid = dynamic_cast<FieldGrid*>(core->getFieldContainer());

    if (fieldGrid) return ModuleExecutionStatus::Success;
    else return ModuleExecutionStatus::Error;
}

ModuleExecutionStatus FieldSolver::onUpdate()
{
    for (int i = 0; i < fieldGrid->getResolutionX(); i++) {
        for (int j = 0; j < fieldGrid->getResolutionY(); j++) updateE(i, j);
    }

    edgeCondition.updateEEdge(this->fieldGrid);

    for (int i = 0; i < fieldGrid->getResolutionX(); i++) {
        for (int j = 0; j < fieldGrid->getResolutionY(); j++) updateB(i, j);
    }

    edgeCondition.updateBEdge(this->fieldGrid);

    return ModuleExecutionStatus::Success;
}

void FieldSolver::updateE(GRID_INDEX i, GRID_INDEX j) 
{
    fieldGrid->getNodeAt(i, j).E.x = fieldGrid->getNodeAt(i, j).E.x - 4 * Constants::PI * core->getTimeDelta() * fieldGrid->getNodeAt(i, j).J.x
        + Constants::SpeedOfLight * core->getTimeDelta() * (fieldGrid->getNodeAt(i, j + 1).B.z - fieldGrid->getNodeAt(i, j - 1).B.z) / (2 * fieldGrid->getDeltaY());

    fieldGrid->getNodeAt(i, j).E.y = fieldGrid->getNodeAt(i, j).E.y - 4 * Constants::PI * core->getTimeDelta() * fieldGrid->getNodeAt(i, j).J.y
        + Constants::SpeedOfLight * core->getTimeDelta() * (fieldGrid->getNodeAt(i + 1, j).B.z - fieldGrid->getNodeAt(i - 1, j).B.z) / (2 * fieldGrid->getDeltaX());

    fieldGrid->getNodeAt(i, j).E.z = fieldGrid->getNodeAt(i, j).E.z - 4 * Constants::PI * core->getTimeDelta() * fieldGrid->getNodeAt(i, j).J.z
        + Constants::SpeedOfLight * core->getTimeDelta() * ((fieldGrid->getNodeAt(i + 1, j).B.y - fieldGrid->getNodeAt(i-1, j).B.y) / (2 * fieldGrid->getDeltaX())
            - (fieldGrid->getNodeAt(i, j + 1).B.x - fieldGrid->getNodeAt(i, j - 1).B.x) / (2 * fieldGrid->getDeltaY()));
}

void FieldSolver::updateB(GRID_INDEX i, GRID_INDEX j) 
{
    fieldGrid->getNodeAt(i, j).B.x = fieldGrid->getNodeAt(i, j).B.x 
        - Constants::SpeedOfLight * core->getTimeDelta() * (fieldGrid->getNodeAt(i, j + 1).E.z - fieldGrid->getNodeAt(i, j - 1).E.z) / (2 * fieldGrid->getDeltaY());

    fieldGrid->getNodeAt(i, j).B.y = fieldGrid->getNodeAt(i, j).B.y //или тут -, надо проверить
        + Constants::SpeedOfLight * core->getTimeDelta() * (fieldGrid->getNodeAt(i + 1, j).E.z - fieldGrid->getNodeAt(i - 1, j).E.z) / (2 * fieldGrid->getDeltaX());

    fieldGrid->getNodeAt(i, j).B.z = fieldGrid->getNodeAt(i, j).B.z
        - Constants::SpeedOfLight * core->getTimeDelta() * ((fieldGrid->getNodeAt(i + 1, j).E.y - fieldGrid->getNodeAt(i - 1, j).E.y) / (2 * fieldGrid->getDeltaX())
            - (fieldGrid->getNodeAt(i, j + 1).E.x - fieldGrid->getNodeAt(i, j - 1).E.x) / (2 * fieldGrid->getDeltaY()));
}

