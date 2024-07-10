// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class IMapView;
class ILayerView;
class IObjectView;
class ITilesetView;
class ITileView;

class IDocumentVisitor
{
 public:
  TACTILE_INTERFACE_CLASS(IDocumentVisitor);

  virtual void visit(const IMapView& map) = 0;

  virtual void visit(const ILayerView& layer) = 0;

  virtual void visit(const IObjectView& object) = 0;

  virtual void visit(const ITilesetView& tileset) = 0;

  virtual void visit(const ITileView& tile) = 0;
};

}  // namespace tactile
