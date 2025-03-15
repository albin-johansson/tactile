// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/basic/macros.hpp"

namespace tactile {

class ILevelVisitor
{
 public:
  TACTILE_INTERFACE_CLASS(ILevelVisitor);
};

class ILevelRootLayerVisitor
{
 public:
  TACTILE_INTERFACE_CLASS(ILevelRootLayerVisitor);
};

}  // namespace tactile
