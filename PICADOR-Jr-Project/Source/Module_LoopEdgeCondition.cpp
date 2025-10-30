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
    std::pair<GRID_INDEX, GRID_INDEX> result = {0, 0};

    GRID_INDEX resX = core->getParticleGrid()->getResolutionX();
    GRID_INDEX resY = core->getParticleGrid()->getResolutionY();

    if (i == -1)
        result.first = i + resY - 1;

    else if (i == resY - 1)
        result.first = 0;

    if (j == -1)
        result.second = j + resX - 1;
    
    else if (j == resX - 1)
        result.second = 0;

    return result;
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
    GRID_INDEX padding = core->getParticleGrid()->getPadding();
    if (padding == 0) return ModuleExecutionStatus::Error;

    // Loops over padding cells and calls processPaddingCell on them

    GRID_INDEX resX = core->getParticleGrid()->getResolutionX();
    GRID_INDEX resY = core->getParticleGrid()->getResolutionY();

    // Left Pad
    for (GRID_INDEX i = -1; i < resY; i++)
        processPaddingCell(i, -1);

    // Top Pad
    for (GRID_INDEX j = -1; j < resX; j++)
        processPaddingCell(resY - 1, j);

    // Right Pad
    for (GRID_INDEX i = resY - 1; i >= -1; i--)
        processPaddingCell(i, resX - 1);

    // Bottom Pad
    for (GRID_INDEX j = resX - 1; j >= -1; j--)
        processPaddingCell(-1, j);

    return ModuleExecutionStatus::Success;
}