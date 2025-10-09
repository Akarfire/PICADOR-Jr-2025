#pragma once
#include "Grid.h"
#include "Particle.h"

class ParticleGrid: public Grid
{
	// Storage of particles
	std::vector<std::vector<Particle>> particlesInCells;


public:
	ParticleGrid(int inResolutionX, int inResolutionY, double inDeltaX, double inDeltaY, double originX, double originY);

	// Returns a reference to the vector of particles in the specified cell
	const std::vector<Particle>& getParticlesInCell(int i, int j);

	// Hands a particle from one cell over to another cell
	void paticleCellTransition(int particleID, int ownerCell_i, int ownerCell_j, int receiverCell_i, int receiverCell_j);
};