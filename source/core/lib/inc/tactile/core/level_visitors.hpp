// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/basic/macros.hpp"

namespace tactile {

class TACTILE_CORE_API ILevelVisitor
{
 public:
  TACTILE_INTERFACE_CLASS(ILevelVisitor);
};

class TACTILE_CORE_API ILevelRootLayerVisitor
{
 public:
  TACTILE_INTERFACE_CLASS(ILevelRootLayerVisitor);
};

}  // namespace tactile
