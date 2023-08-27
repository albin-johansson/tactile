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

#pragma once

#include "common/type/uuid.hpp"
#include "core/attribute.hpp"
#include "model/document/document_type.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/smart_ptr.hpp"
#include "tactile/core/type/string.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(CommandStack)
TACTILE_FWD_DECLARE_CLASS(ContextManager)
TACTILE_FWD_DECLARE_CLASS(ComponentIndex)
TACTILE_FWD_DECLARE_CLASS(Viewport)

/// Abstract base class for document representations.
/// This is almost an interface, with some command functions implemented for convenience.
class Document {
 public:
  virtual ~Document() noexcept = default;

  /// Executes a command that creates a new component definition.
  void define_component(String name);

  /// Executes a command that removes a component definition.
  void undef_component(const UUID& component_id);

  /// Executes a command that renames a component definition.
  void rename_component(const UUID& component_id, String name);

  /// Executes a command that adds an attribute to a component definition.
  void add_component_attribute(const UUID& component_id, String name);

  /// Executes a command that removes an attribute from a component definition.
  void remove_component_attribute(const UUID& component_id, String name);

  /// Executes a command that renames an attribute in a component definition.
  void rename_component_attribute(const UUID& component_id,
                                  String old_name,
                                  String new_name);

  /// Executes a command that duplicates an attribute in a component definition.
  void duplicate_component_attribute(const UUID& component_id, String name);

  /// Executes a command that sets the type of an attribute in a component definition.
  void set_component_attribute_type(const UUID& component_id,
                                    String name,
                                    AttributeType type);

  /// Executes a command that updates an attribute in a component definition.
  void update_component(const UUID& component_id, String name, Attribute value);

  /// Executes a command that attaches a component to a context.
  void attach_component(const UUID& context_id, const UUID& component_id);

  /// Executes a command that detaches a component from a context.
  void detach_component(const UUID& context_id, const UUID& component_id);

  /// Executes a command that updates an attribute in an attached component.
  void update_attached_component(const UUID& context_id,
                                 const UUID& component_id,
                                 String name,
                                 Attribute value);

  /// Executes a command that resets all attributes in an attached component.
  void reset_attached_component(const UUID& context_id, const UUID& component_id);

  /// Executes a command that adds a property to a context.
  void add_property(const UUID& context_id, String name, AttributeType type);

  /// Executes a command that removes a property from a context.
  void remove_property(const UUID& context_id, String name);

  /// Executes a command that renames a property in a context.
  void rename_property(const UUID& context_id, String current, String updated);

  /// Executes a command that updates the value of a property in a context.
  void update_property(const UUID& context_id, String name, Attribute value);

  /// Executes a command that changes the type of a property in a context.
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

  [[nodiscard]] virtual auto get_component_index_ptr() -> Shared<ComponentIndex> = 0;
  [[nodiscard]] virtual auto find_component_index() const -> const ComponentIndex* = 0;

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