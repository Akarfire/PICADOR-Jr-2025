#pragma once

#include "Grid.h"
#include "Particle.h"

class PicadorJrCore
{
	// Basic data

	Grid grid;

	// Cell (based on grid) -> Particle
	std::vector<std::vector<Particle>> particlesInCells;

	// Execution list of calculation modules
	std::vector<class Module*> modules;


	// Simulation settings

	double timeDelta;

	// ... maybe grid settings? ...

	// ...


public:

	// Initializes modules and loads data
	PicadorJrCore(/* ... */);

	// Adds a new module ton the execution list
	void insertModule(class Module*, int position = -1);

	// Runs calculations
	int run();


	// Module interface

	Grid& getGrid() { return grid; }

	std::vector<std::vector<Particle>>& getParticles() { return particlesInCells; }
};