#include "FieldGrid.h"
#include "Module_FieldSolver.h"

#include <cmath>

#include <gtest.h>

TEST(FieldSolver, monochrElMagWaveTestForX)
{
    int n = 128, m = 21;
    double deltaX = Constants::SpeedOfLight, deltaY = 1;
    FieldGrid fieldGrid = FieldGrid(n, m, deltaX, deltaY, Vector3::Zero, 1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fieldGrid.getNodeAt(i, j).E.y = sin(2 * Constants::PI * (i) * deltaX / ((n - 1) * deltaX));
            fieldGrid.getNodeAt(i, j).B.z = sin(2 * Constants::PI * (i) * deltaX / ((n - 1) * deltaX));
        }
    }

    FieldLoopEdgeCondition edgeCondition;
    edgeCondition.updateBEdge(&fieldGrid);
    edgeCondition.updateEEdge(&fieldGrid);

    // Defining particle grid
    ParticleGrid particleGrid = ParticleGrid(n, m, deltaX, deltaY, Vector3::Zero, 1);

    // Initializing core
    PicadorJrCore core = PicadorJrCore(&fieldGrid, &particleGrid, 1e-8, 100);
    
    // Field integrator module
    FieldSolver fieldIntegrator(&core);

    fieldIntegrator.onBegin();

    for (int t = 0; t < 40; t++) {
        fieldIntegrator.onUpdate();
    }
    for (int i = 0; i < n; i++) 
    {
        EXPECT_NEAR(fieldGrid.getNodeAt(i, 0).B.z, sin(2 * Constants::PI * (i * deltaX - Constants::SpeedOfLight * 40 * core.getTimeDelta()) / ((n - 1) * deltaX)), 1e-7);
        EXPECT_NEAR(fieldGrid.getNodeAt(i, 0).E.y, sin(2 * Constants::PI * (i * deltaX - Constants::SpeedOfLight * 40 * core.getTimeDelta()) / ((n - 1) * deltaX)), 1e-7);

        EXPECT_EQ(fieldGrid.getNodeAt(i, 0).E.x, 0.0);
        EXPECT_EQ(fieldGrid.getNodeAt(i, 0).E.z, 0.0);
        EXPECT_EQ(fieldGrid.getNodeAt(i, 0).B.x, 0.0);
        EXPECT_EQ(fieldGrid.getNodeAt(i, 0).B.y, 0.0);
    }
}

TEST(FieldSolver, monochrElMagWaveTestForY)
{
    int n = 21, m = 128;
    double deltaX = 1, deltaY = Constants::SpeedOfLight;
    FieldGrid fieldGrid = FieldGrid(n, m, deltaX, deltaY, Vector3::Zero, 1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fieldGrid.getNodeAt(i, j).E.z = sin(2 * Constants::PI * (j) * deltaY / ((m - 1) * deltaY));
            fieldGrid.getNodeAt(i, j).B.x = sin(2 * Constants::PI * (j) * deltaY / ((m - 1) * deltaY));
        }
    }

    FieldLoopEdgeCondition edgeCondition;
    edgeCondition.updateBEdge(&fieldGrid);
    edgeCondition.updateEEdge(&fieldGrid);

    // Defining particle grid
    ParticleGrid particleGrid = ParticleGrid(n, m, deltaX, deltaY, Vector3::Zero, 1);

    // Initializing core
    PicadorJrCore core = PicadorJrCore(&fieldGrid, &particleGrid, 1e-8, 100);

    // Field integrator module
    FieldSolver fieldIntegrator(&core);

    fieldIntegrator.onBegin();

    for (int t = 0; t < 100; t++) {
        fieldIntegrator.onUpdate();
    }
    for (int j = 0; j < m; j++)
    {
        EXPECT_NEAR(fieldGrid.getNodeAt(0, j).E.z, sin(2 * Constants::PI * (j * deltaY - Constants::SpeedOfLight * 40 * core.getTimeDelta()) / ((m - 1) * deltaY)), 1e-7);
        EXPECT_NEAR(fieldGrid.getNodeAt(0, j).B.x, sin(2 * Constants::PI * (j * deltaY - Constants::SpeedOfLight * 40 * core.getTimeDelta()) / ((m - 1) * deltaY)), 1e-7);

        EXPECT_EQ(fieldGrid.getNodeAt(0, j).E.x, 0.0);
        EXPECT_EQ(fieldGrid.getNodeAt(0, j).E.y, 0.0);
        EXPECT_EQ(fieldGrid.getNodeAt(0, j).B.z, 0.0);
        EXPECT_EQ(fieldGrid.getNodeAt(0, j).B.y, 0.0);
    }
}

