#include "Constants.h"

#include "PicadorJrCore.h"
#include "StaticField.h"
#include "ParticleGrid.h"
#include "Module_ParticleLoopEdgeCondition.h"

#include <gtest.h>

TEST(ParticleLoopEdgeCondition, loopingWorksNominallyForSingleParticle)
{
    // Defining field

    FieldData staticFieldData;
    staticFieldData.E = Vector3::Zero;
    staticFieldData.B = Vector3::Zero;
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    // Defining test particle

    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3::Zero, Vector3::Zero);

    // Various cases

    // Bottom edge (no corners)
    for (int i = 0 ; i <= 2; i++)
    {
        ParticleGrid particleGrid(4, 4, 1, 1, Vector3(-0.5, -0.5), 1);
        testParticle.location = Vector3(i, -1);
    
        particleGrid.editParticlesInCell(i, -1).push_back(testParticle);

        PicadorJrCore core(&staticField, &particleGrid, 1, 1);
        ParticleLoopEdgeCondition loopEdgeCondition(&core);
        core.insertModule(&loopEdgeCondition);
        core.run();

        EXPECT_EQ(0, particleGrid.getParticlesInCell(i, -1).size());
        ASSERT_EQ(1, particleGrid.getParticlesInCell(i, 2).size());
        EXPECT_EQ(Vector3(i, 2), particleGrid.getParticlesInCell(i, 2)[0].location);
    }

    // Top edge (no corners)
    for (int i = 0 ; i <= 2; i++)
    {
        ParticleGrid particleGrid(4, 4, 1, 1, Vector3(-0.5, -0.5), 1);
        testParticle.location = Vector3(i, 3);
    
        particleGrid.editParticlesInCell(i, 3).push_back(testParticle);

        PicadorJrCore core(&staticField, &particleGrid, 1, 1);
        ParticleLoopEdgeCondition loopEdgeCondition(&core);
        core.insertModule(&loopEdgeCondition);
        core.run();

        EXPECT_EQ(0, particleGrid.getParticlesInCell(i, 3).size());
        ASSERT_EQ(1, particleGrid.getParticlesInCell(i, 0).size());
        EXPECT_EQ(Vector3(i, 0), particleGrid.getParticlesInCell(i, 0)[0].location);
    }

    // Left edge (no corners)
    for (int j = 0 ; j <= 2; j++)
    {
        ParticleGrid particleGrid(4, 4, 1, 1, Vector3(-0.5, -0.5), 1);
        testParticle.location = Vector3(-1, j);
    
        particleGrid.editParticlesInCell(-1, j).push_back(testParticle);

        PicadorJrCore core(&staticField, &particleGrid, 1, 1);
        ParticleLoopEdgeCondition loopEdgeCondition(&core);
        core.insertModule(&loopEdgeCondition);
        core.run();

        EXPECT_EQ(0, particleGrid.getParticlesInCell(-1, j).size());
        ASSERT_EQ(1, particleGrid.getParticlesInCell(2, j).size());
        EXPECT_EQ(Vector3(2, j), particleGrid.getParticlesInCell(2, j)[0].location);
    }

    // Right edge (no corners)
    for (int j = 0 ; j <= 2; j++)
    {
        ParticleGrid particleGrid(4, 4, 1, 1, Vector3(-0.5, -0.5), 1);
        testParticle.location = Vector3(3, j);
    
        particleGrid.editParticlesInCell(3, j).push_back(testParticle);

        PicadorJrCore core(&staticField, &particleGrid, 1, 1);
        ParticleLoopEdgeCondition loopEdgeCondition(&core);
        core.insertModule(&loopEdgeCondition);
        core.run();

        EXPECT_EQ(0, particleGrid.getParticlesInCell(3, j).size());
        ASSERT_EQ(1, particleGrid.getParticlesInCell(0, j).size());
        EXPECT_EQ(Vector3(0, j), particleGrid.getParticlesInCell(0, j)[0].location);
    }

    // Bottom Left corner
    {
        ParticleGrid particleGrid(4, 4, 1, 1, Vector3(-0.5, -0.5), 1);
        testParticle.location = Vector3(-1, -1);

        particleGrid.editParticlesInCell(-1, -1).push_back(testParticle);

        PicadorJrCore core(&staticField, &particleGrid, 1, 1);
        ParticleLoopEdgeCondition loopEdgeCondition(&core);
        core.insertModule(&loopEdgeCondition);
        core.run();

        EXPECT_EQ(0, particleGrid.getParticlesInCell(-1, -1).size());
        ASSERT_EQ(1, particleGrid.getParticlesInCell(2, 2).size());
        EXPECT_EQ(Vector3(2, 2), particleGrid.getParticlesInCell(2, 2)[0].location);
    }

    // Bottom Right corner
    {    
        ParticleGrid particleGrid(4, 4, 1, 1, Vector3(-0.5, -0.5), 1);
        testParticle.location = Vector3(3, -1);

        particleGrid.editParticlesInCell(3, -1).push_back(testParticle);

        PicadorJrCore core(&staticField, &particleGrid, 1, 1);
        ParticleLoopEdgeCondition loopEdgeCondition(&core);
        core.insertModule(&loopEdgeCondition);
        core.run();

        EXPECT_EQ(0, particleGrid.getParticlesInCell(3, -1).size());
        ASSERT_EQ(1, particleGrid.getParticlesInCell(0, 2).size());
        EXPECT_EQ(Vector3(0, 2), particleGrid.getParticlesInCell(0, 2)[0].location);
    }

    // Top Right corner
    {    
        ParticleGrid particleGrid(4, 4, 1, 1, Vector3(-0.5, -0.5), 1);
        testParticle.location = Vector3(3, 3);

        particleGrid.editParticlesInCell(3, 3).push_back(testParticle);

        PicadorJrCore core(&staticField, &particleGrid, 1, 1);
        ParticleLoopEdgeCondition loopEdgeCondition(&core);
        core.insertModule(&loopEdgeCondition);
        core.run();

        EXPECT_EQ(0, particleGrid.getParticlesInCell(3, 3).size());
        ASSERT_EQ(1, particleGrid.getParticlesInCell(0, 0).size());
        EXPECT_EQ(Vector3(0, 0), particleGrid.getParticlesInCell(0, 0)[0].location);
    }

    // Top Left corner
    {    
        ParticleGrid particleGrid(4, 4, 1, 1, Vector3(-0.5, -0.5), 1);
        testParticle.location = Vector3(-1, 3);

        particleGrid.editParticlesInCell(-1, 3).push_back(testParticle);

        PicadorJrCore core(&staticField, &particleGrid, 1, 1);
        ParticleLoopEdgeCondition loopEdgeCondition(&core);
        core.insertModule(&loopEdgeCondition);
        core.run();

        EXPECT_EQ(0, particleGrid.getParticlesInCell(-1, 3).size());
        ASSERT_EQ(1, particleGrid.getParticlesInCell(2, 0).size());
        EXPECT_EQ(Vector3(2, 0), particleGrid.getParticlesInCell(2, 0)[0].location);
    }
}

