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

#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // pair

#include <entt/fwd.hpp>

#include "core/attribute.hpp"
#include "core/common/associative.hpp"
#include "core/common/maybe.hpp"
#include "core/components/attributes.hpp"
#include "tactile.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup component-system Component System
 */

/// \addtogroup component-system
/// \{

struct RemoveComponentDefResult final
{
  ComponentID id{};                             ///< Previous component ID.
  std::string name;                             ///< Previous component name.
  ComponentAttrMap attributes;                  ///< Removed component attributes.
  HashMap<ContextID, ComponentAttrMap> values;  ///< Removed context attributes.
};

struct SetComponentAttrTypeResult final
{
  ComponentID comp_id{};                 ///< Affected component definition ID.
  std::string attr_name;                 ///< Affected attribute name.
  Attribute base_value;                  ///< Previous default value.
  HashMap<ContextID, Attribute> values;  ///< Previous values.
};

struct RemoveComponentResult final
{
  ContextID context{};      ///< Context from which component was removed.
  ComponentID component{};  ///< Previous component ID.
  ComponentAttrMap values;  ///< Removed context attributes.
};

struct ResetComponentResult final
{
  ComponentAttrMap values;  ///< Previous component values.
};

/// \name Component definition functions
/// \{

/**
 * \brief Creates a new component definition with the specified name, and returns the ID.
 *
 * \pre the supplied component name must be unique.
 *
 * \param registry the document registry.
 * \param name the (unique) name of the component definition.
 *
 * \return the ID associated with the new component definition.
 */
auto make_component_def(entt::registry& registry, std::string name) -> ComponentID;

/**
 * \brief Creates a new component definition with a specific identifier.
 *
 * \details This function is intended to be used when restoring a previously created
 * component definition, allowing the previously assigned identifier to be safely reused.
 * This makes it much easier to implement command classes related to components.
 *
 * \pre the component ID must be unused.
 * \pre the component name must be unique.
 *
 * \param registry the document registry.
 * \param compId the requested component identifier.
 * \param name the name of the component.
 */
void make_component_def(entt::registry& registry,
                        const ComponentID& compId,
                        std::string name);

/**
 * \brief Deletes a component definition, removing it from all contexts.
 *
 * \pre the component ID must be valid.
 *
 * \param registry the document registry.
 * \param compId the ID of the component definition that will be deleted.
 *
 * \return a snapshot of the removed attributes.
 *
 * \see restore_component_def()
 */
auto remove_component_def(entt::registry& registry, const ComponentID& compId)
    -> RemoveComponentDefResult;

/**
 * \brief Restores a previously removed component definition.
 *
 * \details This function restores a component definition and adds the component to all
 * contexts that featured the component at the time of removal, with the correct attribute
 * values.
 *
 * \param registry the document registry.
 * \param snapshot the cached result of the removal of the component definition.
 */
void restore_component_def(entt::registry& registry, RemoveComponentDefResult snapshot);

/**
 * \brief Changes the name of a component definition.
 *
 * \pre the component ID must be valid.
 * \pre the name must not be used by any other components.
 *
 * \param registry the document registry.
 * \param compId the ID of the component that will be renamed.
 * \param name the new component name.
 */
void rename_component_def(entt::registry& registry,
                          const ComponentID& compId,
                          std::string name);

/**
 * \brief Indicates whether a component ID is valid, i.e. if it's currently in use.
 *
 * \param registry the document registry.
 * \param compId the component ID that will be checked.
 *
 * \return `true` if the component ID is valid; `false` otherwise.
 */
[[nodiscard]] auto is_valid_component(const entt::registry& registry,
                                      const ComponentID& compId) -> bool;

/**
 * \brief Indicates whether a component name is taken.
 *
 * \param registry the document registry.
 * \param name the name that will be checked.
 *
 * \return `true` a component definition uses the name; `false` otherwise.
 */
[[nodiscard]] auto is_component_name_taken(const entt::registry& registry,
                                           std::string_view name) -> bool;

[[nodiscard]] auto get_first_available_component_def(const entt::registry& registry)
    -> Maybe<ComponentID>;

