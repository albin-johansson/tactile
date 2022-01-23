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

/// \name Component definition functions
/// \{

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
 * \param registry the current document registry.
 * \param id the requested component identifier.
 * \param name the name of the component.
 */
void CreateComponentDef(entt::registry& registry, ComponentID id, std::string name);

/**
 * \brief Creates a new component definition with the specified name, and returns the ID.
 *
 * \pre the supplied component name must be unique.
 *
 * \param registry the current document registry.
 * \param name the (unique) name of the component definition.
 *
 * \return the ID associated with the new component definition.
 */
auto CreateComponentDef(entt::registry& registry, std::string name) -> ComponentID;

struct RemoveComponentDefResult final
{
  ComponentID id{};                                   ///< Previous component ID.
  std::string name;                                   ///< Previous component name.
  ComponentAttributeMap attributes;                   ///< Removed component attributes.
  std::map<ContextID, ComponentAttributeMap> values;  ///< Removed context attributes.
};

/**
 * \brief Deletes a component definition, removing it from all contexts.
 *
 * \pre the component ID must be valid.
 *
 * \param registry the current document registry.
 * \param id the ID of the component definition that will be deleted.
 *
 * \return a snapshot of the removed attributes.
 *
 * \see RestoreComponentDef()
 */
auto RemoveComponentDef(entt::registry& registry, ComponentID id)
    -> RemoveComponentDefResult;

/**
 * \brief Restores a previously removed component definition.
 *
 * \details This function restores a component definition and adds the component to all
 * contexts that featured the component at the time of removal, with the correct attribute
 * values.
 *
 * \param registry the current document registry.
 * \param snapshot the cached result of the removal of the component definition.
 */
void RestoreComponentDef(entt::registry& registry, RemoveComponentDefResult snapshot);

/**
 * \brief Changes the name of a component definition.
 *
 * \pre the component ID must be valid.
 * \pre the name must not be used by any other components.
 *
 * \param registry the current document registry.
 * \param id the ID of the component that will be renamed.
 * \param name the new component name.
 */
void RenameComponentDef(entt::registry& registry, ComponentID id, std::string name);

[[nodiscard]] auto FindComponentDef(const entt::registry& registry, ComponentID id)
    -> entt::entity;

[[nodiscard]] auto GetComponentDef(entt::registry& registry, ComponentID id)
    -> std::pair<entt::entity, ComponentDef&>;

[[nodiscard]] auto GetComponentDef(const entt::registry& registry, ComponentID id)
    -> std::pair<entt::entity, const ComponentDef&>;

[[nodiscard]] auto GetComponentDef(const entt::registry& registry, std::string_view name)
    -> std::pair<entt::entity, const ComponentDef&>;

[[nodiscard]] auto GetComponentDefName(const entt::registry& registry, ComponentID id)
    -> const std::string&;

[[nodiscard]] auto GetFirstAvailableComponentDef(const entt::registry& registry)
    -> Maybe<ComponentID>;

/**
 * \brief Indicates whether a component ID is valid, i.e. currently in use.
 *
 * \param registry the current document registry.
 * \param id the component ID that will be checked.
 *
 * \return `true` if the component ID is valid; `false` otherwise.
 */
[[nodiscard]] auto IsComponentValid(const entt::registry& registry, ComponentID id)
    -> bool;

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
void CreateComponentAttribute(entt::registry& registry,
                              ComponentID id,
                              const std::string& name);

void CreateComponentAttribute(entt::registry& registry,
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
void RemoveComponentAttribute(entt::registry& registry,
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
void RenameComponentAttribute(entt::registry& registry,
                              ComponentID id,
                              const std::string& current,
                              std::string updated);

auto DuplicateComponentAttribute(entt::registry& registry,
                                 ComponentID id,
                                 std::string_view attribute) -> std::string;

/**
 * \brief Sets the type of a component attribute.
 *
 * \pre the component ID must be valid.
 * \pre the attribute name must be valid.
 *
 * \details It is required to call this function to update the underlying type before
 * updating the attribute value using `SetComponentAttributeValue()`.
 *
 * \param registry the current document registry.
 * \param id the ID of the parent component.
 * \param attribute the name of the attribute that will be modified.
 * \param type the new type of the attribute.
 *
 * \throws TactileError if the attribute name is invalid.
 *
 * \see SetComponentAttributeValue()
 */
void SetComponentAttributeType(entt::registry& registry,
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
 * \see SetComponentAttributeType()
 */
void SetComponentAttributeValue(entt::registry& registry,
                                ComponentID id,
                                std::string_view attribute,
                                attribute_value value);

[[nodiscard]] auto GetComponentAttributeType(const entt::registry& registry,
                                             ComponentID id,
                                             std::string_view attribute) -> PropertyType;

[[nodiscard]] auto GetComponentAttributeValue(const entt::registry& registry,
                                              ComponentID id,
                                              std::string_view attribute)
    -> const attribute_value&;

[[nodiscard]] auto IsComponentAttributeNameTaken(const entt::registry& registry,
                                                 ComponentID id,
                                                 std::string_view name) -> bool;

[[nodiscard]] auto GetComponentAttributeCount(const entt::registry& registry,
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
 * \see HasComponent()
 */
auto AddComponent(entt::registry& registry, ContextID contextId, ComponentID componentId)
    -> Component&;

struct RemoveComponentResult final
{
  ContextID context_id{};        ///< Context from which component was removed.
  ComponentID component_id{};    ///< Previous component ID.
  ComponentAttributeMap values;  ///< Removed context attributes.
};

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
 * \see HasComponent()
 */
auto RemoveComponent(entt::registry& registry,
                     ContextID contextId,
                     ComponentID componentId) -> RemoveComponentResult;

/**
 * \brief Restores a previously removed component to a context.
 *
 * \param registry the current document registry.
 * \param snapshot the snapshot of the previously removed component.
 */
void RestoreComponent(entt::registry& registry, RemoveComponentResult snapshot);

void UpdateComponent(entt::registry& registry,
                     ContextID contextId,
                     ComponentID componentId,
                     std::string_view attribute,
                     attribute_value value);

struct ResetComponentResult final
{
  ComponentAttributeMap values;  ///< Previous component values.
};

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
auto ResetComponent(entt::registry& registry,
                    ContextID contextId,
                    ComponentID componentId) -> ResetComponentResult;

/**
 * \brief Indicates whether a context holds a specific component.
 *
 * \param registry the current document registry.
 * \param contextId the context that will be checked.
 * \param componentId the ID of the component type to check for.
 *
 * \return `true` if the context has the component; `false` otherwise.
 */
[[nodiscard]] auto HasComponent(const entt::registry& registry,
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
[[nodiscard]] auto GetComponent(const entt::registry& registry,
                                ContextID contextId,
                                ComponentID componentId) -> const Component&;

[[nodiscard]] auto GetAttribute(const Component& component, std::string_view attribute)
    -> const attribute_value&;

[[nodiscard]] auto GetComponentAttribute(const entt::registry& registry,
                                         ContextID contextId,
                                         ComponentID componentId,
                                         std::string_view attribute)
    -> const attribute_value&;

[[nodiscard]] auto GetComponentCount(const entt::registry& registry, ContextID contextId)
    -> usize;

[[nodiscard]] auto IsComponentNameTaken(const entt::registry& registry,
                                        std::string_view name) -> bool;

/// \} End of component functions

}  // namespace tactile::sys
