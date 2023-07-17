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

#include "model/model_factory.hpp"

#include <doctest/doctest.h>

#include "model/documents/document_components.hpp"
#include "model/i18n/language_components.hpp"
#include "model/persistence/file_history_components.hpp"
#include "model/persistence/settings.hpp"
#include "model/textures/texture_components.hpp"
#include "model/view/menu_components.hpp"
#include "ui/widget_state.hpp"

using namespace tactile;

TEST_SUITE("ModelFactory")
{
  TEST_CASE("create_model")
  {
    const auto model = sys::create_model(BackendAPI::Null);

    CHECK(registry.has<DocumentContext>());
    CHECK(registry.has<Settings>());
    CHECK(registry.has<Languages>());
    CHECK(registry.has<FileHistory>());
    CHECK(registry.has<Icons>());
    CHECK(registry.has<TextureCache>());
    CHECK(registry.has<TextureCallbacks>());
    CHECK(registry.has<MenuItems>());
    CHECK(registry.has<ui::WidgetState>());
  }
}
