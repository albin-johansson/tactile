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

#include "document.hpp"

#include <utility>  // move

#include "core/commands/command_stack.hpp"
#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"

namespace tactile {

ADocument::ADocument(entt::registry registry) : mRegistry{std::move(registry)} {}

void ADocument::set_name(std::string name)
{
  auto& context = ctx_get<comp::Context>(mRegistry);
  context.name = std::move(name);
}

void ADocument::set_path(std::filesystem::path path)
{
  if (mRegistry.ctx().contains<std::filesystem::path>()) {
    ctx_get<std::filesystem::path>(mRegistry) = std::move(path);
  }
  else {
    mRegistry.ctx().emplace<std::filesystem::path>(std::move(path));
  }
}

auto ADocument::has_path() const -> bool
{
  return mRegistry.ctx().contains<std::filesystem::path>();
}

auto ADocument::get_path() const -> const std::filesystem::path&
{
  return ctx_get<std::filesystem::path>(mRegistry);
}

auto ADocument::get_name() const -> const std::string&
{
  return ctx_get<comp::Context>(mRegistry).name;
}

auto ADocument::get_history() -> CommandStack&
{
  return ctx_get<CommandStack>(mRegistry);
}

auto ADocument::get_history() const -> const CommandStack&
{
  return ctx_get<CommandStack>(mRegistry);
}

auto ADocument::is_map() const -> bool
{
  return get_type() == DocumentType::Map;
}

auto ADocument::is_tileset() const -> bool
{
  return get_type() == DocumentType::Tileset;
}

auto ADocument::id() const -> const UUID&
{
  return ctx_get<comp::Context>(mRegistry).id;
}

}  // namespace tactile