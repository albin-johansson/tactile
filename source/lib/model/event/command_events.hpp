// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"

namespace tactile {

struct UndoEvent final {};

struct RedoEvent final {};

struct SetCommandCapacityEvent final {
  usize capacity {};
};

}  // namespace tactile
