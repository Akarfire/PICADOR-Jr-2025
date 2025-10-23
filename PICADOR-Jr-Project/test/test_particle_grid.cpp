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

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            EXPECT_EQ(expectedMap[i][j], particleGrid.recalculateCellIndex(j, i));
        }
}

TEST(ParticleGrid, recalculateCellIndexWorksForPadding)
{
    ParticleGrid particleGrid(2, 2, 1.0, 1.0, Vector3::Zero, 1);

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
            EXPECT_EQ(expectedMap.at({i, j}), particleGrid.recalculateCellIndex(i, j));
        }
}