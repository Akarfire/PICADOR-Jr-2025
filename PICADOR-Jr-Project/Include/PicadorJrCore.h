#pragma once

#include "FieldContainer.h"
#include "ParticleGrid.h"

class PicadorJrCore final
{
	// Basic data

	FieldContainer* fieldContainer;
	ParticleGrid* particleGrid;

	// Execution list of calculation modules
	std::vector<class Module*> modules;


	// Simulation settings

	double timeDelta;

	// Maximum simulation time
	double timeDomainBound;

	// ...


	// Runtime values

	double currentTime;

public:

	// Initializes modules and loads data
	PicadorJrCore(FieldContainer* fieldContainer_, ParticleGrid* particleGrid_);
	~PicadorJrCore();

	// Adds a new module ton the execution list
	void insertModule(class Module* module, int position = -1);

	// Runs calculations
	int run();


	// Module interface

	FieldContainer* getFieldContainer() { return fieldContainer; }

	ParticleGrid* getParticleGrid() { return particleGrid; }

	double getTimeDelta() const { return timeDelta; }
};