// PICADOR-Jr-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PicadorJrCore.h"
#include "FieldIntegrator.h"
#include "ParticleIntegrator.h"

int main()
{
    PicadorJrCore core = PicadorJrCore(/* ... */);

    // Field integrator module
    FieldIntegrator fieldIntegrator(&core);
    core.insertModule(&fieldIntegrator);

    // Particle integrator module
    ParticleIntegrator particleIntegrator(&core);
    core.insertModule(&particleIntegrator);

    // ... Inserting additional modules (if needed) ...

    core.run();

    return 0;
}
