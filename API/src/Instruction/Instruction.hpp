#pragma once

#include <Variable/Variable.hpp>

#include "boost/container/small_vector.hpp"
#include <memory>
#include <string_view>
#include <typeindex>
class IInstruction
{

    std::string Name;

  protected:
    template <typename T> void AddInput(const std::string_view Name)
    {
        std::shared_ptr<Variable<T>> var = std::make_shared<Variable<T>>(Name);
        inputs.push_back(var);
    }
    template <typename T> void AddOutput(const std::string_view Name)
    {
        std::shared_ptr<Variable<T>> var = std::make_shared<Variable<T>>(Name);
        outputs.push_back(var);
    }

  public:
    IInstruction(const std::string_view Name) : Name(Name) {}

  private:
    boost::container::small_vector<std::shared_ptr<IVariable>, 15> inputs;
    boost::container::small_vector<std::shared_ptr<IVariable>, 15> outputs;
};
template <typename T>
class Instruction
{

  public:
};