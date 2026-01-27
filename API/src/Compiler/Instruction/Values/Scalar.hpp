#pragma once
#include "Value.hpp"
#include <format>
// using Scalar = float;

struct Scalar : Value<float>
{
    float value;
    Scalar(float f) : value(f) {}
    std::string EmitDOT() const override
    {
        return std::format(R"([label={}, shape=ellipse])", value);
    }
};