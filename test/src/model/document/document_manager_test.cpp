/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "model/document/document_manager.hpp"

#include <doctest/doctest.h>

#include "core/tile/tileset.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "io/load_texture.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::test {
namespace {

[[nodiscard]] auto make_map() -> Shared<MapDocument>
{
  return std::make_shared<MapDocument>(Int2 {32, 32}, TileExtent {5, 5});
}

[[nodiscard]] auto make_tileset() -> Shared<TilesetDocument>
{
  return std::make_shared<TilesetDocument>(TilesetInfo {
      .texture = load_texture("resources/terrain.png"),
      .tile_size = {32, 32},
  });
}

}  // namespace

TEST_SUITE("DocumentManager")
{
  TEST_CASE("Defaults")
  {
    const DocumentManager manager;
    REQUIRE(!manager.current_document_id().has_value());
  }

  TEST_CASE("add_map_document")
  {
    DocumentManager manager;
    REQUIRE_THROWS_AS(manager.add_map_document(nullptr), Exception);

    auto document = make_map();
    const auto id = document->get_map().get_uuid();

    manager.add_map_document(document);
    REQUIRE(id == manager.current_document_id());

    REQUIRE(document.get() == manager.current_document());
    REQUIRE(document.get() == manager.current_map_document());
    REQUIRE(nullptr == manager.current_tileset_document());

    REQUIRE(document == manager.get_document_ptr(id));
    REQUIRE(document == manager.get_map_document_ptr(id));

    REQUIRE(manager.is_document_open(id));

    REQUIRE(manager.is_document(id));
    REQUIRE(manager.is_map(id));
    REQUIRE(!manager.is_tileset(id));

    REQUIRE(manager.is_map_active());
    REQUIRE(!manager.is_tileset_active());
  }

  TEST_CASE("add_tileset_document")
  {
    DocumentManager manager;
    REQUIRE_THROWS_AS(manager.add_tileset_document(nullptr), Exception);

    auto document = make_tileset();
    const auto id = document->get_tileset_ptr()->get_uuid();

    manager.add_tileset_document(document);
    REQUIRE(!manager.current_document_id().has_value());

    REQUIRE(nullptr == manager.current_document());
    REQUIRE(nullptr == manager.current_map_document());
    REQUIRE(nullptr == manager.current_tileset_document());

    REQUIRE(document == manager.get_document_ptr(id));
    REQUIRE(document == manager.get_tileset_document_ptr(id));

    REQUIRE(!manager.is_document_open(id));

    REQUIRE(manager.is_document(id));
    REQUIRE(!manager.is_map(id));
    REQUIRE(manager.is_tileset(id));

    REQUIRE(!manager.is_map_active());
    REQUIRE(!manager.is_tileset_active());
  }

  TEST_CASE("remove_map_document")
  {
    DocumentManager manager;

    auto document = make_map();
    const auto id = document->get_map().get_uuid();

    REQUIRE(nullptr == manager.remove_map_document(id));

    manager.add_map_document(document);
    REQUIRE(document == manager.remove_map_document(id));

    REQUIRE(!manager.current_document_id().has_value());

    REQUIRE(nullptr == manager.current_document());
    REQUIRE(nullptr == manager.current_map_document());
    REQUIRE(nullptr == manager.current_tileset_document());

    REQUIRE(!manager.is_document_open(id));

    REQUIRE(!manager.is_document(id));
    REQUIRE(!manager.is_map(id));
    REQUIRE(!manager.is_tileset(id));

    REQUIRE(!manager.is_map_active());
    REQUIRE(!manager.is_tileset_active());
  }

  TEST_CASE("remove_tileset_tileset")
  {
    DocumentManager manager;

    auto document = make_tileset();
    const auto id = document->get_tileset_ptr()->get_uuid();

    REQUIRE(nullptr == manager.remove_tileset_document(id));

    manager.add_tileset_document(document);
    REQUIRE(document == manager.remove_tileset_document(id));

    REQUIRE(!manager.current_document_id().has_value());

    REQUIRE(nullptr == manager.current_document());
    REQUIRE(nullptr == manager.current_map_document());
    REQUIRE(nullptr == manager.current_tileset_document());

    REQUIRE(!manager.is_document_open(id));

    REQUIRE(!manager.is_document(id));
    REQUIRE(!manager.is_map(id));
    REQUIRE(!manager.is_tileset(id));

    REQUIRE(!manager.is_map_active());
    REQUIRE(!manager.is_tileset_active());
  }

  TEST_CASE("select_document")
  {
    DocumentManager manager;

    auto document = make_tileset();
    const auto id = document->get_tileset_ptr()->get_uuid();

    manager.add_tileset_document(document);
    REQUIRE(!manager.current_document_id().has_value());

    manager.select_document(id);
    REQUIRE(id == manager.current_document_id());
  }

  TEST_CASE("open_document")
  {
    DocumentManager manager;

    auto document = make_tileset();
    const auto id = document->get_tileset_ptr()->get_uuid();

    manager.add_tileset_document(document);
    REQUIRE(manager.is_document(id));
    REQUIRE(!manager.is_document_open(id));

    manager.open_document(id);
    REQUIRE(manager.is_document(id));
    REQUIRE(manager.is_document_open(id));

    manager.close_document(id);
    REQUIRE(manager.is_document(id));
    REQUIRE(!manager.is_document_open(id));
  }

  TEST_CASE("close_document")
  {
    DocumentManager manager;

    auto map = make_map();
    const auto map_id = map->get_map().get_uuid();

    auto tileset = make_tileset();
    const auto tileset_id = tileset->get_tileset_ptr()->get_uuid();

    map->get_map().get_tileset_bundle().attach_tileset(tileset->get_tileset_ptr(), false);

    manager.add_map_document(map);
    manager.add_tileset_document(tileset);

    REQUIRE(manager.is_document(map_id));
    REQUIRE(manager.is_document(tileset_id));

    REQUIRE(manager.is_document_open(map_id));
    REQUIRE(!manager.is_document_open(tileset_id));

    manager.open_document(tileset_id);
    REQUIRE(manager.is_document_open(map_id));
    REQUIRE(manager.is_document_open(tileset_id));

    manager.close_document(tileset_id);
    REQUIRE(manager.is_document_open(map_id));
    REQUIRE(!manager.is_document_open(tileset_id));

    REQUIRE(manager.is_document(map_id));
    REQUIRE(manager.is_document(tileset_id));

    manager.close_document(map_id);
    REQUIRE(!manager.is_document(map_id));
    REQUIRE(!manager.is_document(tileset_id));
  }

  TEST_CASE("remove_map_document should remove associated tilesets that are unused")
  {
    DocumentManager manager;

    auto map1 = make_map();
    auto map2 = make_map();

    const auto map1_id = map1->get_map().get_uuid();
    const auto map2_id = map2->get_map().get_uuid();

    auto ts1 = make_tileset();
    auto ts2 = make_tileset();
    auto ts3 = make_tileset();

    const auto ts1_id = ts1->get_tileset_ptr()->get_uuid();
    const auto ts2_id = ts2->get_tileset_ptr()->get_uuid();
    const auto ts3_id = ts3->get_tileset_ptr()->get_uuid();

    // First map uses TS1 and TS2
    map1->get_map().get_tileset_bundle().attach_tileset(ts1->get_tileset_ptr(), false);
    map1->get_map().get_tileset_bundle().attach_tileset(ts2->get_tileset_ptr(), false);

    // Second map uses TS2 and TS3
    map2->get_map().get_tileset_bundle().attach_tileset(ts2->get_tileset_ptr(), false);
    map2->get_map().get_tileset_bundle().attach_tileset(ts3->get_tileset_ptr(), false);

    manager.add_map_document(map1);
    manager.add_map_document(map2);
    manager.add_tileset_document(ts1);
    manager.add_tileset_document(ts2);
    manager.add_tileset_document(ts3);

    REQUIRE(manager.is_map(map1_id));
    REQUIRE(manager.is_map(map2_id));
    REQUIRE(manager.is_tileset(ts1_id));
    REQUIRE(manager.is_tileset(ts2_id));
    REQUIRE(manager.is_tileset(ts3_id));

    // This should lead to TS3 being removed, but not TS1 since that is still used
    manager.remove_map_document(map2_id);

    REQUIRE(manager.is_tileset(ts1_id));
    REQUIRE(!manager.is_tileset(ts3_id));

    REQUIRE(manager.is_document(map1_id));
    REQUIRE(!manager.is_document(map2_id));
    REQUIRE(manager.is_document(ts1_id));
    REQUIRE(manager.is_document(ts2_id));
    REQUIRE(!manager.is_document(ts3_id));

    manager.remove_map_document(map1_id);

    REQUIRE(!manager.is_tileset(ts1_id));
    REQUIRE(!manager.is_tileset(ts2_id));
    REQUIRE(!manager.is_tileset(ts3_id));

    REQUIRE(!manager.is_document(map1_id));
    REQUIRE(!manager.is_document(map2_id));
    REQUIRE(!manager.is_document(ts1_id));
    REQUIRE(!manager.is_document(ts2_id));
    REQUIRE(!manager.is_document(ts3_id));
  }

  TEST_CASE("first_match")
  {
    DocumentManager manager;

    auto map = make_map();
    map->set_name("foo");

    const auto id = map->get_map().get_uuid();
    manager.add_map_document(map);

    REQUIRE(id == manager.first_match([](const Document& document) {
      return document.get_name() == "foo";
    }));

    REQUIRE(!manager.first_match(
        [](const Document& document) { return document.get_name() == "bar"; }));
  }
}

}  // namespace tactile::test
