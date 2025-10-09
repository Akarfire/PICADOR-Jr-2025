#pragma once
#include "Grid.h"
#include "Particle.h"

class ParticleGrid: public Grid
{
	// Storage of particles
	std::vector<std::vector<Particle>> particlesInCells;

public:
	ParticleGrid(size_t resolutionX_, size_t resolutionY_, double deltaX_, double deltaY_, const Vector3& origin_): Grid(resolutionX_, resolutionY_, deltaX_, deltaY_, origin_) {}

	// Returns a reference to the vector of particles in the specified cell
	const std::vector<Particle>& getParticlesInCell(size_t i, size_t j) const;

	// Hands a particle from one cell over to another
	// Particle ID is the index of the particle in the owner cell
	void particleCellTransfer(size_t particleID, size_t ownerCell_i, size_t ownerCell_j, size_t receiverCell_i, size_t receiverCell_j);
};