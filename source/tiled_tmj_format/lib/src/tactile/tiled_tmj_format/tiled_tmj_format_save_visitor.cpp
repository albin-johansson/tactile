// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tiled_tmj_format_save_visitor.hpp"

#include <utility>     // move

namespace tactile {

auto TiledTmjFormatSaveVisitor::visit(const IMapView& map) -> Result<void>
{
  return kOK;
}

auto TiledTmjFormatSaveVisitor::visit(const ITilesetView& tileset) -> Result<void>
{
  return kOK;
}

auto TiledTmjFormatSaveVisitor::visit(const ITileView& tile) -> Result<void>
{
  return kOK;
}

auto TiledTmjFormatSaveVisitor::visit(const ILayerView& layer) -> Result<void>
{
  return kOK;
}

auto TiledTmjFormatSaveVisitor::visit(const IObjectView& object) -> Result<void>
{
  return kOK;
}

auto TiledTmjFormatSaveVisitor::visit(const IComponentView&) -> Result<void>
{
  return kOK;
}

void TiledTmjFormatSaveVisitor::set_options(SaveFormatWriteOptions options)
{
  mOptions = std::move(options);
}

}  // namespace tactile
