// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tiled_tmj_format_save_visitor.hpp"

#include <utility>     // move

namespace tactile {

void TiledTmjFormatSaveVisitor::visit(const IMapView& map)
{
}

void TiledTmjFormatSaveVisitor::visit(const ITilesetView& tileset)
{
}

void TiledTmjFormatSaveVisitor::visit(const ITileView& tile)
{
}

void TiledTmjFormatSaveVisitor::visit(const ILayerView& layer)
{}

void TiledTmjFormatSaveVisitor::visit(const IObjectView& object)
{
}

void TiledTmjFormatSaveVisitor::visit(const IComponentView& component)
{}

void TiledTmjFormatSaveVisitor::set_options(SaveFormatWriteOptions options)
{
  mOptions = std::move(options);
}

}  // namespace tactile
