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

#include "core/attribute.hpp"
#include "core/commands/command_stack.hpp"
#include "core/common/filesystem.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/documents/document_type.hpp"
#include "core/fwd.hpp"

namespace tactile {

/// Abstract base class for document representations.
/// TODO: convert to interface, add DocumentDelegate class
class ADocument
{
 public:
  virtual ~ADocument() noexcept = default;

  /// Updates the state of the document.
  virtual void update() = 0;

  /// Sets the name of the root document context.
  [[deprecated]] virtual void set_name(std::string name) = 0;

  /// Sets the file path associated with the document.
  void set_path(fs::path path);

  void select_context(const UUID& contextId);

  [[nodiscard]] auto active_context_id() const -> const UUID&;

  void set_component_index(Shared<ComponentIndex> index);

  [[nodiscard]] auto get_component_index() -> Shared<ComponentIndex>;
  [[nodiscard]] auto get_component_index() const -> Shared<const ComponentIndex>;

  void define_component(std::string name);

  void undef_component(const UUID& componentId);

  void rename_component(const UUID& componentId, std::string name);

  void add_component_attribute(const UUID& componentId, std::string name);

  void remove_component_attribute(const UUID& componentId, std::string name);

  void rename_component_attribute(const UUID& componentId,
                                  std::string current,
                                  std::string updated);

  void duplicate_component_attribute(const UUID& componentId, std::string name);

  void set_component_attribute_type(const UUID&   componentId,
                                    std::string   name,
                                    AttributeType type);

  void update_component(const UUID& componentId, std::string name, Attribute value);

  void attach_component(const UUID& contextId, const UUID& componentId);

  void detach_component(const UUID& contextId, const UUID& componentId);

  void update_attached_component(const UUID& contextId,
                                 const UUID& componentId,
                                 std::string name,
                                 Attribute   value);

  void reset_attached_component(const UUID& contextId, const UUID& componentId);

  void add_property(const UUID& contextId, std::string name, AttributeType type);

  void remove_property(const UUID& contextId, std::string name);

  void rename_property(const UUID& contextId, std::string current, std::string updated);

  void update_property(const UUID& contextId, std::string name, Attribute value);

  void change_property_type(const UUID& contextId, std::string name, AttributeType type);

  virtual void register_context(Shared<IContext> context) = 0;
  virtual void unregister_context(const UUID& id) = 0;

  /// Looks up an existing context in the document.
  [[nodiscard]] virtual auto get_context(const UUID& id) -> Shared<IContext> = 0;

  [[nodiscard]] virtual auto view_context(const UUID& id) const -> const IContext& = 0;

  [[nodiscard]] virtual auto has_context(const UUID& id) const -> bool = 0;

  /// Indicates whether the document represents a map.
  [[nodiscard]] auto is_map() const -> bool;

  /// Indicates whether the document represents a tileset.
  [[nodiscard]] auto is_tileset() const -> bool;

  /// Indicates whether the document has a defined path.
  [[nodiscard]] auto has_path() const -> bool;

  /// Returns the previously set document path, throwing if there is none.
  [[nodiscard]] auto get_path() const -> const fs::path&;

  /// Returns the document command history.
  [[nodiscard]] auto get_history() -> CommandStack&;
  [[nodiscard]] auto get_history() const -> const CommandStack&;

  [[nodiscard]] virtual auto get_viewport() -> Viewport& = 0;
  [[nodiscard]] virtual auto get_viewport() const -> const Viewport& = 0;

  /// Returns the name of the root document context.
  [[nodiscard]] virtual auto get_name() const -> const std::string& = 0;

  /// Returns the type of the document.
  [[nodiscard]] virtual auto get_type() const -> DocumentType = 0;

 protected:
  CommandStack           mCommands;
  Shared<ComponentIndex> mComponentIndex;
  Maybe<fs::path>        mPath;
  UUID                   mActiveContext;
};

}  // namespace tactile