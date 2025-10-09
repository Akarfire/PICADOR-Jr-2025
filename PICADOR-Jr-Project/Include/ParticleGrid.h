#pragma once
#include "Grid.h"
#include "Particle.h"

class ParticleGrid: public Grid
{
	// Storage of particles
	std::vector<std::vector<Particle>> particlesInCells;


public:
	ParticleGrid(int resolutionX_, int resolutionY_, double deltaX_, double deltaY_, const Vector3& origin_): Grid(resolutionX_, resolutionY_, deltaX_, deltaY_, origin_) {}

	// Returns a reference to the vector of particles in the specified cell
	const std::vector<Particle>& getParticlesInCell(int i, int j);

	// Hands a particle from one cell over to another cell
	void paticleCellTransition(int particleID, int ownerCell_i, int ownerCell_j, int receiverCell_i, int receiverCell_j);
};