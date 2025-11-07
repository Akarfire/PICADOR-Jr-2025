#pragma once

#include "FieldContainer.h"

class StaticField : public FieldContainer
{
    FieldData fieldValue;

public:
    StaticField(const FieldData& fieldValue_): FieldContainer(), fieldValue(fieldValue_) {}

    // Accesing field data at a given point in space
    virtual FieldData getFieldsAt(const Vector3& location) const override
    {
        return fieldValue;
    }
};