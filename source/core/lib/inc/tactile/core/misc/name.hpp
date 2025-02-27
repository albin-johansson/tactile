// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/basic/string.hpp"

namespace tactile {

/// A component type that represents an arbitrary name.
struct TACTILE_CORE_API CName final
{
  String name;
};

}  // namespace tactile
