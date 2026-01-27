#pragma once

#include <string>
struct IValue
{
    public:
virtual std::string EmitDOT() const = 0;
};
template <typename T>
struct Value: IValue{


};
