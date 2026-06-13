#pragma once

#include "Instruction/Values/Scalar.hpp"
#include "glm/glm.hpp"
#include <cstddef>

template <size_t L, typename T> using Vector = glm::vec<L, T>;
using Vector2 = Vector<2, Scalar>;
using Vector3 = Vector<3, Scalar>;
using Vector4 = Vector<4, Scalar>;