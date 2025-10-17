#include "FieldGrid.h"

#include <gtest.h>

TEST(FieldGrid, canCreatePrimitiveGrid)
{
  ASSERT_NO_THROW(FieldGrid(1, 1, 1.0, 1.0, Vector3::Zero, 1));
}

TEST(FieldGrid, throwsForInvalidLocation)
{
	FieldGrid field(2, 2, 1.0, 1.0, Vector3::Zero, 1);

	ASSERT_ANY_THROW(field.getCell(Vector3(2.0, 2.0, 2.0)));
	ASSERT_ANY_THROW(field.getCell(Vector3(-1.0, 0.5, 0.0)));
}

TEST(FieldGrid, doesntThrowForCorrectLocation)
{
	FieldGrid field(2, 2, 1.0, 1.0, Vector3::Zero, 1);

	ASSERT_NO_THROW(field.getCell(Vector3(0.5, 0.5, 0.0)));
	
}

TEST(FieldGrid, getCellReturnCorrectCellInComplexGrid)
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 1);

	std::pair<GRID_INDEX, GRID_INDEX> ans = std::pair<GRID_INDEX, GRID_INDEX>(0, 1);
	std::pair<GRID_INDEX, GRID_INDEX> indices = field.getCell(Vector3(1.5, 2.5, 0.0));

	EXPECT_EQ(ans, indices);

}

TEST(FieldGrid, getCellReturnCorrectCellForTheOrigin)
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 1);

	std::pair<GRID_INDEX, GRID_INDEX> ans = std::pair<GRID_INDEX, GRID_INDEX>(0, 0);
	std::pair<GRID_INDEX, GRID_INDEX> indices = field.getCell(field.getOrigin());

	EXPECT_EQ(ans, indices);

}

TEST(FieldGrid, getCellReturnCorrectCellForTheResolution)
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 1);

	std::pair<GRID_INDEX, GRID_INDEX> ans = std::pair<GRID_INDEX, GRID_INDEX>(2, 2);
	std::pair<GRID_INDEX, GRID_INDEX> indices = field.getCell(Vector3(field.getDeltaX()*field.getResolutionX(), field.getDeltaY() * field.getResolutionY()));

	EXPECT_EQ(ans, indices);

}