[[nodiscard]] auto find_component_def(const entt::registry& registry,
                                      const ComponentID& compId) -> entt::entity;

[[nodiscard]] auto find_component_def(const entt::registry& registry,
                                      std::string_view name) -> entt::entity;

[[nodiscard]] auto get_component_def(entt::registry& registry, const ComponentID& compId)
    -> std::pair<entt::entity, comp::ComponentDef&>;

[[nodiscard]] auto get_component_def(const entt::registry& registry,
                                     const ComponentID& compId)
    -> std::pair<entt::entity, const comp::ComponentDef&>;

[[nodiscard]] auto get_component_def_name(const entt::registry& registry,
                                          const ComponentID& compId)
    -> const std::string&;

/// \} End of component definition functions

/// \name Component attribute functions
/// \{

/**
 * \brief Adds a component attribute to a component definition.
 *
 * \details The attribute is initially assumed to be a string attribute, this can later be
 * changed by the user. This function also ensures that all current contexts that feature
 * the component are updated to provide the new attribute.
 *
 * \pre the component ID must be valid.
 * \pre the attribute name must be unique in the context of the component definition.
 *
 * \param registry the current document registry.
 * \param compId the ID of the component definition that will be updated.
 * \param name the name of the attribute.
 */
void make_component_attribute(entt::registry& registry,
                              const ComponentID& compId,
                              const std::string& name);

void make_component_attribute(entt::registry& registry,
                              const ComponentID& compId,
                              const std::string& name,
                              const Attribute& value);

/**
 * \brief Removes an attribute from a component definition.
 *
 * \details This function ensures that all existing components of the affected type are
 * updated to no longer feature the attribute.
 *
 * \pre the component ID must be valid.
 * \pre the attribute name must be valid.
 *
 * \param registry the current document registry.
 * \param compId the ID of the component definition that will be updated.
 * \param name the name of the attribute that will be removed.
 */
void remove_component_attribute(entt::registry& registry,
                                const ComponentID& compId,
                                std::string_view name);

/**
 * \brief Changes the name of a component attribute.
 *
 * \pre the component ID must be valid.
 * \pre `current` must be associated with an existing attribute.
 * \pre `updated` must not be used by any other attributes.
 *
 * \param registry the current document registry.
 * \param compId the ID of the parent component.
 * \param current the current name of the attribute.
 * \param updated the new name of the attribute.
 */
void rename_component_attribute(entt::registry& registry,
                                const ComponentID& compId,
                                const std::string& current,
                                std::string updated);

auto duplicate_component_attribute(entt::registry& registry,
                                   const ComponentID& compId,
                                   std::string_view attribute) -> std::string;

/**
 * \brief Sets the type of a component attribute.
 *
 * \pre the component ID must be valid.
 * \pre the attribute name must be valid.
 *
 * \details It is required to call this function to update the underlying type before
 * updating the attribute value using `set_component_attribute_value()`.
 *
 * \param registry the current document registry.
 * \param compId the ID of the parent component.
 * \param attrName the name of the attribute that will be modified.
 * \param type the new type of the attribute.
 *
 * \return the previous values of the affected attributes.
 *
 * \throws TactileError if the attribute name is invalid.
 *
 * \see set_component_attribute_value()
 */
auto set_component_attribute_type(entt::registry& registry,
                                  const ComponentID& compId,
                                  std::string_view attrName,
                                  AttributeType type) -> SetComponentAttrTypeResult;

/**
 * \brief Restores the type of a component attribute.
 *
 * \param registry the map registry.
 * \param snapshot the snapshot of the previous values.
 */
void restore_component_attribute_type(entt::registry& registry,
                                      const SetComponentAttrTypeResult& snapshot);

/**
 * \brief Sets the default value of a component attribute.
 *
 * \pre the component ID must be valid.
 * \pre the attribute name must be valid.
 * \pre the type of the attribute value must be the same as the previous value.
 *
 * \param registry the current document registry.
 * \param compId the ID of the parent component.
 * \param attribute the name of the attribute that will be updated-
 * \param value the new value of the attribute.
 *
 * \throws TactileError if the attribute name is invalid.
 *
 * \see set_component_attribute_type()
 */
