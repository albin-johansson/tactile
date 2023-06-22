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

#include "model/documents/document_system.hpp"

#include <doctest/doctest.h>

#include "model/components/document.hpp"
#include "model/components/texture.hpp"
#include "model/documents/document_factory.hpp"
#include "model/model.hpp"
#include "model/systems/model_system.hpp"
#include "model/systems/validation_system.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/tilesets/tileset_ops.hpp"

using namespace tactile;

TEST_SUITE("DocumentSystem")
{
  TEST_CASE("create_map_document")
  {
    Model model;
    sys::init_model(model, BackendAPI::Null);

    const TileExtent extent {12, 34};
    const Int2 tile_size {32, 48};

    const auto document_entity = sys::create_map_document(model, extent, tile_size);
    REQUIRE(sys::is_map_document_entity(model, document_entity));

    const auto& document = model.get<Document>(document_entity);
    CHECK(document.type == DocumentType::Map);
    CHECK(document.component_set == kNullEntity);
    CHECK(document.active_context == document.default_context);
    CHECK(!document.path.has_value());

    const auto& map_document = model.get<MapDocument>(document_entity);
    CHECK(map_document.map == document.default_context);
    CHECK(map_document.active_tileset == kNullEntity);
    REQUIRE(sys::is_map_entity(model, map_document.map));

    const auto& map = model.get<Map>(map_document.map);
    CHECK(map.extent == extent);
    CHECK(map.tile_size == tile_size);
    CHECK(map.active_layer == kNullEntity);
    CHECK(map.active_tileset == kNullEntity);
    CHECK(map.attached_tilesets.empty());
    CHECK(sys::is_group_layer_entity(model, map.root_layer));
  }

  TEST_CASE("create_tileset_document")
  {
    Model model;
    sys::init_model(model, BackendAPI::Null);

    const Int2 tile_size {128, 96};
    const Path img_path = "resources/outside.png";
    const Int2 img_size {1'024, 1'024};

    const auto document_entity = sys::create_tileset_document(model, tile_size, img_path);
    REQUIRE(sys::is_tileset_document_entity(model, document_entity));

    const auto& document = model.get<Document>(document_entity);
    CHECK(document.type == DocumentType::Tileset);
    CHECK(document.component_set == kNullEntity);
    CHECK(document.active_context == document.default_context);
    CHECK(!document.path.has_value());

    const auto& tileset_document = model.get<TilesetDocument>(document_entity);
    REQUIRE(sys::is_tileset_entity(model, tileset_document.tileset));

    const auto& tileset = model.get<Tileset>(tileset_document.tileset);
    CHECK(tileset.tile_size == tile_size);
    CHECK(tileset.row_count == (img_size / tile_size).y);
    CHECK(tileset.column_count == (img_size / tile_size).x);
    CHECK(tileset.tile_index_map.size() == tileset.tiles.size());
    CHECK(tileset.uv_size == Float2 {tileset.tile_size} / Float2 {img_size});
    CHECK(static_cast<usize>(sys::tile_count(tileset)) == tileset.tiles.size());
    CHECK(!tileset.selected_tile_index.has_value());
    REQUIRE(sys::is_texture_entity(model, tileset.texture));

    const auto& texture = model.get<Texture>(tileset.texture);
    CHECK(texture.size == img_size);
    CHECK(texture.path == img_path);
  }

  TEST_CASE("destroy_document")
  {
    Model model;
    sys::init_model(model, BackendAPI::Null);

    const auto document_entity =
        sys::create_map_document(model, TileExtent {5, 5}, Float2 {32, 32});
    REQUIRE(sys::is_document_entity(model, document_entity));

    sys::open_document(model, document_entity);
    sys::select_document(model, document_entity);

    CHECK(sys::is_document_open(model, document_entity));
    CHECK(document_entity == sys::get_active_document(model));

    sys::destroy_document(model, document_entity);

    CHECK(!model.is_valid(document_entity));
    CHECK(!sys::has_active_document(model));

    const auto& document_context = model.get<DocumentContext>();
    CHECK(!document_context.open_documents.contains(document_entity));
  }

  TEST_CASE("select_document")
  {
    Model model;
    sys::init_model(model, BackendAPI::Null);

    const auto document_entity =
        sys::create_map_document(model, TileExtent {5, 5}, Float2 {32, 32});
    REQUIRE(sys::is_document_entity(model, document_entity));

    sys::open_document(model, document_entity);
    CHECK(document_entity != sys::get_active_document(model));

    sys::select_document(model, document_entity);
    CHECK(document_entity == sys::get_active_document(model));
  }
}
