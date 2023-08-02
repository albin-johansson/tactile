/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "document_system.hpp"

#include <utility>  // move

#include <spdlog/spdlog.h>

#include "cmd/command_stack.hpp"
#include "core/debug/panic.hpp"
#include "io/parsers/map_parser.hpp"
#include "model/contexts/context_components.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_factory.hpp"
#include "model/entity_validation.hpp"
#include "model/maps/map_system.hpp"
#include "model/services/settings/settings.hpp"
#include "model/tilesets/tileset_system.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _is_active_document(const Registry& registry, const DocumentType type)
    -> bool
{
  const auto& document_context = registry.get<DocumentContext>();

  if (document_context.active_document != kNullEntity) {
    const auto& document = registry.get<Document>(document_context.active_document);
    return document.type == type;
  }

  return false;
}

}  // namespace

void destroy_document(Registry& registry, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(registry, document_entity));
  auto& document_context = registry.get<DocumentContext>();

  if (document_context.active_document == document_entity) {
    document_context.active_document = kNullEntity;
  }

  close_document(registry, document_entity);

  registry.destroy(document_entity);
}

void select_document(Registry& registry, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(registry, document_entity));

  auto& document_context = registry.get<DocumentContext>();
  TACTILE_ASSERT(contained_in(document_context.open_documents, document_entity));

  document_context.active_document = document_entity;
}

void open_document(Registry& registry, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(registry, document_entity));

  auto& document_context = registry.get<DocumentContext>();
  document_context.open_documents.push_back(document_entity);
  document_context.active_document = document_entity;
}

// TODO this function should perhaps check if the document is active as well (and select
// another document)
void close_document(Registry& registry, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(registry, document_entity));

  auto& document_context = registry.get<DocumentContext>();

  TACTILE_ASSERT(contained_in(document_context.open_documents, document_entity));
  std::erase(document_context.open_documents, document_entity);

  if (document_context.active_document == document_entity) {
    document_context.active_document = kNullEntity;
  }

  if (document_context.active_document == kNullEntity &&
      !document_context.open_documents.empty()) {
    document_context.active_document = *document_context.open_documents.begin();
  }
}

void set_document_name(Registry& registry, const Entity document_entity, String name)
{
  TACTILE_ASSERT(is_document_entity(registry, document_entity));

  if (const auto* map_document = registry.try_get<MapDocument>(document_entity)) {
    auto& map_context = registry.get<Context>(map_document->map);
    map_context.name = std::move(name);
  }
  else if (const auto* tileset_document =
               registry.try_get<TilesetDocument>(document_entity)) {
    auto& tileset_context = registry.get<Context>(tileset_document->tileset);
    tileset_context.name = std::move(name);
  }
  else {
    throw TactileError {"Invalid document type"};
  }
}

auto get_document_name(const Registry& registry, const Entity document_entity) -> String
{
  TACTILE_ASSERT(is_document_entity(registry, document_entity));

  if (const auto* map_document = registry.try_get<MapDocument>(document_entity)) {
    const auto& map_context = registry.get<Context>(map_document->map);
    return map_context.name;
  }
  else if (const auto* tileset_document =
               registry.try_get<TilesetDocument>(document_entity)) {
    const auto& tileset_context = registry.get<Context>(tileset_document->tileset);
    return tileset_context.name;
  }
  else {
    throw TactileError {"Invalid document type"};
  }
}

auto get_active_document(const Registry& registry) -> Entity
{
  const auto& document_context = registry.get<DocumentContext>();
  return document_context.active_document;
}

auto get_active_context(const Registry& registry) -> Entity
{
  const auto document_entity = get_active_document(registry);

  if (document_entity != kNullEntity) {
    const auto& document = registry.get<Document>(document_entity);
    return document.active_context;
  }

  return kNullEntity;
}

auto get_active_map(const Registry& registry) -> Entity
{
  const auto document_entity = get_active_document(registry);

  if (document_entity != kNullEntity && registry.has<MapDocument>(document_entity)) {
    const auto& map_document = registry.get<MapDocument>(document_entity);
    return map_document.map;
  }

  return kNullEntity;
}

auto try_get_active_map(const Registry& registry) -> const Map*
{
  const auto document_entity = get_active_document(registry);

  if (document_entity != kNullEntity && registry.has<MapDocument>(document_entity)) {
    const auto& map_document = registry.get<MapDocument>(document_entity);
    return registry.try_get<Map>(map_document.map);
  }

  return nullptr;
}

auto get_associated_tileset_document(const Registry& registry,
                                     const Entity tileset_entity) -> Entity
{
  TACTILE_ASSERT(is_tileset_entity(registry, tileset_entity));

  // TODO add each version that ignores disabled entities
  for (const auto& [document_entity, tileset_document]:
       registry.each<TilesetDocument>()) {
    if (tileset_document.tileset == tileset_entity) {
      return document_entity;
    }
  }

  return kNullEntity;
}

auto has_document_with_path(const Registry& registry, const Path& path) -> bool
{
  return get_document_with_path(registry, path) != kNullEntity;
}

auto get_document_with_path(const Registry& registry, const Path& path) -> Entity
{
  for (const auto& [document_entity, document]: registry.each<Document>()) {
    if (document.path == path) {
      return document_entity;
    }
  }

  return kNullEntity;
}

auto has_active_document(const Registry& registry) -> bool
{
  const auto& document_context = registry.get<DocumentContext>();
  return document_context.active_document != kNullEntity;
}

auto is_map_document_active(const Registry& registry) -> bool
{
  return _is_active_document(registry, DocumentType::Map);
}

auto is_tileset_document_active(const Registry& registry) -> bool
{
  return _is_active_document(registry, DocumentType::Tileset);
}

auto is_document_open(const Registry& registry, const Entity document_entity) -> bool
{
  TACTILE_ASSERT(is_document_entity(registry, document_entity));

  const auto& document_context = registry.get<DocumentContext>();
  return contained_in(document_context.open_documents, document_entity);
}

void on_create_map(Registry& registry, const CreateMapEvent& event)
{
  const TileExtent extent {event.row_count, event.column_count};
  const auto document_entity = create_map_document(registry, extent, event.tile_size);
  open_document(registry, document_entity);
}

}  // namespace tactile::sys
