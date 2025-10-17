#include "ParticleGrid.h"
#include <stdexcept>

ParticleGrid::ParticleGrid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, const Vector3& origin_, size_t padding_): 
	Grid(resolutionX_, resolutionY_, deltaX_, deltaY_, origin_, padding_) 
{
    this->particlesInCells = std::vector<std::vector<Particle>>((this->resolutionX + 2 * this->padding) * (this->resolutionY + 2 * this->padding));
}

// Returns a reference to the vector of particles in the specified cell
const std::vector<Particle>& ParticleGrid::getParticlesInCell(size_t i, size_t j) const 
{ 
    if (i < 0 || i >= resolutionX || j < 0 || j >= resolutionY) throw(std::runtime_error("Invalid particle cell index!"));
    return particlesInCells[recalculateCellIndex(i, j)];
}

// Hands a particle from one cell over to another
// Particle ID is the index of the particle in the owner cell
int ParticleGrid::particleCellTransfer(size_t particleID, size_t ownerCell_i, size_t ownerCell_j, size_t receiverCell_i, size_t receiverCell_j)
{
    if (ownerCell_i < 0 || ownerCell_j < 0 || ownerCell_i >= resolutionX || ownerCell_j >= resolutionY) return 1;

    // NOTE: We allow receiver cell to go out of bounds, to accomadate looping / particle removal
    
    size_t numParticlesInOwner = getParticlesInCell(ownerCell_i, ownerCell_j).size();

    if (particleID < 0 || particleID >= numParticlesInOwner) return 1;

    // Identifying receiver cell (Looping)
    size_t realReceiver_i;
    size_t realReceiver_j;

    if (receiverCell_i < 0) realReceiver_i = resolutionX + receiverCell_i;
    else if (receiverCell_i >= resolutionX) realReceiver_i = receiverCell_i % resolutionX;

    if (receiverCell_j < 0) realReceiver_j = resolutionY + receiverCell_j;
    else if (receiverCell_j >= resolutionY) realReceiver_j = receiverCell_j % resolutionY;

    // Calculating cell indeces
    size_t owner_ID = recalculateCellIndex(ownerCell_i, ownerCell_j);
    size_t receiver_ID = recalculateCellIndex(realReceiver_i, realReceiver_j);

    // Actual transfer
    particlesInCells[receiver_ID].push_back( particlesInCells[owner_ID][particleID] );

    particlesInCells[owner_ID][particleID] = particlesInCells[owner_ID][numParticlesInOwner - 1];
    particlesInCells[owner_ID].pop_back();

    // TO DO : Location shift

    return 0;
}

size_t ParticleGrid::recalculateCellIndex(size_t i, size_t j) const 
{
    return (i + padding) * this->resolutionX + (j + padding);
}