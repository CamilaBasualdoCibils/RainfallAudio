#pragma once

#include "NodeRegistry.hpp"
#define REGISTER_NODE(Type,Name,Icon) \
    static bool _auto_reg_##Type = NodeRegistry::Get().RegisterNode<Type>(Name,Icon)
