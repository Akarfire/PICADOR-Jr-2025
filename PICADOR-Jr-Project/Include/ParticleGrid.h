#pragma once
#include "Grid.h"
#include "Particle.h"

class ParticleGrid: public Grid
{
	// Storage of particles
	std::vector<std::vector<Particle>> particlesInCells;

public:
	// Converts 2D cell coordinates into a 1D array index
	size_t recalculateCellIndex(GRID_INDEX i, GRID_INDEX j) const;

	ParticleGrid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, const Vector3& origin_, size_t padding_);

	// Returns a reference to the vector of particles in the specified cell
	const std::vector<Particle>& getParticlesInCell(GRID_INDEX i, GRID_INDEX j) const;

	// Hands a particle from one cell over to another
	// Particle ID is the index of the particle in the owner cell
	// Returns 0 if successful, 1 - if failed
	int particleCellTransfer(size_t particleID, GRID_INDEX ownerCell_i, GRID_INDEX ownerCell_j, GRID_INDEX receiverCell_i, GRID_INDEX receiverCell_j);
};