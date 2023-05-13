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

#include "cmd/command_stack.hpp"
#include "common/debug/panic.hpp"
#include "core/context.hpp"
#include "core/map.hpp"
#include "core/viewport.hpp"
#include "model/document.hpp"
#include "model/systems/map_system.hpp"
#include "model/systems/tileset_system.hpp"
#include "model/systems/validation.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _is_active_document(const Model& model, const DocumentType type)
    -> bool
{
  const auto& document_context = model.get<CDocumentContext>();

  if (document_context.active_document != kNullEntity) {
    const auto& document = model.get<Document>(document_context.active_document);
    return document.type == type;
  }

  return false;
}

}  // namespace

auto create_map_document(Model& model, const TileExtent& extent, const Int2& tile_size)
    -> Entity
{
  const auto document_entity = model.create_entity();
  model.add<CommandStack>(document_entity);

  auto& map_document = model.add<MapDocument>(document_entity);
  map_document.map = create_map(model, extent, tile_size);
  map_document.active_tileset = kNullEntity;

  auto& document = model.add<Document>(document_entity);
  document.type = DocumentType::Map;
  document.default_context = map_document.map;

  auto& document_viewport = model.add<Viewport>(document_entity);
  document_viewport.offset = Float2 {0, 0};
  document_viewport.tile_size = tile_size;

  TACTILE_ASSERT(is_map_document_entity(model, document_entity));
  return document_entity;
}

auto create_tileset_document(Model& model, const Int2& tile_size, const Path& image_path)
    -> Entity
{
  const auto document_entity = model.create_entity();
  model.add<CommandStack>(document_entity);

  auto& tileset_document = model.add<TilesetDocument>(document_entity);
  tileset_document.tileset = create_tileset(model, tile_size, image_path);

  auto& document = model.add<Document>(document_entity);
  document.type = DocumentType::Tileset;
  document.default_context = tileset_document.tileset;

  auto& document_viewport = model.add<Viewport>(document_entity);
  document_viewport.offset = Float2 {0, 0};
  document_viewport.tile_size = tile_size;

  TACTILE_ASSERT(is_tileset_document_entity(model, document_entity));
  return document_entity;
}

void destroy_document(Model& model, const DocumentEntity document_entity)
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));
  auto& document_context = model.get<CDocumentContext>();

  if (document_context.active_document == document_entity) {
    document_context.active_document = kNullEntity;
  }

  close_document(model, document_entity);

  model.destroy(document_entity);
}

void select_document(Model& model, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));

  auto& document_context = model.get<CDocumentContext>();
  TACTILE_ASSERT(document_context.open_documents.contains(document_entity));

  document_context.active_document = document_entity;
}

void open_document(Model& model, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));

  auto& document_context = model.get<CDocumentContext>();
  document_context.open_documents.insert(document_entity);
}

// TODO this function should perhaps check if the document is active as well
void close_document(Model& model, const Entity document_entity)
{
  TACTILE_ASSERT(is_document_entity(model, document_entity));

  auto& document_context = model.get<CDocumentContext>();
  document_context.open_documents.erase(document_entity);
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
  const auto& document_context = model.get<CDocumentContext>();
  return document_context.active_document;
}

auto get_associated_tileset_document(const Model& model, const Entity tileset_entity)
    -> Entity
{
  TACTILE_ASSERT(is_tileset_entity(model, tileset_entity));
  const auto& document_context = model.get<CDocumentContext>();

  for (const auto document_entity: document_context.open_documents) {
    if (const auto* tileset_document = model.try_get<TilesetDocument>(document_entity)) {
      if (tileset_document->tileset == tileset_entity) {
        return document_entity;
      }
    }
  }

  return kNullEntity;
}

auto has_active_document(const Model& model) -> bool
{
  const auto& document_context = model.get<CDocumentContext>();
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

  const auto& document_context = model.get<CDocumentContext>();
  return document_context.open_documents.contains(document_entity);
}

}  // namespace tactile::sys
