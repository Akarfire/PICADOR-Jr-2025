#include "ParticleGrid.h"

#include <gtest.h>
#include <map>

TEST(ParticleGrid, canCreatePrimitiveGrid)
{
    ASSERT_NO_THROW(ParticleGrid(1, 1, 1.0, 1.0, Vector3::Zero, 1));
}

TEST(ParticleGrid, throwsForInvalidLocation)
{
	ParticleGrid particleGrid(2, 2, 1.0, 1.0, Vector3::Zero, 1);

	ASSERT_ANY_THROW(particleGrid.getCell(Vector3(2.0, 2.0, 2.0)));
	ASSERT_ANY_THROW(particleGrid.getCell(Vector3(-1.0, 0.5, 0.0)));
}

TEST(ParticleGrid, recalculateCellIndexWorksForMainGrid)
{
    ParticleGrid particleGrid(5, 5, 1.0, 1.0, Vector3::Zero, 0);

    std::vector<std::vector<int>> expectedMap = 
    {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {8, 9, 10, 11},
        {12, 13, 14, 15}
    };

    //  12  13  14  15
    //  8   9   10  11
    //  4   5   6   7
    //  0   1   2   3

    for(size_t i = 0; i < 4; i++)
        for(size_t j = 0; j < 4; j++)
        {
            EXPECT_EQ(expectedMap[i][j], particleGrid.recalculateCellIndex(i, j));
        }
}

TEST(ParticleGrid, recalculateCellIndexWorksForPadding)
{
    ParticleGrid particleGrid(4, 4, 1.0, 1.0, Vector3::Zero, 1);

    //  6   7   8
    //  3   4   5  
    //  0   1   2

    std::map<std::pair<int, int>, int> expectedMap = 
    {
        {{-1, -1}, 0},
        {{-1, 0}, 3},
        {{-1, 1}, 6},

        {{0, -1}, 1},
        {{1, -1}, 2},

        {{1, 1}, 8},
        {{1, 0}, 5},
        {{0, 1}, 7},

        {{0, 0}, 4}
    };

    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
        {
            EXPECT_EQ(expectedMap.at({j, i}), particleGrid.recalculateCellIndex(i, j));
        }
}

TEST(ParticleGrid, particlesCanBeAddedToCells)
{
    ParticleGrid particleGrid(4, 4, 1.0, 1.0, Vector3::Zero, 1);

    // Adding particles into the grid
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
            for (int p = 0; p < 10 + i + j; p++)
            {
                ASSERT_NO_THROW(particleGrid.editParticlesInCell(i, j).push_back(Particle(1, 1, Vector3(particleGrid.getDeltaX() * j, particleGrid.getDeltaY() * i), Vector3::Zero)));
            }
}

TEST(ParticleGrid, particlesAreStoredInCells)
{
    ParticleGrid particleGrid(4, 4, 1.0, 1.0, Vector3::Zero, 1);

    // Adding particles into the grid
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
            for (int p = 0; p < 10 + i + j; p++)
            {
                ASSERT_NO_THROW(particleGrid.editParticlesInCell(i, j).push_back(Particle(1, 1, Vector3(particleGrid.getDeltaX() * j, particleGrid.getDeltaY() * i, p), Vector3::Zero)));
            }

    // Testing if particles are stored correctly
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
        {
            EXPECT_EQ(10 + i + j, particleGrid.getParticlesInCell(i, j).size());
            
            for (int p = 0; p < 10 + i + j; p++)
            {
                Vector3 deltaLocation = Vector3(particleGrid.getDeltaX() * j, particleGrid.getDeltaY() * i, p) - particleGrid.getParticlesInCell(i, j)[p].location;

                EXPECT_DOUBLE_EQ(0, deltaLocation.x);
                EXPECT_DOUBLE_EQ(0, deltaLocation.y);
                EXPECT_DOUBLE_EQ(0, deltaLocation.z);
            }
        }
}

TEST(ParticleGrid, canTransferParticleToAnotherCell)
{
    for (int i_o = -1; i_o < 2; i_o++)
        for (int j_o = -1; j_o < 2; j_o++)
            for (int i_r = -1; i_r < 2; i_r++)
                for (int j_r = -1; j_r < 2; j_r++)
                    if (! (i_o == i_r && j_o == j_r))
                    {
                        ParticleGrid particleGrid(4, 4, 1.0, 1.0, Vector3::Zero, 1);

                        // Adding original particle
                        ASSERT_NO_THROW(particleGrid.editParticlesInCell(i_o, j_o).push_back(Particle(1, 1, Vector3::Zero, Vector3::Zero)));

                        // Checking if original particle is in the specified cell
                        ASSERT_EQ(1, particleGrid.getParticlesInCell(i_o, j_o).size());

                        // Transfering particle to a neighbouring cell
                        ASSERT_NO_THROW(particleGrid.particleCellTransfer(0, i_o, j_o, i_r, j_r));

                        // Checking if transfer was correct
                        EXPECT_EQ(0, particleGrid.getParticlesInCell(i_o, j_o).size());
                        EXPECT_EQ(1, particleGrid.getParticlesInCell(i_r, j_r).size());
                    }
}