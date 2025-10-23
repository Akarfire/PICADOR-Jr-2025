#include <gtest.h>

#include "PicadorJrCore.h"
#include "Module_ParticleSolver.h"
#include "StaticField.h"

TEST(ParticleSolver, RelativisticAccelerationInStaticField)
{
    // Defining static field

    double EZero = 0.01;

    FieldData staticFieldData;
    staticFieldData.E = Vector3(EZero, 0, 0);
    staticFieldData.B = Vector3::Zero;
    staticFieldData.J = Vector3::Zero;

    StaticField staticField(staticFieldData);

    // Defining particle grid
    ParticleGrid particleGrid();


    //PicadorJrCore core(&staticField);
}