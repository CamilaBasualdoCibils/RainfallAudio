#pragma once

#include "Instruction/Value.hpp"
#include "Value.hpp"
#include "boost/container/container_fwd.hpp"
#include <memory>
#include <string_view>
class IInstruction
{

  protected:
    template <typename T> void AddInput(const std::string_view Name)
    {
        std::shared_ptr<Value<T>> value = std::make_shared<Value<T>>();
    }
    template <typename T> void AddOutput(const std::string_view Name) {}

    boost::container::small_vector<std::shared_ptr<IValue>, 15> inputs;
    boost::container::small_vector<std::shared_ptr<IValue>, 15> outputs;
};

template <typename InputTypes, typename OutputTypes> class Instruction
{

  public:
};