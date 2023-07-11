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
#include "common/debug/panic.hpp"
#include "io/parsers/map_parser.hpp"
#include "model/contexts/context_components.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_factory.hpp"
#include "model/entity_validation.hpp"
#include "model/maps/map_system.hpp"
#include "model/persistence/settings.hpp"
#include "model/tilesets/tileset_system.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _is_active_document(const Model& model, const DocumentType type)
    -> bool
{
  const auto& document_context = model.get<DocumentContext>();

  if (document_context.active_document != kNullEntity) {
    const auto& document = model.get<Document>(document_context.active_document);
    return document.type == type;
  }

  return false;
}

}  // namespace

void destroy_document(Model& model, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));
  auto& document_context = model.get<DocumentContext>();

  if (document_context.active_document == document_entity) {
    document_context.active_document = kNullEntity;
  }

  close_document(model, document_entity);

  model.destroy(document_entity);
}

void select_document(Model& model, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));

  auto& document_context = model.get<DocumentContext>();
  TACTILE_ASSERT(document_context.open_documents.contains(document_entity));

  document_context.active_document = document_entity;
}

void open_document(Model& model, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));

  auto& document_context = model.get<DocumentContext>();
  document_context.open_documents.insert(document_entity);
  document_context.active_document = document_entity;
}

// TODO this function should perhaps check if the document is active as well (and select another document)
void close_document(Model& model, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));

  auto& document_context = model.get<DocumentContext>();

  TACTILE_ASSERT(document_context.open_documents.contains(document_entity));
  document_context.open_documents.erase(document_entity);

  if (document_context.active_document == document_entity) {
    document_context.active_document = kNullEntity;
  }

  if (document_context.active_document == kNullEntity &&
      !document_context.open_documents.empty()) {
    document_context.active_document = *document_context.open_documents.begin();
  }
}

void set_document_name(Model& model, const Entity document_entity, String name)
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));

  if (const auto* map_document = model.try_get<MapDocument>(document_entity)) {
    auto& map_context = model.get<Context>(map_document->map);
    map_context.name = std::move(name);
  }
  else if (const auto* tileset_document =
               model.try_get<TilesetDocument>(document_entity)) {
    auto& tileset_context = model.get<Context>(tileset_document->tileset);
    tileset_context.name = std::move(name);
  }
  else {
    throw TactileError {"Invalid document type"};
  }
}

auto get_document_name(const Model& model, const Entity document_entity) -> String
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));

  if (const auto* map_document = model.try_get<MapDocument>(document_entity)) {
    const auto& map_context = model.get<Context>(map_document->map);
    return map_context.name;
  }
  else if (const auto* tileset_document =
               model.try_get<TilesetDocument>(document_entity)) {
    const auto& tileset_context = model.get<Context>(tileset_document->tileset);
    return tileset_context.name;
  }
  else {
    throw TactileError {"Invalid document type"};
  }
}

auto get_active_document(const Model& model) -> Entity
{
  const auto& document_context = model.get<DocumentContext>();
  return document_context.active_document;
}

auto get_active_context(const Model& model) -> Entity
{
  const auto document_entity = get_active_document(model);

  if (document_entity != kNullEntity) {
    const auto& document = model.get<Document>(document_entity);
    return document.active_context;
  }

  return kNullEntity;
}

auto get_active_map(const Model& model) -> Entity
{
  const auto document_entity = get_active_document(model);

  if (document_entity != kNullEntity && model.has<MapDocument>(document_entity)) {
    const auto& map_document = model.get<MapDocument>(document_entity);
    return map_document.map;
  }

  return kNullEntity;
}

auto try_get_active_map(const Model& model) -> const Map*
{
  const auto document_entity = get_active_document(model);

  if (document_entity != kNullEntity && model.has<MapDocument>(document_entity)) {
    const auto& map_document = model.get<MapDocument>(document_entity);
    return model.try_get<Map>(map_document.map);
  }

  return nullptr;
}

auto get_associated_tileset_document(const Model& model, const Entity tileset_entity)
    -> Entity
{
  TACTILE_ASSERT(is_tileset_entity(model, tileset_entity));

  // TODO add each version that ignores disabled entities
  for (const auto& [document_entity, tileset_document]: model.each<TilesetDocument>()) {
    if (tileset_document.tileset == tileset_entity) {
      return document_entity;
    }
  }

  return kNullEntity;
}

auto has_document_with_path(const Model& model, const Path& path) -> bool
{
  return get_document_with_path(model, path) != kNullEntity;
}

auto get_document_with_path(const Model& model, const Path& path) -> Entity
{
  for (const auto& [document_entity, document]: model.each<Document>()) {
    if (document.path == path) {
      return document_entity;
    }
  }

  return kNullEntity;
}

auto has_active_document(const Model& model) -> bool
{
  const auto& document_context = model.get<DocumentContext>();
  return document_context.active_document != kNullEntity;
}

auto is_map_document_active(const Model& model) -> bool
{
  return _is_active_document(model, DocumentType::Map);
}

auto is_tileset_document_active(const Model& model) -> bool
{
  return _is_active_document(model, DocumentType::Tileset);
}

auto is_document_open(const Model& model, const Entity document_entity) -> bool
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));

  const auto& document_context = model.get<DocumentContext>();
  return document_context.open_documents.contains(document_entity);
}

void on_create_map(Model& model, const CreateMapEvent& event)
{
  const TileExtent extent {event.row_count, event.column_count};
  const auto document_entity = create_map_document(model, extent, event.tile_size);
  open_document(model, document_entity);
}

}  // namespace tactile::sys