//TEST(FieldSolver, pupupu) {
//
//    int n = 128, m = 21;
//    double deltaX = Constants::SpeedOfLight * 2, deltaY = 1;
//
//    FieldGrid fieldGrid = FieldGrid(n, m, deltaX, deltaY, Vector3::Zero, 1);
//
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < m; j++) {
//            fieldGrid.getNodeAt(i, j).E.y = sin(2 * Constants::PI * (i)*deltaX / ((n - 1) * deltaX));
//            fieldGrid.getNodeAt(i, j).B.z = sin(2 * Constants::PI * (i)*deltaX / ((n - 1) * deltaX));
//        }
//    }
//
//    FieldLoopEdgeCondition edgeCondition;
//    edgeCondition.updateBEdge(&fieldGrid);
//    edgeCondition.updateEEdge(&fieldGrid);
//
//    double deltaT = 1e-3;
//    int iterations = 10;
//    int step = 2;
//
//    for (int k = 1; k <= 50; k++) {
//        deltaT /= step;
//        iterations *= step;
//
//        // Defining particle grid
//        ParticleGrid particleGrid = ParticleGrid(n, m, deltaX, deltaY, Vector3::Zero, 1);
//
//        // Initializing core
//        PicadorJrCore core = PicadorJrCore(&fieldGrid, &particleGrid, deltaT, iterations);
//
//        // Field integrator module
//        FieldSolver fieldIntegrator(&core);
//
//        fieldIntegrator.onBegin();
//
//        for (int t = 0; t < iterations; t++) {
//            fieldIntegrator.onUpdate();
//        }
//
//        double absError = 0;
//        for (int i = 0; i < n; i++) absError += abs(fieldGrid.getNodeAt(i, 0).B.z - sin(2 * Constants::PI * (i * deltaX - Constants::SpeedOfLight * iterations * core.getTimeDelta()) / ((n - 1) * deltaX)));
//            //std::cout << iterations << " " << core.getTimeDelta() << " " << abs(fieldGrid.getNodeAt(i, 0).B.z - sin(2 * Constants::PI * (i * deltaX - Constants::SpeedOfLight * iterations * core.getTimeDelta()) / ((n - 1) * deltaX))) << "\n";
//        absError /= n;
//        std::cout << deltaT << ", " << absError << "\n";
//
//    }
//
//}


TEST(FieldSolver, edgeConditions)
{
    int n = 21, m = 21;
    double deltaX = 1e-3, deltaY = 1e-3;
    FieldGrid fieldGrid = FieldGrid(n, m, deltaX, deltaY, Vector3::Zero, 1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fieldGrid.getNodeAt(i, j).E.y = sin(2 * Constants::PI * (i) / ((n - 1) * deltaX));
            fieldGrid.getNodeAt(i, j).B.z = sin(2 * Constants::PI * (i) / ((n - 1) * deltaX));
        }
    }

    FieldLoopEdgeCondition edgeCondition;
    edgeCondition.updateBEdge(&fieldGrid);
    edgeCondition.updateEEdge(&fieldGrid);

    for (int j = 0; j < (GRID_INDEX)fieldGrid.getResolutionY(); j++) EXPECT_EQ(fieldGrid.getNodeAt(-1, j).E.y, fieldGrid.getNodeAt((GRID_INDEX)fieldGrid.getResolutionX() - 1, j).E.y);
    for (int j = 0; j < (GRID_INDEX)fieldGrid.getResolutionY(); j++) EXPECT_EQ(fieldGrid.getNodeAt(0, j).E.y, fieldGrid.getNodeAt((GRID_INDEX)fieldGrid.getResolutionX(), j).E.y);
    for (int i = 0; i < (GRID_INDEX)fieldGrid.getResolutionX(); i++) EXPECT_EQ(fieldGrid.getNodeAt(i, -1).E.y, fieldGrid.getNodeAt(i,(GRID_INDEX)fieldGrid.getResolutionY() - 1).E.y);
    for (int i = 0; i < (GRID_INDEX)fieldGrid.getResolutionX(); i++) EXPECT_EQ(fieldGrid.getNodeAt(i, 0).E.y, fieldGrid.getNodeAt(i, (GRID_INDEX)fieldGrid.getResolutionY()).E.y);
}
