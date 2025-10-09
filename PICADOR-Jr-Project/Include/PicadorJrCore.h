#pragma once

#include "FieldGrid.h"
#include "ParticleGrid.h"

class PicadorJrCore final
{
	// Basic data

	FieldGrid* fieldGrid;
	ParticleGrid* particleGrid;

	// Execution list of calculation modules
	std::vector<class Module*> modules;


	// Simulation settings

	double timeDelta;

	// Maximum simulation time
	double timeDomainBound;

	// ... maybe grid settings? ...

	// ...

	// Runtime values
	double currentTime;

public:

	// Initializes modules and loads data
	PicadorJrCore(/* ... */);
	~PicadorJrCore();

	// Adds a new module ton the execution list
	void insertModule(class Module* module, int position = -1);

	// Runs calculations
	int run();


	// Module interface

	FieldGrid* getFieldGrid() { return fieldGrid; }

	ParticleGrid* getParticleGrid() { return particleGrid; }

	double getTimeDelta() { return timeDelta; }
};