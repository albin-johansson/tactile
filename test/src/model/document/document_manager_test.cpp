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

#include <gtest/gtest.h>

#include "core/tile/tileset.hpp"
#include "misc/panic.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"

namespace tactile::test {
namespace {

[[nodiscard]] auto make_map() -> Shared<MapDocument>
{
  return std::make_shared<MapDocument>(Int2 {32, 32}, 5, 5);
}

[[nodiscard]] auto make_tileset() -> Shared<TilesetDocument>
{
  return std::make_shared<TilesetDocument>(TilesetInfo {
      .texture_path = "foo.png",
      .texture_id = 1,
      .texture_size = {1024, 1024},
      .tile_size = {32, 32},
  });
}

}  // namespace

TEST(DocumentManager, Defaults)
{
  const DocumentManager manager;
  ASSERT_FALSE(manager.current().has_value());
}

TEST(DocumentManager, AddMap)
{
  DocumentManager manager;
  ASSERT_THROW(manager.add_map(nullptr), TactileError);

  auto document = make_map();
  const auto id = document->get_map().uuid();

  manager.add_map(document);
  ASSERT_EQ(id, manager.current());

  ASSERT_EQ(document.get(), manager.current_document());
  ASSERT_EQ(document.get(), manager.current_map());
  ASSERT_EQ(nullptr, manager.current_tileset());

  ASSERT_EQ(document, manager.get_document(id));
  ASSERT_EQ(document, manager.get_map(id));

  ASSERT_TRUE(manager.is_open(id));

  ASSERT_TRUE(manager.is_document(id));
  ASSERT_TRUE(manager.is_map(id));
  ASSERT_FALSE(manager.is_tileset(id));

  ASSERT_TRUE(manager.is_map_active());
  ASSERT_FALSE(manager.is_tileset_active());
}

TEST(DocumentManager, AddTileset)
{
  DocumentManager manager;
  ASSERT_THROW(manager.add_tileset(nullptr), TactileError);

  auto document = make_tileset();
  const auto id = document->get_tileset()->uuid();

  manager.add_tileset(document);
  ASSERT_FALSE(manager.current().has_value());

  ASSERT_EQ(nullptr, manager.current_document());
  ASSERT_EQ(nullptr, manager.current_map());
  ASSERT_EQ(nullptr, manager.current_tileset());

  ASSERT_EQ(document, manager.get_document(id));
  ASSERT_EQ(document, manager.get_tileset(id));

  ASSERT_FALSE(manager.is_open(id));

  ASSERT_TRUE(manager.is_document(id));
  ASSERT_FALSE(manager.is_map(id));
  ASSERT_TRUE(manager.is_tileset(id));

  ASSERT_FALSE(manager.is_map_active());
  ASSERT_FALSE(manager.is_tileset_active());
}

TEST(DocumentManager, RemoveMap)
{
  DocumentManager manager;

  auto document = make_map();
  const auto id = document->get_map().uuid();

  ASSERT_EQ(nullptr, manager.remove_map(id));

  manager.add_map(document);
  ASSERT_EQ(document, manager.remove_map(id));

  ASSERT_FALSE(manager.current().has_value());

  ASSERT_EQ(nullptr, manager.current_document());
  ASSERT_EQ(nullptr, manager.current_map());
  ASSERT_EQ(nullptr, manager.current_tileset());

  ASSERT_FALSE(manager.is_open(id));

  ASSERT_FALSE(manager.is_document(id));
  ASSERT_FALSE(manager.is_map(id));
  ASSERT_FALSE(manager.is_tileset(id));

  ASSERT_FALSE(manager.is_map_active());
  ASSERT_FALSE(manager.is_tileset_active());
}

TEST(DocumentManager, RemoveTileset)
{
  DocumentManager manager;

  auto document = make_tileset();
  const auto id = document->get_tileset()->uuid();

  ASSERT_EQ(nullptr, manager.remove_tileset(id));

  manager.add_tileset(document);
  ASSERT_EQ(document, manager.remove_tileset(id));

  ASSERT_FALSE(manager.current().has_value());

  ASSERT_EQ(nullptr, manager.current_document());
  ASSERT_EQ(nullptr, manager.current_map());
  ASSERT_EQ(nullptr, manager.current_tileset());

  ASSERT_FALSE(manager.is_open(id));

  ASSERT_FALSE(manager.is_document(id));
  ASSERT_FALSE(manager.is_map(id));
  ASSERT_FALSE(manager.is_tileset(id));

  ASSERT_FALSE(manager.is_map_active());
  ASSERT_FALSE(manager.is_tileset_active());
}

TEST(DocumentManager, Select)
{
  DocumentManager manager;

  auto document = make_tileset();
  const auto id = document->get_tileset()->uuid();

  manager.add_tileset(document);
  ASSERT_FALSE(manager.current().has_value());

  manager.select(id);
  ASSERT_EQ(id, manager.current());
}

TEST(DocumentManager, Open)
{
  DocumentManager manager;

  auto document = make_tileset();
  const auto id = document->get_tileset()->uuid();

  manager.add_tileset(document);
  ASSERT_TRUE(manager.is_document(id));
  ASSERT_FALSE(manager.is_open(id));

  manager.open(id);
  ASSERT_TRUE(manager.is_document(id));
  ASSERT_TRUE(manager.is_open(id));

  manager.close(id);
  ASSERT_TRUE(manager.is_document(id));
  ASSERT_FALSE(manager.is_open(id));
}

TEST(DocumentManager, Close)
{
  DocumentManager manager;

  auto map = make_map();
  const auto map_id = map->get_map().uuid();

  auto tileset = make_tileset();
  const auto tileset_id = tileset->get_tileset()->uuid();

  map->get_map().tileset_bundle().attach_tileset(tileset->get_tileset(), false);

  manager.add_map(map);
  manager.add_tileset(tileset);

  ASSERT_TRUE(manager.is_document(map_id));
  ASSERT_TRUE(manager.is_document(tileset_id));

  ASSERT_TRUE(manager.is_open(map_id));
  ASSERT_FALSE(manager.is_open(tileset_id));

  manager.open(tileset_id);
  ASSERT_TRUE(manager.is_open(map_id));
  ASSERT_TRUE(manager.is_open(tileset_id));

  manager.close(tileset_id);
  ASSERT_TRUE(manager.is_open(map_id));
  ASSERT_FALSE(manager.is_open(tileset_id));

  ASSERT_TRUE(manager.is_document(map_id));
  ASSERT_TRUE(manager.is_document(tileset_id));

  manager.close(map_id);
  ASSERT_FALSE(manager.is_document(map_id));
  ASSERT_FALSE(manager.is_document(tileset_id));
}

TEST(DocumentManager, RemoveUnusedTilesets)
{
  DocumentManager manager;

  auto map1 = make_map();
  auto map2 = make_map();

  const auto map1_id = map1->get_map().uuid();
  const auto map2_id = map2->get_map().uuid();

  auto ts1 = make_tileset();
  auto ts2 = make_tileset();
  auto ts3 = make_tileset();

  const auto ts1_id = ts1->get_tileset()->uuid();
  const auto ts2_id = ts2->get_tileset()->uuid();
  const auto ts3_id = ts3->get_tileset()->uuid();

  // First map uses TS1 and TS2
  map1->get_map().tileset_bundle().attach_tileset(ts1->get_tileset(), false);
  map1->get_map().tileset_bundle().attach_tileset(ts2->get_tileset(), false);

  // Second map uses TS2 and TS3
  map2->get_map().tileset_bundle().attach_tileset(ts2->get_tileset(), false);
  map2->get_map().tileset_bundle().attach_tileset(ts3->get_tileset(), false);

  manager.add_map(map1);
  manager.add_map(map2);
  manager.add_tileset(ts1);
  manager.add_tileset(ts2);
  manager.add_tileset(ts3);

  ASSERT_TRUE(manager.is_map(map1_id));
  ASSERT_TRUE(manager.is_map(map2_id));
  ASSERT_TRUE(manager.is_tileset(ts1_id));
  ASSERT_TRUE(manager.is_tileset(ts2_id));
  ASSERT_TRUE(manager.is_tileset(ts3_id));

  // This should lead to TS3 being removed, but not TS1 since that is still used
  manager.remove_map(map2_id);

  ASSERT_TRUE(manager.is_tileset(ts1_id));
  ASSERT_FALSE(manager.is_tileset(ts3_id));

  ASSERT_TRUE(manager.is_document(map1_id));
  ASSERT_FALSE(manager.is_document(map2_id));
  ASSERT_TRUE(manager.is_document(ts1_id));
  ASSERT_TRUE(manager.is_document(ts2_id));
  ASSERT_FALSE(manager.is_document(ts3_id));

  manager.remove_map(map1_id);

  ASSERT_FALSE(manager.is_tileset(ts1_id));
  ASSERT_FALSE(manager.is_tileset(ts2_id));
  ASSERT_FALSE(manager.is_tileset(ts3_id));

  ASSERT_FALSE(manager.is_document(map1_id));
  ASSERT_FALSE(manager.is_document(map2_id));
  ASSERT_FALSE(manager.is_document(ts1_id));
  ASSERT_FALSE(manager.is_document(ts2_id));
  ASSERT_FALSE(manager.is_document(ts3_id));
}

TEST(DocumentManager, FirstMatch)
{
  DocumentManager manager;

  auto map = make_map();
  map->set_name("foo");

  const auto id = map->get_map().uuid();
  manager.add_map(map);

  ASSERT_EQ(id, manager.first_match([](const Document& document) {
    return document.get_name() == "foo";
  }));

  ASSERT_FALSE(manager.first_match(
      [](const Document& document) { return document.get_name() == "bar"; }));
}

}  // namespace tactile::test
