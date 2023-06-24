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

#include "model/documents/document_factory.hpp"

#include <doctest/doctest.h>

#include "model/documents/document_components.hpp"
#include "model/entity_validation.hpp"
#include "model/maps/map_components.hpp"
#include "model/model_factory.hpp"
#include "model/textures/texture_components.hpp"
#include "model/tilesets/tileset_components.hpp"

using namespace tactile;

TEST_SUITE("DocumentFactory")
{
  TEST_CASE("create_map_document")
  {
    auto model = sys::create_model(BackendAPI::Null);

    const TileExtent extent {12, 34};
    const Int2 tile_size {32, 48};

    const auto document_entity = sys::create_map_document(model, extent, tile_size);
    REQUIRE(sys::is_map_document_entity(model, document_entity));

    const auto& document = model.get<Document>(document_entity);
    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    CHECK(document.type == DocumentType::Map);
    CHECK(document.component_set == kNullEntity);
    CHECK(document.default_context == map_document.map);
    CHECK(document.active_context == document.default_context);
    CHECK(!document.path.has_value());

    CHECK(map_document.active_tileset == kNullEntity);
    REQUIRE(sys::is_map_entity(model, map_document.map));

    CHECK(map.extent == extent);
    CHECK(map.tile_size == tile_size);
    CHECK(map.active_layer == kNullEntity);
    CHECK(map.active_tileset == kNullEntity);
    CHECK(map.attached_tilesets.empty());
    CHECK(sys::is_group_layer_entity(model, map.root_layer));
  }

  TEST_CASE("create_tileset_document")
  {
    auto model = sys::create_model(BackendAPI::Null);

    const Int2 tile_size {128, 96};
    const Path img_path = "resources/outside.png";
    const Int2 img_size {1'024, 1'024};

    const auto document_entity = sys::create_tileset_document(model, tile_size, img_path);
    REQUIRE(sys::is_tileset_document_entity(model, document_entity));

    const auto& document = model.get<Document>(document_entity);
    const auto& tileset_document = model.get<TilesetDocument>(document_entity);
    const auto& tileset = model.get<Tileset>(tileset_document.tileset);
    const auto& texture = model.get<Texture>(tileset.texture);

    REQUIRE(sys::is_tileset_entity(model, tileset_document.tileset));
    REQUIRE(sys::is_texture_entity(model, tileset.texture));

    CHECK(document.type == DocumentType::Tileset);
    CHECK(document.component_set == kNullEntity);
    CHECK(document.default_context == tileset_document.tileset);
    CHECK(document.active_context == document.default_context);
    CHECK(!document.path.has_value());

    CHECK(tileset.tile_size == tile_size);

    const auto tile_count = img_size / tile_size;
    CHECK(tileset.row_count == tile_count.y);
    CHECK(tileset.column_count == tile_count.x);
    CHECK(static_cast<usize>(tile_count.x * tile_count.y) == tileset.tiles.size());

    CHECK(tileset.tile_index_map.size() == tileset.tiles.size());
    CHECK(tileset.uv_size == Float2 {tileset.tile_size} / Float2 {img_size});
    CHECK(!tileset.selected_tile_index.has_value());

    CHECK(texture.size == img_size);
    CHECK(texture.path == img_path);
  }
}