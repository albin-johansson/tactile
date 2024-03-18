// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "icons.hpp"

#include "core/texture.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::ui {
namespace {

inline Shared<Texture> gTactileIcon;

}  // namespace

void load_icons()
{
  gTactileIcon = load_texture("assets/icon.png");
}

void unload_icons() noexcept
{
  gTactileIcon.reset();
}

auto get_icon(const LayerType type) -> const char*
{
  switch (type) {
    case LayerType::TileLayer: return TAC_ICON_TILE_LAYER;

    case LayerType::ObjectLayer: return TAC_ICON_OBJECT_LAYER;

    case LayerType::GroupLayer: return TAC_ICON_GROUP_LAYER;

    default: throw Exception {"Invalid layer type!"};
  }
}

auto get_icon(const ObjectType type) -> const char*
{
  switch (type) {
    case ObjectType::Rect: return TAC_ICON_RECTANGLE;

    case ObjectType::Ellipse: return TAC_ICON_ELLIPSE;

    case ObjectType::Point: return TAC_ICON_POINT;

    default: throw Exception {"Invalid object type!"};
  }
}

auto get_tactile_icon() -> uint
{
  TACTILE_ASSERT(gTactileIcon != nullptr);
  return gTactileIcon->get_id();
}

}  // namespace tactile::ui
