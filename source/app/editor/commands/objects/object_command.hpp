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

#include "core/components/attributes.hpp"
#include "core/components/objects.hpp"
#include "editor/commands/command.hpp"
#include "tactile.hpp"

namespace tactile {

class AObjectCommand : public ACommand
{
 public:
  AObjectCommand(std::string name, registry_ref registry, object_id id);

 protected:
  [[nodiscard]] auto target_object() -> comp::Object&;

  [[nodiscard]] auto target_object_context() const -> comp::AttributeContext&;

  [[nodiscard]] auto target_object_id() const noexcept -> object_id { return mObjectId; }

 private:
  registry_ref mRegistry;
  object_id mObjectId;
};

}  // namespace tactile
