#pragma once

#include <string>

class IVariable
{

  public:
    IVariable(const std::string_view name)
        : name(name) {

          };

  private:
    std::string name;
};

template <typename T> class Variable : public IVariable
{
  public:
    Variable(const std::string_view name) : IVariable(name) {}
};