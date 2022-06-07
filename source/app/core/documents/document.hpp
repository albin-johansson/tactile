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
#include <string>      // string

#include <entt/entity/registry.hpp>

#include "core/common/uuid.hpp"
#include "core/documents/document_type.hpp"
#include "core/fwd.hpp"

namespace tactile {

/// Abstract base class for document representations.
class ADocument
{
 public:
  explicit ADocument(entt::registry registry);

  virtual ~ADocument() noexcept = default;

  /// Updates the state of the document.
  virtual void update() = 0;

  /// Sets the name of the root document context.
  void set_name(std::string name);

  /// Sets the file path associated with the document.
  void set_path(std::filesystem::path path);

  void attach_component(const UUID& contextId, const UUID& componentId);

  /// Indicates whether the document represents a map.
  [[nodiscard]] auto is_map() const -> bool;

  /// Indicates whether the document represents a tileset.
  [[nodiscard]] auto is_tileset() const -> bool;

  /// Indicates whether the document has a defined path.
  [[nodiscard]] auto has_path() const -> bool;

  /// Returns the previously set document path, throwing if there is none.
  [[nodiscard]] auto get_path() const -> const std::filesystem::path&;

  /// Returns the name of the root document context.
  [[nodiscard]] auto get_name() const -> const std::string&;

  /// Returns the document command history.
  [[nodiscard]] auto get_history() -> CommandStack&;
  [[nodiscard]] auto get_history() const -> const CommandStack&;

  /// Returns the associated registry.
  [[nodiscard]] auto get_registry() -> entt::registry& { return mRegistry; };
  [[nodiscard]] auto get_registry() const -> const entt::registry& { return mRegistry; }

  /// Returns the identifier associated with the document.
  [[nodiscard]] auto id() const -> const UUID&;

  /// Returns the type of the document.
  [[nodiscard]] virtual auto get_type() const -> DocumentType = 0;

 protected:
  entt::registry mRegistry;
};

}  // namespace tactile