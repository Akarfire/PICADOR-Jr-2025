#include "FieldGrid.h"

#include <gtest.h>

TEST(FieldGrid, canCreatePrimitiveGrid)
{
  ASSERT_NO_THROW(FieldGrid(1, 1, 1.0, 1.0, Vector3::Zero, 1));
}

TEST(FieldGrid, throwsForInvalidLocation)
{
	FieldGrid field(2, 2, 1.0, 1.0, Vector3::Zero, 0);

	ASSERT_ANY_THROW(field.getCell(Vector3(2.0, 2.0, 2.0)));
	ASSERT_ANY_THROW(field.getCell(Vector3(-1.0, 0.5, 0.0)));
}

TEST(FieldGrid, doesntThrowForCorrectLocation)
{
	FieldGrid field(2, 2, 1.0, 1.0, Vector3::Zero, 1);

	ASSERT_NO_THROW(field.getCell(Vector3(0.5, 0.5, 0.0)));
	
}

TEST(FieldGrid, getCellReturnsCorrectCellInComplexGrid)
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 1);

	std::pair<GRID_INDEX, GRID_INDEX> ans = std::pair<GRID_INDEX, GRID_INDEX>(1, 0);
	std::pair<GRID_INDEX, GRID_INDEX> indices = field.getCell(Vector3(1.5, 2.5, 0.0));

	EXPECT_EQ(ans, indices);

}

TEST(FieldGrid, getCellReturnsCorrectCellForTheOrigin)
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 1);

	std::pair<GRID_INDEX, GRID_INDEX> ans = std::pair<GRID_INDEX, GRID_INDEX>(0, 0);
	std::pair<GRID_INDEX, GRID_INDEX> indices = field.getCell(field.getOrigin());

	EXPECT_EQ(ans, indices);

}

TEST(FieldGrid, getCellReturnsCorrectCellForTheResolution)
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 1);

	std::pair<GRID_INDEX, GRID_INDEX> ans = std::pair<GRID_INDEX, GRID_INDEX>(2, 2);
	std::pair<GRID_INDEX, GRID_INDEX> indices = field.getCell(Vector3(field.getDeltaX()*field.getResolutionX(), field.getDeltaY() * field.getResolutionY()));

	EXPECT_EQ(ans, indices);

}

TEST(FieldGrid, getNodeAtThrowsForIncorrectIndices)
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 0);

	ASSERT_ANY_THROW(field.getNodeAt(-1, -1));
	ASSERT_ANY_THROW(field.getNodeAt(3, 3));
}

TEST(FieldGrid, getNodeAtDoesntThrowForCorrectIndices)
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 1);

	ASSERT_NO_THROW(field.getNodeAt(1, 2));
}

TEST(FieldGrid, getNodeAtReturnsCorrectNode) 
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 1);

	for (int i = 0; i < field.getResolutionX(); i++) {
		for (int j = 0; j < field.getResolutionY(); j++) {
			field.getNodeAt(i, j).B = i;
			field.getNodeAt(i, j).E = j;
		}
	}


	FieldData ans;
	ans.B = 1;
	ans.E = 2;

	EXPECT_EQ(ans.B, field.getNodeAt(1, 2).B);
	EXPECT_EQ(ans.E, field.getNodeAt(1, 2).E);
	EXPECT_EQ(ans.J, field.getNodeAt(1, 2).J);
}

TEST(FieldGrid, getFieldsAtReturnsCorrectField)
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 1);

	for (int i = 0; i < field.getResolutionX(); i++) {
		for (int j = 0; j < field.getResolutionY(); j++) {
			field.getNodeAt(i, j).B = i;
			field.getNodeAt(i, j).E = j;
		}
	}

	FieldData ans;
	ans.B = 1.5;
	ans.E = 0.5;

	EXPECT_EQ(ans.B, field.getFieldsAt(Vector3(2.5, 1.5)).B);
	EXPECT_EQ(ans.E, field.getFieldsAt(Vector3(2.5, 1.5)).E);
	EXPECT_EQ(ans.J, field.getFieldsAt(Vector3(2.5, 1.5)).J);
}

TEST(FieldGrid, anotherInterpolationTest)
{
	FieldGrid field(3, 3, 1.0, 1.0, Vector3::One, 1);

	for (int i = 0; i < field.getResolutionX(); i++) {
		for (int j = 0; j < field.getResolutionY(); j++) {
			field.getNodeAt(i, j).B = i;
			field.getNodeAt(i, j).E = j;
		}
	}

	FieldData ans;
	ans.B = 1.1;
	ans.E = 0.5;

	EXPECT_EQ(ans.B, field.getFieldsAt(Vector3(2.1, 1.5)).B);
	EXPECT_EQ(ans.E, field.getFieldsAt(Vector3(2.1, 1.5)).E);
	EXPECT_EQ(ans.J, field.getFieldsAt(Vector3(2.1, 1.5)).J);
}