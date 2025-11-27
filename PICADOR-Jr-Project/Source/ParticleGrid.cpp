#include "ParticleGrid.h"
#include <stdexcept>

ParticleGrid::ParticleGrid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, const Vector3& origin_, size_t padding_): 
	Grid(resolutionX_, resolutionY_, deltaX_, deltaY_, origin_, padding_) 
{
    this->particlesInCells = std::vector<std::vector<Particle>>((this->resolutionX + 2 * this->padding) * (this->resolutionY + 2 * this->padding));
}

// Returns a reference to the vector of particles in the specified cell
const std::vector<Particle>& ParticleGrid::getParticlesInCell(GRID_INDEX i, GRID_INDEX j) const 
{ 
    if (i < (0 - (GRID_INDEX)padding) || i >= ((GRID_INDEX)resolutionX - 1 + (GRID_INDEX)padding) || j < (0 - (GRID_INDEX)padding) || j >= ((GRID_INDEX)resolutionY - 1 + (GRID_INDEX)padding)) throw(std::runtime_error("Invalid particle cell index!"));
    return particlesInCells[recalculateCellIndex(i, j)];
}

// Returns an editable reference to the vector of particles in the specified cell
std::vector<Particle>& ParticleGrid::editParticlesInCell(GRID_INDEX i, GRID_INDEX j)
{
    if (i < (0 - (GRID_INDEX)padding) || i >= ((GRID_INDEX)resolutionX - 1 + (GRID_INDEX)padding) || j < (0 - (GRID_INDEX)padding) || j >= ((GRID_INDEX)resolutionY - 1 + (GRID_INDEX)padding)) throw(std::runtime_error("Invalid particle cell index!"));
    return particlesInCells[recalculateCellIndex(i, j)];
}

// Hands a particle from one cell over to another
// Particle ID is the index of the particle in the owner cell
void ParticleGrid::particleCellTransfer(size_t particleID, GRID_INDEX ownerCell_i, GRID_INDEX ownerCell_j, GRID_INDEX receiverCell_i, GRID_INDEX receiverCell_j)
{
    // Error checking
    if (ownerCell_i < 0 - (GRID_INDEX)padding || ownerCell_j < 0 - (GRID_INDEX)padding || ownerCell_i >= (GRID_INDEX)(resolutionX - 1 + padding) || ownerCell_j >= (GRID_INDEX)(resolutionY - 1 + padding))
        throw(std::runtime_error("Invalid owner cell index!"));

    if (receiverCell_i < 0 - (GRID_INDEX)padding || receiverCell_j < 0 - (GRID_INDEX)padding || receiverCell_i >= (GRID_INDEX)(resolutionX - 1 + padding) || receiverCell_j >= (GRID_INDEX)(resolutionY - 1 + padding))
        throw(std::runtime_error("Invalid receiver cell index!"));

    // Handling self-transfer
    if (ownerCell_i == receiverCell_i && ownerCell_j == receiverCell_j) return;
    
    // Main logic

    size_t numParticlesInOwner = getParticlesInCell(ownerCell_i, ownerCell_j).size();

    if (particleID < 0 || particleID >= numParticlesInOwner)
        throw(std::runtime_error("Invalid particle ID for transfer!"));

    // Calculating cell indeces
    size_t owner_ID = recalculateCellIndex(ownerCell_i, ownerCell_j);
    size_t receiver_ID = recalculateCellIndex(receiverCell_i, receiverCell_j);

    // Actual transfer
    particlesInCells[receiver_ID].push_back( particlesInCells[owner_ID][particleID] );

    particlesInCells[owner_ID][particleID] = particlesInCells[owner_ID][numParticlesInOwner - 1];
    particlesInCells[owner_ID].pop_back();
}

// Converts 2D cell coordinates into a 1D array index
size_t ParticleGrid::recalculateCellIndex(GRID_INDEX i, GRID_INDEX j) const 
{
    return (i + padding) * (this->resolutionY - 1 + 2 * padding) + (j + padding);
}