#pragma once

#include <string>       // string
#include <string_view>  // string_view

#include <entt/entt.hpp>  // registry, entity
#include <tactile_def.hpp>

#include "core/attribute_value.hpp"
#include "core/components/property.hpp"
#include "core/components/property_context.hpp"
#include "snapshot.hpp"

namespace tactile::sys {

/// \name Property system
/// \{

/**
 * \brief Adds a property context component to an entity.
 *
 * \details The created property context will automatically be assigned a unique ID.
 *
 * \pre The supplied entity identifier cannot be null.
 *
 * \param registry the source registry.
 * \param entity the entity that will be assigned a property context.
 *
 * \return the created property context.
 */
auto AddPropertyContext(entt::registry& registry, entt::entity entity)
    -> PropertyContext&;

/**
 * \brief Creates a snapshot of the state of a property context.
 *
 * \pre The supplied entity cannot be null.
 * \pre The supplied entity must feature a property context component.
 *
 * \param registry the source registry.
 * \param source the source entity that features a property context.
 *
 * \return a snapshot of the property context.
 *
 * \see RestorePropertyContext()
 */
[[nodiscard]] auto CopyPropertyContext(const entt::registry& registry,
                                       entt::entity source) -> PropertyContextSnapshot;

/**
 * \brief Restores a property context component based on a snapshot.
 *
 * \details This function will override any current property context component associated
 * with the specified entity.
 *
 * \pre The supplied entity identifier cannot be null.
 *
 * \param registry the source registry.
 * \param entity the entity that will be assigned the restored property context.
 * \param snapshot a snapshot obtained through `CopyPropertyContext()`.
 *
 * \see CopyPropertyContext()
 */
void RestorePropertyContext(entt::registry& registry,
                            entt::entity entity,
                            PropertyContextSnapshot snapshot);

/**
 * \brief Returns the currently active property context.
 *
 * \param registry the source registry.
 *
 * \return the current property context.
 */
[[nodiscard]] auto GetCurrentContext(entt::registry& registry) -> PropertyContext&;

/// \copydoc GetCurrentContext()
[[nodiscard]] auto GetCurrentContext(const entt::registry& registry)
    -> const PropertyContext&;

/**
 * \brief Indicates whether or not there is a context associated with a specific ID.
 *
 * \param registry the source registry.
 * \param id the context identifier to look for.
 *
 * \return `true` if there is a property context associated with the ID; `false`
 * otherwise.
 */
[[nodiscard]] auto HasContext(const entt::registry& registry, ContextID id) -> bool;

/**
 * \brief Returns the property context associated with a context ID.
 *
 * \param registry the source registry.
 * \param id the identifier associated with the desired property context.
 *
 * \return a reference to the found property context.
 *
 * \throws TactileError if there is no matching property context.
 */
[[nodiscard]] auto GetContext(entt::registry& registry, ContextID id) -> PropertyContext&;

/// \copydoc GetContext()
[[nodiscard]] auto GetContext(const entt::registry& registry, ContextID id)
    -> const PropertyContext&;

/**
 * \brief Returns the identifier associated with the currently active property context.
 *
 * \param registry the source registry.
 *
 * \return the current context identifier.
 */
[[nodiscard]] auto GetCurrentContextId(const entt::registry& registry) -> ContextID;

/**
 * \brief Adds a property to a property context.
 *
 * \pre The supplied name must be unique within the property context.
 *
 * \param registry the source registry.
 * \param context the context that will be modified.
 * \param name the name of the property.
 * \param type the type of the property value.
 */
void AddProperty(entt::registry& registry,
                 PropertyContext& context,
                 std::string name,
                 PropertyType type);

/**
 * \brief Adds a property to a property context.
 *
 * \pre The name must be unique within the property context.
 *
 * \param registry the source registry.
 * \param context the context that will be modified.
 * \param name the name of the property.
 * \param value the initial value of the property.
 */
void AddProperty(entt::registry& registry,
                 PropertyContext& context,
                 std::string name,
                 attribute_value value);

/**
 * \brief Removes a property from a property context.
 *
 * \pre The context must feature a property with the specified name.
 *
 * \param registry the source registry.
 * \param context the context that will be modified.
 * \param name the name of the property that will be removed.
 */
void RemoveProperty(entt::registry& registry,
                    PropertyContext& context,
                    std::string_view name);

/**
 * \brief Renames a property in a property context.
 *
 * \pre The context must feature a property with the _old_ name.
 * \pre The context must _not_ feature a property with the _new_ name.
 *
 * \param registry the source registry.
 * \param context the context that will be modified.
 * \param oldName the name of the property that will be renamed.
 * \param newName the new name of the property.
 */
void RenameProperty(entt::registry& registry,
                    PropertyContext& context,
                    std::string_view oldName,
                    std::string newName);

/**
 * \brief Updates the value of an existing property.
 *
 * \pre The context must feature a property with the specified name.
 *
 * \param registry the source registry.
 * \param context the context that will be modified.
 * \param name the name of the property that will be updated.
 * \param value the new value of the property.
 */
void UpdateProperty(entt::registry& registry,
                    PropertyContext& context,
                    std::string_view name,
                    attribute_value value);

/**
 * \brief Changes the type of the value of an existing property.
 *
 * \details The property will assume the default value of the specified type.
 *
 * \pre The context must feature a property with the specified name.
 *
 * \param registry the source registry.
 * \param context the context that will be modified.
 * \param name the name of the property that will be updated.
 * \param value the new type of the property.
 */
void ChangePropertyType(entt::registry& registry,
                        PropertyContext& context,
                        std::string_view name,
                        PropertyType type);

/**
 * \brief Attempts to find a property in a context with a specific name.
 *
 * \param registry the source registry.
 * \param context the property context that will be queried.
 * \param name the name of the desired property.
 *
 * \return the found property entity; the null entity is returned if there was no match.
 *
 * \see HasPropertyWithName()
 * \see GetProperty()
 */
[[nodiscard]] auto FindProperty(const entt::registry& registry,
                                const PropertyContext& context,
                                std::string_view name) -> entt::entity;

/**
 * \brief Returns a property in a property context with a specific name.
 *
 * \param registry the source registry.
 * \param context the property context that will be queried.
 * \param name the name of the desired property.
 *
 * \return a reference to the matching property.
 *
 * \throws TactileError if there is no property with the specified name.
 *
 * \see FindProperty()
 */
[[nodiscard]] auto GetProperty(const entt::registry& registry,
                               const PropertyContext& context,
                               std::string_view name) -> const Property&;

/**
 * \brief Indicates whether or not a property context has a property with a certain name.
 *
 * \note Use the `FindProperty()` function if you intend to make use of the property
 * entity, in order to avoid unnecessary lookups.
 *
 * \param registry the source registry.
 * \param context the property context that will be queried..
 * \param name the name of the property to look for.
 *
 * \return `true` if the context has a property with the specified name; `false`
 * otherwise.
 *
 * \see FindProperty()
 */
[[nodiscard]] auto HasPropertyWithName(const entt::registry& registry,
                                       const PropertyContext& context,
                                       std::string_view name) -> bool;

/* Useful for testing purposes */
void ResetNextContextId() noexcept;

/**
 * \brief Increments the next available context ID and returns the previous value.
 *
 * \return the previous next context ID.
 */
[[nodiscard]] auto GetAndUpdateNextContextId() noexcept -> ContextID;

/**
 * \brief Returns the next available context ID.
 *
 * \return a valid context ID.
 */
[[nodiscard]] auto GetNextContextId() noexcept -> ContextID;

/// \} End of property system

}  // namespace tactile::sys
