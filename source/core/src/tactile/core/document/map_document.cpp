// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/map_document.hpp"

#include <utility>  // move

#include "tactile/base/container/maybe.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/map/map_spec.hpp"
#include "tactile/core/tile/tileset.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile {

struct MapDocument::Data final  // NOLINT(*-member-init)
{
  UUID uuid;
  Registry registry;
  EntityID map_entity;
  Maybe<Path> path;
};

MapDocument::MapDocument(const MapSpec& spec)
  : mData {std::make_unique<Data>()}
{
  mData->uuid = UUID::generate();

  auto& registry = mData->registry;
  registry.add<CTileCache>();

  mData->map_entity = make_map(registry, spec);
  if (mData->map_entity == kInvalidEntity) {
    throw Exception {"could not create map"};
  }

  auto& document_info = registry.add<CDocumentInfo>();
  document_info.root = mData->map_entity;
  document_info.active_context = document_info.root;
  document_info.tile_size = spec.tile_size;
}

TACTILE_DEFINE_MOVE(MapDocument);

MapDocument::~MapDocument() noexcept = default;

void MapDocument::update()
{}

void MapDocument::set_path(Path path)
{
  mData->path = std::move(path);
}

auto MapDocument::get_path() const -> const Path*
{
  return mData->path.has_value() ? &mData->path.value() : nullptr;
}

auto MapDocument::get_registry() -> Registry&
{
  return mData->registry;
}

auto MapDocument::get_registry() const -> const Registry&
{
  return mData->registry;
}

auto MapDocument::get_content_size() const -> Float2
{
  const auto map_entity = mData->map_entity;

  const auto& map = mData->registry.get<CMap>(map_entity);
  const auto& viewport = mData->registry.get<CViewport>(map_entity);

  return Float2 {static_cast<float>(map.extent.cols),
                 static_cast<float>(map.extent.rows)} *
         vec_cast<Float2>(map.tile_size) * viewport.scale;
}

auto MapDocument::get_tile_size() const -> Int2
{
  const auto& map = mData->registry.get<CMap>(mData->map_entity);
  return map.tile_size;
}

auto MapDocument::get_uuid() const -> const UUID&
{
  return mData->uuid;
}

}  // namespace tactile
