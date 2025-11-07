#include "FieldLoopEdgeCondition.h"

std::pair<GRID_INDEX, GRID_INDEX> FieldLoopEdgeCondition::calculateLoopIndices(FieldGrid *fieldGrid, GRID_INDEX i, GRID_INDEX j) {
    std::pair<GRID_INDEX, GRID_INDEX> loopIndices;

    loopIndices.first = (i + fieldGrid->getResolutionX()) % fieldGrid->getResolutionX();
    loopIndices.second = (j + fieldGrid->getResolutionY()) % fieldGrid->getResolutionY();

    return loopIndices;
}

void FieldLoopEdgeCondition::updateEEdge(FieldGrid* fieldGrid) {
    for (GRID_INDEX i = -(GRID_INDEX)fieldGrid->getPadding(); i < 0; i++) {
        for (GRID_INDEX j = -(GRID_INDEX)fieldGrid->getPadding(); j < (GRID_INDEX)fieldGrid->getResolutionY() + (GRID_INDEX)fieldGrid->getPadding(); j++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).E = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).E;
        }
    }

    for (GRID_INDEX i = (GRID_INDEX)fieldGrid->getResolutionX(); i < (GRID_INDEX)fieldGrid->getResolutionX() + (GRID_INDEX)fieldGrid->getPadding(); i++) {
        for (GRID_INDEX j = -(GRID_INDEX)fieldGrid->getPadding(); j < (GRID_INDEX)fieldGrid->getResolutionY() + (GRID_INDEX)fieldGrid->getPadding(); j++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).E = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).E;
        }
    }

    for (GRID_INDEX j = -(GRID_INDEX)fieldGrid->getPadding(); j < 0; j++) {
        for (GRID_INDEX i = 0; i < (GRID_INDEX)fieldGrid->getResolutionX(); i++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).E = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).E;
        }
    }

    for (GRID_INDEX j = (GRID_INDEX)fieldGrid->getResolutionY(); j < (GRID_INDEX)fieldGrid->getResolutionY() + (GRID_INDEX)fieldGrid->getPadding(); j++) {
        for (GRID_INDEX i = 0; i < (GRID_INDEX)fieldGrid->getResolutionX(); i++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).E = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).E;
        }
    }
}

void FieldLoopEdgeCondition::updateBEdge(FieldGrid* fieldGrid) {
    for (GRID_INDEX i = -(GRID_INDEX)fieldGrid->getPadding(); i < 0; i++) {
        for (GRID_INDEX j = -(GRID_INDEX)fieldGrid->getPadding(); j < (GRID_INDEX)fieldGrid->getResolutionY() + (GRID_INDEX)fieldGrid->getPadding(); j++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).B = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).B;
        }
    }

    for (GRID_INDEX i = (GRID_INDEX)fieldGrid->getResolutionX(); i < (GRID_INDEX)fieldGrid->getResolutionX() + (GRID_INDEX)fieldGrid->getPadding(); i++) {
        for (GRID_INDEX j = -(GRID_INDEX)fieldGrid->getPadding(); j < (GRID_INDEX)fieldGrid->getResolutionY() + (GRID_INDEX)fieldGrid->getPadding(); j++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).B = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).B;
        }
    }

    for (GRID_INDEX j = -(GRID_INDEX)fieldGrid->getPadding(); j < 0; j++) {
        for (GRID_INDEX i = 0; i < (GRID_INDEX)fieldGrid->getResolutionX(); i++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).B = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).B;
        }
    }

    for (GRID_INDEX j = (GRID_INDEX)fieldGrid->getResolutionY(); j < (GRID_INDEX)fieldGrid->getResolutionY() + (GRID_INDEX)fieldGrid->getPadding(); j++) {
        for (GRID_INDEX i = 0; i < (GRID_INDEX)fieldGrid->getResolutionX(); i++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).B = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).B;
        }
    }
}

void FieldLoopEdgeCondition::updateJEdge(FieldGrid* fieldGrid) {
    for (GRID_INDEX i = -(GRID_INDEX)fieldGrid->getPadding(); i < 0; i++) {
        for (GRID_INDEX j = -(GRID_INDEX)fieldGrid->getPadding(); j < (GRID_INDEX)fieldGrid->getResolutionY() + (GRID_INDEX)fieldGrid->getPadding(); j++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).J = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).J;
        }
    }

    for (GRID_INDEX i = (GRID_INDEX)fieldGrid->getResolutionX(); i < (GRID_INDEX)fieldGrid->getResolutionX() + (GRID_INDEX)fieldGrid->getPadding(); i++) {
        for (GRID_INDEX j = -(GRID_INDEX)fieldGrid->getPadding(); j < (GRID_INDEX)fieldGrid->getResolutionY() + (GRID_INDEX)fieldGrid->getPadding(); j++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).J = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).J;
        }
    }

    for (GRID_INDEX j = -(GRID_INDEX)fieldGrid->getPadding(); j < 0; j++) {
        for (GRID_INDEX i = 0; i < (GRID_INDEX)fieldGrid->getResolutionX(); i++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).J = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).J;
        }
    }

    for (GRID_INDEX j = (GRID_INDEX)fieldGrid->getResolutionY(); j < (GRID_INDEX)fieldGrid->getResolutionY() + (GRID_INDEX)fieldGrid->getPadding(); j++) {
        for (GRID_INDEX i = 0; i < (GRID_INDEX)fieldGrid->getResolutionX(); i++) {
            std::pair<GRID_INDEX, GRID_INDEX> loopIndices = calculateLoopIndices(fieldGrid, i, j);
            fieldGrid->getNodeAt(i, j).J = fieldGrid->getNodeAt(loopIndices.first, loopIndices.second).J;
        }
    }
}


