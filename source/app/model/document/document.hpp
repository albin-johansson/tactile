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

#include "core/attribute.hpp"
#include "core/type/path.hpp"
#include "core/type/ptr.hpp"
#include "core/type/string.hpp"
#include "core/uuid.hpp"
#include "document_type.hpp"

namespace tactile {

class CommandStack;
class ContextManager;
class ComponentIndex;
class Viewport;

/// Abstract base class for document representations.
/// This is almost an interface, with some command functions implemented for convenience.
class Document {
 public:
  virtual ~Document() noexcept = default;

  void define_component(String name);

  void undef_component(const UUID& component_id);

  void rename_component(const UUID& component_id, String name);

  void add_component_attribute(const UUID& component_id, String name);

  void remove_component_attribute(const UUID& component_id, String name);

  void rename_component_attribute(const UUID& component_id,
                                  String current,
                                  String updated);

  void duplicate_component_attribute(const UUID& component_id, String name);

  void set_component_attribute_type(const UUID& component_id,
                                    String name,
                                    AttributeType type);

  void update_component(const UUID& component_id, String name, Attribute value);

  void attach_component(const UUID& context_id, const UUID& component_id);

  void detach_component(const UUID& context_id, const UUID& component_id);

  void update_attached_component(const UUID& context_id,
                                 const UUID& component_id,
                                 String name,
                                 Attribute value);

  void reset_attached_component(const UUID& context_id, const UUID& component_id);

  void add_property(const UUID& context_id, String name, AttributeType type);

  void remove_property(const UUID& context_id, String name);

  void rename_property(const UUID& context_id, String current, String updated);

  void update_property(const UUID& context_id, String name, Attribute value);

  void change_property_type(const UUID& context_id, String name, AttributeType type);

  /// Indicates whether the document represents a map.
  [[nodiscard]] auto is_map() const -> bool;

  /// Indicates whether the document represents a tileset.
  [[nodiscard]] auto is_tileset() const -> bool;

  /// Updates the state of the document.
  virtual void update() = 0;

  virtual void set_component_index(Shared<ComponentIndex> index) = 0;

  /// Sets the name of the root document context.
  [[deprecated]] virtual void set_name(String name) = 0;

  /// Sets the file path associated with the document.
  virtual void set_path(Path path) = 0;

  /// Indicates whether the document has a defined path.
  [[nodiscard]] virtual auto has_path() const -> bool = 0;

  /// Returns the previously set document path, throwing if there is none.
  [[nodiscard]] virtual auto get_path() const -> const Path& = 0;

  /// Returns the name of the root document context.
  [[nodiscard]] virtual auto get_name() const -> const String& = 0;

  [[nodiscard]] virtual auto get_component_index() -> Shared<ComponentIndex> = 0;
  [[nodiscard]] virtual auto view_component_index() const -> const ComponentIndex* = 0;

  [[nodiscard]] virtual auto get_contexts() -> ContextManager& = 0;
  [[nodiscard]] virtual auto get_contexts() const -> const ContextManager& = 0;

  /// Returns the document command history.
  [[nodiscard]] virtual auto get_history() -> CommandStack& = 0;
  [[nodiscard]] virtual auto get_history() const -> const CommandStack& = 0;

  [[nodiscard]] virtual auto get_viewport() -> Viewport& = 0;
  [[nodiscard]] virtual auto get_viewport() const -> const Viewport& = 0;

  /// Returns the type of the document.
  [[nodiscard]] virtual auto get_type() const -> DocumentType = 0;
};

}  // namespace tactile