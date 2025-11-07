#include "FieldGrid.h"
#include "Module_FieldSolver.h"

#include <cmath>

#include <gtest.h>

TEST(FieldSolver, monochrElMagWaveTest)
{
    int n = 21, m = 21;
    double deltaX = 1e-3, deltaY = 1e-3;
    FieldGrid fieldGrid = FieldGrid(n, m, deltaX, deltaY, Vector3::Zero, 1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fieldGrid.getNodeAt(i, j).E.y = sin(2 * Constants::PI * (i) / ((n-1)*deltaX));
            fieldGrid.getNodeAt(i, j).B.z = sin(2 * Constants::PI * (i) / ((n - 1) * deltaX));
            
        }std::cout << fieldGrid.getNodeAt(i, 0).B.z << " ";
    }

    FieldLoopEdgeCondition edgeCondition;
    edgeCondition.updateBEdge(&fieldGrid);
    edgeCondition.updateEEdge(&fieldGrid);

    // Defining particle grid
    ParticleGrid particleGrid = ParticleGrid(n, m, deltaX, deltaY, Vector3::Zero, 1);

    // Initializing core
    PicadorJrCore core = PicadorJrCore(&fieldGrid, &particleGrid);
    core.getTimeDelta() = 1e-3;
    // Field integrator module
    FieldSolver fieldIntegrator(&core);

    fieldIntegrator.onBegin();

    for (int t = 0; t < 100; t++) {
        fieldIntegrator.onUpdate();
        std::cout << "\n\nBy numeric: ";
        for (int i = 0; i < n; i++) std::cout << fieldGrid.getNodeAt(i, 0).B.z << " ";

        std::cout << "\nBz analitic: ";
        for (int i = 0; i < n; i++) std::cout << sin(2 * Constants::PI * (i - Constants::SpeedOfLight* t*core.getTimeDelta()) / ((n - 1) * deltaX)) << " ";
    }
}

