#include "Module_LoopEdgeCondition.h"
#include "PicadorJrCore.h"

// Transfers particles of this cell onto the other end of the grid
void LoopEdgeCondition::processPaddingCell(GRID_INDEX i, GRID_INDEX j)
{
    std::vector<Particle>& particles = core->getParticleGrid()->editParticlesInCell(i, j);

    std::pair<GRID_INDEX, GRID_INDEX> loopingCell = calculateLoopingCell(i, j);

    Vector3 originalCellLocation = core->getParticleGrid()->getOrigin() + Vector3(core->getParticleGrid()->getDeltaX() * j, core->getParticleGrid()->getDeltaY() * i);
    Vector3 loopingCellLocation = core->getParticleGrid()->getOrigin() + Vector3(core->getParticleGrid()->getDeltaX() * loopingCell.second, core->getParticleGrid()->getDeltaY() * loopingCell.first);

    for (size_t p = 0; p < particles.size(); p++)
    {
        // Shifting particle location
        particles[p].location = particles[p].location - originalCellLocation + loopingCellLocation;

        // Transfering particle to the looping cell
        core->getParticleGrid()->particleCellTransfer(p, i, j, loopingCell.first, loopingCell.second);
    }
}

// Given a padding cell, calculates indeces of the corresponding looping cell on the main grid
std::pair<GRID_INDEX, GRID_INDEX> LoopEdgeCondition::calculateLoopingCell(GRID_INDEX i, GRID_INDEX j)
{
    // PLACE HOLDER
    return {i, j};
}

// Called by the core before entering the simulation loop
ModuleExecutionStatus LoopEdgeCondition::onBegin()
{
    if (core->getParticleGrid()->getPadding() > 0) return ModuleExecutionStatus::Success;
    else return ModuleExecutionStatus::Error;
}

// Called on every iteration of the simulation loop
ModuleExecutionStatus LoopEdgeCondition::onUpdate()
{
    // Loops over padding cells and calls processPaddingCell on them

    GRID_INDEX padding = core->getParticleGrid()->getPadding();

    // Left Pad
    for (GRID_INDEX j = -1 * padding; j < 0; j++)
        for (GRID_INDEX i = -1 * padding; i < core->getParticleGrid()->getResolutionY(); i++)
            processPaddingCell(i, j);

    // Top Pad
    for (GRID_INDEX j = -1 * padding; j < core->getParticleGrid()->getResolutionX(); j++)
        for (GRID_INDEX i = core->getParticleGrid()->getResolutionY(); i < core->getParticleGrid()->getResolutionY() + padding; i++)
            processPaddingCell(i, j);

    // Right Pad
    for (GRID_INDEX j = core->getParticleGrid()->getResolutionX(); j < core->getParticleGrid()->getResolutionX() + padding; j++)
        for (GRID_INDEX i = 0; i < core->getParticleGrid()->getResolutionY() + padding; i++)
            processPaddingCell(i, j);

    // Bottom Pad
    for (GRID_INDEX j = 0; j < core->getParticleGrid()->getResolutionX() + padding; j++)
        for (GRID_INDEX i = -1 * padding; i < 0; i++)
            processPaddingCell(i, j);


    return ModuleExecutionStatus::Success;
}