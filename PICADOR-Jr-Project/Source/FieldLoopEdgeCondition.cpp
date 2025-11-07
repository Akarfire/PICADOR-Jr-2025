#include "FieldLoopEdgeCondition.h"

std::pair<GRID_INDEX, GRID_INDEX> FieldLoopEdgeCondition::calculateLoopIndices(FieldGrid *fieldGrid, GRID_INDEX i, GRID_INDEX j) {
    std::pair<GRID_INDEX, GRID_INDEX> loopIndices;

    loopIndices.first = (i + fieldGrid->getResolutionX()) % fieldGrid->getResolutionX();
    loopIndices.second = (j + fieldGrid->getResolutionY()) % fieldGrid->getResolutionY();

    return loopIndices;
}

ModuleExecutionStatus FieldLoopEdgeCondition::onUpdate()
{
    for (GRID_INDEX i = -this->fieldGrid->getPadding(); i < 0; i++) {
        for (GRID_INDEX j = -this->fieldGrid->getPadding(); j < this->fieldGrid->getResolutionY() + this->fieldGrid->getPadding(); j++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = recallculateCellIndex(i, j);
            this->fieldGrid->getNodeAt(i, j) = this->fieldGrid->getNodeAt(loopIndices.first, loopIndices.second);
        }
    }

    for (GRID_INDEX i = this->fieldGrid->getResolutionX(); i < this->fieldGrid->getResolutionX() + this->fieldGrid->getPadding(); i++) {
        for (GRID_INDEX j = -this->fieldGrid->getPadding(); j < this->fieldGrid->getResolutionY() + this->fieldGrid->getPadding(); j++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = recallculateCellIndex(i, j);
            this->fieldGrid->getNodeAt(i, j) = this->fieldGrid->getNodeAt(loopIndices.first, loopIndices.second);
        }
    }

    for (GRID_INDEX j = -this->fieldGrid->getPadding(); j < 0; j++) {
        for (GRID_INDEX i = 0; i < this->fieldGrid->getResolutionX(); i++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = recallculateCellIndex(i, j);
            this->fieldGrid->getNodeAt(i, j) = this->fieldGrid->getNodeAt(loopIndices.first, loopIndices.second);
        }
    }

    for (GRID_INDEX j = this->fieldGrid->getResolutionY(); j < this->fieldGrid->getResolutionY() + this->fieldGrid->getPadding(); j++) {
        for (GRID_INDEX i = 0; i < this->fieldGrid->getResolutionX(); i++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = recallculateCellIndex(i, j);
            this->fieldGrid->getNodeAt(i, j) = this->fieldGrid->getNodeAt(loopIndices.first, loopIndices.second);
        }
    }

    return ModuleExecutionStatus::Success;
}

