#include "Module_CurrentDepositor.h"
#include <iostream>

ModuleExecutionStatus CurrentDepositor::onBegin() 
{
    // Casting core's field container to FieldGrid
    fieldGrid = dynamic_cast<FieldGrid*>(core->getFieldContainer());
    particleGrid = core->getParticleGrid();

    if (fieldGrid) return ModuleExecutionStatus::Success;
    else return ModuleExecutionStatus::Error;
}

ModuleExecutionStatus CurrentDepositor::onUpdate()
{
    if (fieldGrid)
    {
        for (int i = 0; i < this->fieldGrid->getResolutionX(); i++) {
            for (int j = 0; j < this->fieldGrid->getResolutionY(); j++) {
                this->fieldGrid->getNodeAt(i, j).J = Vector3::Zero;
            }
        }

        this->edgeCondition.updateJEdge(this->fieldGrid);
        
        //std::cout << core->getCurrentIteration() << " : " << fieldGrid->getNodeAt(32, 4).J.x << std::endl;

        for (int i = 0; i < this->fieldGrid->getResolutionX(); i++) {
            for (int j = 0; j < this->fieldGrid->getResolutionY(); j++) {
                deposite(i, j);
            }
        }
        this->edgeCondition.updateJEdge(this->fieldGrid);

        //std::cout << "U : " << core->getCurrentIteration() << " : " << fieldGrid->getNodeAt(32, 4).J.x << std::endl;

        return ModuleExecutionStatus::Success;
    }

    return ModuleExecutionStatus::Error;
}

void CurrentDepositor::deposite(GRID_INDEX i, GRID_INDEX j) 
{
    const std::vector<Particle>& ensemble = this->particleGrid->getParticlesInCell(i, j);

    Vector3 delta(fieldGrid->getDeltaX(), fieldGrid->getDeltaY(), fieldGrid->getDeltaY());
    Vector3 cellOrigin(fieldGrid->getOrigin().x + i * delta.x, fieldGrid->getOrigin().y + j * delta.y, 0.0);

    for (int cellNumber = 0; cellNumber < ensemble.size(); cellNumber++) {
        Vector3 newLocation = (ensemble[cellNumber].location - cellOrigin) / delta;
        Vector3 temporaryJ =  ensemble[cellNumber].getVelocity() * ensemble[cellNumber].charge * ensemble[cellNumber].weight /delta.x /delta.y /delta.y;

        this->fieldGrid->getNodeAt(i, j).J = this->fieldGrid->getNodeAt(i, j).J + temporaryJ*(1.0 - newLocation.x) * (1.0 - newLocation.y);
        this->fieldGrid->getNodeAt(i, j+1).J = this->fieldGrid->getNodeAt(i, j+1).J + temporaryJ*(1.0 - newLocation.x) * (newLocation.y);
        this->fieldGrid->getNodeAt(i+1, j).J = this->fieldGrid->getNodeAt(i+1, j).J + temporaryJ*(newLocation.x) * (1.0 - newLocation.y);
        this->fieldGrid->getNodeAt(i+1, j+1).J = this->fieldGrid->getNodeAt(i+1, j+1).J + temporaryJ*(newLocation.x) * (newLocation.y);
    }
    
}
