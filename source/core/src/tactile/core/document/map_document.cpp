// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/map_document.hpp"

#include <system_error>  // make_error_code, errc
#include <utility>       // move

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/io/save/ir.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/document/map_view_impl.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/map/map_spec.hpp"
#include "tactile/core/tile/tileset.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile {

struct MapDocument::Data final
{
  UUID uuid;
  Registry registry;
  EntityID map_entity;
  Maybe<Path> path;

  Data()
    : uuid {UUID::generate()},
      registry {},
      map_entity {kInvalidEntity},
      path {kNone}
  {
    registry.add<CTileCache>();
    registry.add<CDocumentInfo>();
  }
};

MapDocument::MapDocument()
  : mData {std::make_unique<Data>()}
{}

auto MapDocument::make(const MapSpec& spec) -> Result<MapDocument>
{
  MapDocument document {};
  auto& registry = document.mData->registry;

  const auto map_id = make_map(registry, spec);
  if (map_id == kInvalidEntity) {
    TACTILE_LOG_ERROR("Could not create map document");
    return unexpected(std::make_error_code(std::errc::invalid_argument));
  }

  document.mData->map_entity = map_id;

  auto& document_info = registry.get<CDocumentInfo>();
  document_info.root = map_id;
  document_info.active_context = map_id;
  document_info.tile_size = spec.tile_size;

  return document;
}

auto MapDocument::make(IRenderer& renderer, const ir::Map& ir_map) -> Result<MapDocument>
{
  MapDocument document {};
  auto& registry = document.mData->registry;

  const auto map_id = make_map(registry, renderer, ir_map);
  if (!map_id.has_value()) {
    TACTILE_LOG_ERROR("Could not create map document: {}", map_id.error().message());
    return propagate_unexpected(map_id);
  }

  document.mData->map_entity = *map_id;
  const auto& map = registry.get<CMap>(*map_id);

  auto& document_info = registry.get<CDocumentInfo>();
  document_info.root = *map_id;
  document_info.active_context = *map_id;
  document_info.tile_size = map.tile_size;

  return document;
}

TACTILE_DEFINE_MOVE(MapDocument);

MapDocument::~MapDocument() noexcept = default;

void MapDocument::accept(IDocumentVisitor& visitor) const
{
  const MapViewImpl map_view {this};
  map_view.accept(visitor);
}

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

  return Float2 {static_cast<float>(map.extent.cols), static_cast<float>(map.extent.rows)} *
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
