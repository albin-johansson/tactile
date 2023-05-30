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

#include "model/systems/model_system.hpp"

#include <doctest/doctest.h>

#include "cmd/command_stack.hpp"
#include "components/document.hpp"
#include "components/file_history.hpp"
#include "components/language.hpp"
#include "components/menu.hpp"
#include "components/texture.hpp"
#include "components/tool.hpp"
#include "model/settings.hpp"

namespace tactile::test {

TEST_SUITE("Model system")
{
  TEST_CASE("init_model")
  {
    Model model;
    sys::init_model(model, BackendAPI::Null);

    CHECK(model.has<DocumentContext>());
    CHECK(model.has<ToolContext>());
    CHECK(model.has<Settings>());
    CHECK(model.has<Languages>());
    CHECK(model.has<FileHistory>());
    CHECK(model.has<Icons>());
    CHECK(model.has<TextureCache>());
    CHECK(model.has<TextureCallbacks>());
    CHECK(model.has<MenuItems>());
  }
}

}  // namespace tactile::test