TEST(ParticleLoopEdgeCondition, loopingWorksNominallyForMultipleCells)
{
    // Defining field

    FieldData staticFieldData;
    staticFieldData.E = Vector3::Zero;
    staticFieldData.B = Vector3::Zero;
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    // Defining test particle

    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3::Zero, Vector3::Zero);

    ParticleGrid particleGrid(4, 4, 1, 1, Vector3(-0.5, -0.5), 1);

    // Filling particle grid edges with particles
    for (int i = -1; i <= 3; i++)
    {
        testParticle.location = Vector3(i, -1);
        particleGrid.editParticlesInCell(i, -1).push_back(testParticle);

        testParticle.location = Vector3(i, 3);
        particleGrid.editParticlesInCell(i, 3).push_back(testParticle);
    }
    
    for (int j = 0; j <= 2; j++)
    {
        testParticle.location = Vector3(-1, j);
        particleGrid.editParticlesInCell(-1, j).push_back(testParticle);

        testParticle.location = Vector3(3, j);
        particleGrid.editParticlesInCell(3, j).push_back(testParticle);
    }

    // Running
    PicadorJrCore core(&staticField, &particleGrid, 1, 1);
    ParticleLoopEdgeCondition loopEdgeCondition(&core);
    core.insertModule(&loopEdgeCondition);
    core.run();
    
    // Checking results

    // All padding cells should be empty
    for (int i = -1; i <= 3; i++)
    {
        EXPECT_EQ(0, particleGrid.getParticlesInCell(i, -1).size());
        EXPECT_EQ(0, particleGrid.getParticlesInCell(i, 3).size());
    }
    
    for (int j = 0; j <= 2; j++)
    {
        EXPECT_EQ(0, particleGrid.getParticlesInCell(-1, j).size());
        EXPECT_EQ(0, particleGrid.getParticlesInCell(3, j).size());
    }

    // Checking main grid cells
    for (int i = 0; i <= 2; i++)
        for (int j = 0; j <= 2; j++)
        {
            // Checking number of particles in each cell

            if ( (i == j && i != 1) || (i == 0 && j == 2) || (i == 2 && j == 0) )
                EXPECT_EQ(3, particleGrid.getParticlesInCell(i, j).size());
            
            else if (!(i == 1 && j == 1))
                EXPECT_EQ(1, particleGrid.getParticlesInCell(i, j).size());

            else
                EXPECT_EQ(0, particleGrid.getParticlesInCell(i, j).size());

            // Checking particle locations
            for (const auto& particle : particleGrid.getParticlesInCell(i, j))
            {
                EXPECT_DOUBLE_EQ(j, particle.location.y);
                EXPECT_DOUBLE_EQ(i, particle.location.x);
            }
        }
}

