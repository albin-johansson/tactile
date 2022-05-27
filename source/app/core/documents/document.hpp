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

#include <filesystem>  // path

#include <entt/entity/registry.hpp>

#include "core/common/uuid.hpp"
#include "core/documents/document_type.hpp"
#include "editor/fwd.hpp"

namespace tactile {

class ADocument
{
 public:
  explicit ADocument(entt::registry registry);

  virtual ~ADocument() noexcept = default;

  virtual void update() = 0;

  void set_path(std::filesystem::path path);

  [[nodiscard]] auto is_map() const -> bool;

  [[nodiscard]] auto is_tileset() const -> bool;

  [[nodiscard]] auto has_path() const -> bool;

  [[nodiscard]] auto get_path() const -> const std::filesystem::path&;

  [[nodiscard]] auto get_history() -> CommandStack&;
  [[nodiscard]] auto get_history() const -> const CommandStack&;

  [[nodiscard]] auto get_registry() -> entt::registry& { return mRegistry; };
  [[nodiscard]] auto get_registry() const -> const entt::registry& { return mRegistry; }

  [[nodiscard]] auto id() const -> const UUID&;

  [[nodiscard]] virtual auto get_type() const -> DocumentType = 0;

 protected:
  entt::registry mRegistry;
};

}  // namespace tactile