#pragma once
#include "Value.hpp"
#include <format>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Constant.h>
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

struct Matrix4x4 : Value<float>
{
};