TEST(ParticleLoopEdgeCondition, loopingWorksNominallyForMultipleCellsWithMultipleParticles)
{
    // Defining field

    FieldData staticFieldData;
    staticFieldData.E = Vector3::Zero;
    staticFieldData.B = Vector3::Zero;
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    // Defining test particle

    Particle testParticle(Constants::ElectronMass, Constants::ElectronCharge, Vector3::Zero, Vector3::Zero);

    ParticleGrid particleGrid(4, 4, 1, 1, Vector3(-0.5, -0.5), 1);

    // Filling particle grid edges with particles
    for (int i = -1; i <= 3; i++)
    {
        testParticle.location = Vector3(i, -1);
        particleGrid.editParticlesInCell(i, -1).push_back(testParticle);
         particleGrid.editParticlesInCell(i, -1).push_back(testParticle);

        testParticle.location = Vector3(i, 3);
        particleGrid.editParticlesInCell(i, 3).push_back(testParticle);
        particleGrid.editParticlesInCell(i, 3).push_back(testParticle);
    }
    
    for (int j = 0; j <= 2; j++)
    {
        testParticle.location = Vector3(-1, j);
        particleGrid.editParticlesInCell(-1, j).push_back(testParticle);
        particleGrid.editParticlesInCell(-1, j).push_back(testParticle);

        testParticle.location = Vector3(3, j);
        particleGrid.editParticlesInCell(3, j).push_back(testParticle);
        particleGrid.editParticlesInCell(3, j).push_back(testParticle);
    }

    // Running
    PicadorJrCore core(&staticField, &particleGrid, 1, 1);
    ParticleLoopEdgeCondition loopEdgeCondition(&core);
    core.insertModule(&loopEdgeCondition);
    core.run();
    
    // Checking results

    // All padding cells should be empty
    for (int i = -1; i <= 3; i++)
    {
        EXPECT_EQ(0, particleGrid.getParticlesInCell(i, -1).size());
        EXPECT_EQ(0, particleGrid.getParticlesInCell(i, 3).size());
    }
    
    for (int j = 0; j <= 2; j++)
    {
        EXPECT_EQ(0, particleGrid.getParticlesInCell(-1, j).size());
        EXPECT_EQ(0, particleGrid.getParticlesInCell(3, j).size());
    }

    // Checking main grid cells
    for (int i = 0; i <= 2; i++)
        for (int j = 0; j <= 2; j++)
        {
            // Checking number of particles in each cell

            if ( (i == j && i != 1) || (i == 0 && j == 2) || (i == 2 && j == 0) )
                EXPECT_EQ(6, particleGrid.getParticlesInCell(i, j).size());
            
            else if (!(i == 1 && j == 1))
                EXPECT_EQ(2, particleGrid.getParticlesInCell(i, j).size());

            else
                EXPECT_EQ(0, particleGrid.getParticlesInCell(i, j).size());

            // Checking particle locations
            for (const auto& particle : particleGrid.getParticlesInCell(i, j))
            {
                EXPECT_DOUBLE_EQ(j, particle.location.y);
                EXPECT_DOUBLE_EQ(i, particle.location.x);
            }
        }
}