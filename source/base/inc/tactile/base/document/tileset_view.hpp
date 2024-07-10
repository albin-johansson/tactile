// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class IDocumentVisitor;

class ITilesetView
{
 public:
  TACTILE_INTERFACE_CLASS(ITilesetView);

  virtual void accept(IDocumentVisitor& visitor) const = 0;
};

}  // namespace tactile
