#pragma once

#include <string>

class IValue
{

  public:
    IValue(const std::string_view name)
        : name(name) {

          };

  private:
    std::string name;
};

template <typename T> class Value
{
  public:
    Value(const std::string_view name) : Value(name) {}
};