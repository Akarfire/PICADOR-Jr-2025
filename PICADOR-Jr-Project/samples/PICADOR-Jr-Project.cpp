// PICADOR-Jr-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PicadorJrCore.h"

#include "Module_FieldSolver.h"
#include "Module_ParticleSolver.h"
#include "Module_CurrentDepositor.h"

#include "FieldGrid.h"
#include "ParticleGrid.h"

int main()
{
    // Defining field container
    FieldGrid fieldGrid = FieldGrid(100, 100, 1e-3, 1e-3, Vector3::Zero, 1);

    // Defining particle grid
    ParticleGrid particleGrid = ParticleGrid(100, 100, 1e-3, 1e-3, Vector3::Zero, 1);

    // Initializing core
    PicadorJrCore core = PicadorJrCore(&fieldGrid, &particleGrid, 1e-3, 1);

    // Field integrator module
    FieldSolver fieldIntegrator(&core);
    core.insertModule(&fieldIntegrator);

    // Particle integrator module
    ParticleSolver particleIntegrator(&core);
    core.insertModule(&particleIntegrator);

    // Current depositor
    CurrentDepositor currentDepositor(&core);
    core.insertModule(&currentDepositor);

    // ... Inserting additional modules (if needed) ...

    core.run();

    return 0;
}
