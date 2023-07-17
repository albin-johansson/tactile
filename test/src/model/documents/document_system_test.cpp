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

#include "model/documents/document_components.hpp"
#include "model/documents/document_factory.hpp"
#include "model/entity_validation.hpp"
#include "model/model_factory.hpp"
#include "model/textures/texture_components.hpp"

using namespace tactile;

TEST_SUITE("DocumentSystem")
{
  TEST_CASE("destroy_document")
  {
    auto model = sys::create_model(BackendAPI::Null);

    const auto document_entity =
        sys::create_map_document(model, TileExtent {5, 5}, Float2 {32, 32});
    REQUIRE(sys::is_document_entity(model, document_entity));

    sys::open_document(model, document_entity);

    CHECK(sys::is_document_open(model, document_entity));
    CHECK(document_entity == sys::get_active_document(model));

    sys::destroy_document(model, document_entity);

    CHECK(!registry.is_valid(document_entity));
    CHECK(!sys::has_active_document(model));

    const auto& document_context = registry.get<DocumentContext>();
    CHECK(!document_context.open_documents.contains(document_entity));
  }

  TEST_CASE("select_document")
  {
    auto model = sys::create_model(BackendAPI::Null);

    const auto document_entity1 =
        sys::create_map_document(model, TileExtent {5, 5}, Float2 {32, 32});
    const auto document_entity2 =
        sys::create_map_document(model, TileExtent {5, 5}, Float2 {32, 32});

    REQUIRE(sys::is_document_entity(model, document_entity1));
    REQUIRE(sys::is_document_entity(model, document_entity2));

    sys::open_document(model, document_entity1);
    sys::open_document(model, document_entity2);
    CHECK(sys::get_active_document(model) == document_entity2);

    sys::select_document(model, document_entity1);
    CHECK(sys::get_active_document(model) == document_entity1);
  }
}
