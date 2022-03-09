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

#pragma once

#include <string>  // string

#include <entt/entt.hpp>

#include "core/components/attributes.hpp"
#include "items/property_item_context_menu.hpp"
#include "tactile.hpp"

namespace tactile {

class PropertyTable final
{
 public:
  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

 private:
  PropertyItemContextMenuState mContextState;
  Maybe<std::string> mRenameTarget;
  Maybe<std::string> mChangeTypeTarget;

  void ShowCustomProperties(const entt::registry& registry,
                            entt::dispatcher& dispatcher,
                            const comp::AttributeContext& context,
                            bool& isItemContextOpen);
};

}  // namespace tactile
