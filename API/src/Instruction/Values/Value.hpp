#pragma once

#include <string>

struct IValue
{

  public:
    IValue(const std::string_view name)
        : name(name) {

          };

  private:
    std::string name;
};

template <typename T> struct Value
{
  public:
    Value(const std::string_view name) : Value(name) {}
};