void set_component_attribute_value(entt::registry& registry,
                                   const ComponentID& compId,
                                   std::string_view attribute,
                                   Attribute value);

[[nodiscard]] auto get_component_attribute_type(const entt::registry& registry,
                                                const ComponentID& compId,
                                                std::string_view attribute)
    -> AttributeType;

[[nodiscard]] auto get_component_attribute_value(const entt::registry& registry,
                                                 const ComponentID& compId,
                                                 std::string_view attribute)
    -> const Attribute&;

[[nodiscard]] auto is_component_attribute_name_taken(const entt::registry& registry,
                                                     const ComponentID& compId,
                                                     std::string_view name) -> bool;

[[nodiscard]] auto get_component_attribute_count(const entt::registry& registry,
                                                 const ComponentID& compId) -> usize;

/// \} End of component attribute functions

/// \name Component functions
/// \{

/**
 * \brief Adds a component to a context.
 *
 * \pre the context ID must be valid.
 * \pre the component ID must be valid.
 * \pre the context must not already feature the component.
 *
 * \param registry the current document registry.
 * \param contextId the ID of the context to which the component will be added.
 * \param componentId the ID of the component type that will be added.
 *
 * \return the created component.
 *
 * \see has_component()
 */
auto add_component(entt::registry& registry,
                   ContextID contextId,
                   const ComponentID& compId) -> comp::Component&;

/**
 * \brief Removes a component from a context.
 *
 * \pre the context ID must be valid.
 * \pre the component ID must be valid.
 * \pre the context must feature the component.
 *
 * \param registry the current document registry.
 * \param contextId the ID of the context that will have the component removed.
 * \param componentId the ID of the component that will be removed.
 *
 * \return a snapshot of the removed component.
 *
 * \see has_component()
 */
auto remove_component(entt::registry& registry,
                      ContextID contextId,
                      const ComponentID& compId) -> RemoveComponentResult;

/**
 * \brief Restores a previously removed component to a context.
 *
 * \param registry the current document registry.
 * \param snapshot the snapshot of the previously removed component.
 */
void restore_component(entt::registry& registry, RemoveComponentResult snapshot);

void update_component(entt::registry& registry,
                      ContextID contextId,
                      const ComponentID& compId,
                      std::string_view attribute,
                      Attribute value);

/**
 * \brief Resets the attributes of an attached component to their default values.
 *
 * \pre the context ID must be valid.
 * \pre the component ID must be valid.
 *
 * \param registry the current document registry.
 * \param contextId the context that will be affected.
 * \param componentId the ID of the component that will be reset.
 *
 * \return a snapshot of the previous values of the reset attributes.
 */
auto reset_component(entt::registry& registry,
                     ContextID contextId,
                     const ComponentID& compId) -> ResetComponentResult;

/**
 * \brief Indicates whether a context holds a specific component.
 *
 * \param registry the current document registry.
 * \param contextId the context that will be checked.
 * \param componentId the ID of the component type to check for.
 *
 * \return `true` if the context has the component; `false` otherwise.
 */
[[nodiscard]] auto has_component(const entt::registry& registry,
                                 ContextID contextId,
                                 const ComponentID& compId) -> bool;

/**
 * \brief Returns a component from a context.
 *
 * \param registry the current document registry.
 * \param contextId the ID of the context that will be queried.
 * \param componentId the ID of the component type that will be retrieved.
 *
 * \return a reference to the found component.
 *
 * \throws TactileError if there is no match.
 */
[[nodiscard]] auto get_component(const entt::registry& registry,
                                 ContextID contextId,
                                 const ComponentID& compId) -> const comp::Component&;

[[nodiscard]] auto get_component_attribute(const entt::registry& registry,
                                           ContextID contextId,
                                           const ComponentID& compId,
                                           std::string_view attribute)
    -> const Attribute&;

[[nodiscard]] auto get_component_count(const entt::registry& registry,
                                       ContextID contextId) -> usize;

/// \} End of component functions

/// \} End of group component-system

}  // namespace tactile::sys
