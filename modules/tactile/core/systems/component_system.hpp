#pragma once

#include <map>          // map
#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // pair

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/attribute_value.hpp"
#include "core/components/component.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup component-system Component System
 */

/// \addtogroup component-system
/// \{

struct remove_component_def_result final
{
  ComponentID id{};                                   ///< Previous component ID.
  std::string name;                                   ///< Previous component name.
  ComponentAttributeMap attributes;                   ///< Removed component attributes.
  std::map<ContextID, ComponentAttributeMap> values;  ///< Removed context attributes.
};

struct remove_component_result final
{
  ContextID context_id{};        ///< Context from which component was removed.
  ComponentID component_id{};    ///< Previous component ID.
  ComponentAttributeMap values;  ///< Removed context attributes.
};

struct reset_component_result final
{
  ComponentAttributeMap values;  ///< Previous component values.
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
 * \param id the requested component identifier.
 * \param name the name of the component.
 */
void make_component_def(entt::registry& registry, ComponentID id, std::string name);

/**
 * \brief Deletes a component definition, removing it from all contexts.
 *
 * \pre the component ID must be valid.
 *
 * \param registry the document registry.
 * \param id the ID of the component definition that will be deleted.
 *
 * \return a snapshot of the removed attributes.
 *
 * \see restore_component_def()
 */
auto remove_component_def(entt::registry& registry, ComponentID id)
    -> remove_component_def_result;

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
void restore_component_def(entt::registry& registry,
                           remove_component_def_result snapshot);

/**
 * \brief Changes the name of a component definition.
 *
 * \pre the component ID must be valid.
 * \pre the name must not be used by any other components.
 *
 * \param registry the document registry.
 * \param id the ID of the component that will be renamed.
 * \param name the new component name.
 */
void rename_component_def(entt::registry& registry, ComponentID id, std::string name);

/**
 * \brief Indicates whether a component ID is valid, i.e. if it's currently in use.
 *
 * \param registry the document registry.
 * \param id the component ID that will be checked.
 *
 * \return `true` if the component ID is valid; `false` otherwise.
 */
[[nodiscard]] auto is_valid_component(const entt::registry& registry, ComponentID id)
    -> bool;

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

[[nodiscard]] auto find_component_def(const entt::registry& registry, ComponentID id)
    -> entt::entity;

[[nodiscard]] auto find_component_def(const entt::registry& registry,
                                      std::string_view name) -> entt::entity;

[[nodiscard]] auto get_component_def(entt::registry& registry, ComponentID id)
    -> std::pair<entt::entity, ComponentDef&>;

[[nodiscard]] auto get_component_def(const entt::registry& registry, ComponentID id)
    -> std::pair<entt::entity, const ComponentDef&>;

[[nodiscard]] auto get_component_def_name(const entt::registry& registry, ComponentID id)
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
 * \param id the ID of the component definition that will be updated.
 * \param name the name of the attribute.
 */
void make_component_attribute(entt::registry& registry,
                              ComponentID id,
                              const std::string& name);

void make_component_attribute(entt::registry& registry,
                              ComponentID id,
                              const std::string& name,
                              const attribute_value& value);

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
 * \param id the ID of the component definition that will be updated.
 * \param name the name of the attribute that will be removed.
 */
void remove_component_attribute(entt::registry& registry,
                                ComponentID id,
                                std::string_view name);

/**
 * \brief Changes the name of a component attribute.
 *
 * \pre the component ID must be valid.
 * \pre `current` must be associated with an existing attribute.
 * \pre `updated` must not be used by any other attributes.
 *
 * \param registry the current document registry.
 * \param id the ID of the parent component.
 * \param current the current name of the attribute.
 * \param updated the new name of the attribute.
 */
void rename_component_attribute(entt::registry& registry,
                                ComponentID id,
                                const std::string& current,
                                std::string updated);

auto duplicate_component_attribute(entt::registry& registry,
                                   ComponentID id,
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
 * \param id the ID of the parent component.
 * \param attribute the name of the attribute that will be modified.
 * \param type the new type of the attribute.
 *
 * \throws TactileError if the attribute name is invalid.
 *
 * \see set_component_attribute_value()
 */
void set_component_attribute_type(entt::registry& registry,
                                  ComponentID id,
                                  std::string_view attribute,
                                  PropertyType type);

/**
 * \brief Sets the default value of a component attribute.
 *
 * \pre the component ID must be valid.
 * \pre the attribute name must be valid.
 * \pre the type of the attribute value must be the same as the previous value.
 *
 * \param registry the current document registry.
 * \param id the ID of the parent component.
 * \param attribute the name of the attribute that will be updated-
 * \param value the new value of the attribute.
 *
 * \throws TactileError if the attribute name is invalid.
 *
 * \see set_component_attribute_type()
 */
void set_component_attribute_value(entt::registry& registry,
                                   ComponentID id,
                                   std::string_view attribute,
                                   attribute_value value);

[[nodiscard]] auto get_component_attribute_type(const entt::registry& registry,
                                                ComponentID id,
                                                std::string_view attribute)
    -> PropertyType;

[[nodiscard]] auto get_component_attribute_value(const entt::registry& registry,
                                                 ComponentID id,
                                                 std::string_view attribute)
    -> const attribute_value&;

[[nodiscard]] auto is_component_attribute_name_taken(const entt::registry& registry,
                                                     ComponentID id,
                                                     std::string_view name) -> bool;

[[nodiscard]] auto get_component_attribute_count(const entt::registry& registry,
                                                 ComponentID id) -> usize;

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
auto add_component(entt::registry& registry, ContextID contextId, ComponentID componentId)
    -> Component&;

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
                      ComponentID componentId) -> remove_component_result;

/**
 * \brief Restores a previously removed component to a context.
 *
 * \param registry the current document registry.
 * \param snapshot the snapshot of the previously removed component.
 */
void restore_component(entt::registry& registry, remove_component_result snapshot);

void update_component(entt::registry& registry,
                      ContextID contextId,
                      ComponentID componentId,
                      std::string_view attribute,
                      attribute_value value);

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
                     ComponentID componentId) -> reset_component_result;

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
                                 ComponentID componentId) -> bool;

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
                                 ComponentID componentId) -> const Component&;

[[nodiscard]] auto get_component_attribute(const entt::registry& registry,
                                           ContextID contextId,
                                           ComponentID componentId,
                                           std::string_view attribute)
    -> const attribute_value&;

[[nodiscard]] auto get_component_count(const entt::registry& registry,
                                       ContextID contextId) -> usize;

/// \} End of component functions

/// \} End of group component-system

}  // namespace tactile::sys
