#pragma once

#include <string>       // string
#include <string_view>  // string_view

#include <entt/entt.hpp>  // registry, entity
#include <tactile_def.hpp>

#include "core/attribute_value.hpp"
#include "core/components/attribute_context.hpp"
#include "core/components/property.hpp"
#include "snapshot.hpp"

namespace tactile::sys {

/// \name Property system
/// \{

/**
 * \brief Adds a attribute context component to an entity.
 *
 * \details The created attribute context will automatically be assigned a unique ID.
 *
 * \pre The supplied entity identifier cannot be null.
 *
 * \param registry the source registry.
 * \param entity the entity that will be assigned a attribute context.
 *
 * \return the created attribute context.
 */
auto AddPropertyContext(entt::registry& registry, entt::entity entity)
    -> comp::attribute_context&;

/**
 * \brief Creates a snapshot of the state of a attribute context.
 *
 * \pre The supplied entity cannot be null.
 * \pre The supplied entity must feature a attribute context component.
 *
 * \param registry the source registry.
 * \param source the source entity that features a attribute context.
 *
 * \return a snapshot of the attribute context.
 *
 * \see RestorePropertyContext()
 */
[[nodiscard]] auto CopyPropertyContext(const entt::registry& registry,
                                       entt::entity source) -> PropertyContextSnapshot;

/**
 * \brief Restores a attribute context component based on a snapshot.
 *
 * \details This function will override any current attribute context component associated
 * with the specified entity.
 *
 * \pre The supplied entity identifier cannot be null.
 *
 * \param registry the source registry.
 * \param entity the entity that will be assigned the restored attribute context.
 * \param snapshot a snapshot obtained through `CopyPropertyContext()`.
 *
 * \see CopyPropertyContext()
 */
void RestorePropertyContext(entt::registry& registry,
                            entt::entity entity,
                            PropertyContextSnapshot snapshot);

/**
 * \brief Returns the currently active attribute context.
 *
 * \param registry the source registry.
 *
 * \return the current attribute context.
 */
[[nodiscard]] auto GetCurrentContext(entt::registry& registry)
    -> comp::attribute_context&;

/// \copydoc GetCurrentContext()
[[nodiscard]] auto GetCurrentContext(const entt::registry& registry)
    -> const comp::attribute_context&;

/**
 * \brief Indicates whether or not there is a context associated with a specific ID.
 *
 * \param registry the source registry.
 * \param id the context identifier to look for.
 *
 * \return `true` if there is a attribute context associated with the ID; `false`
 * otherwise.
 */
[[nodiscard]] auto HasContext(const entt::registry& registry, ContextID id) -> bool;

/**
 * \brief Returns the attribute context associated with a context ID.
 *
 * \param registry the source registry.
 * \param id the identifier associated with the desired attribute context.
 *
 * \return a reference to the found attribute context.
 *
 * \throws TactileError if there is no matching attribute context.
 */
[[nodiscard]] auto GetContext(entt::registry& registry, ContextID id)
    -> comp::attribute_context&;

/// \copydoc GetContext()
[[nodiscard]] auto GetContext(const entt::registry& registry, ContextID id)
    -> const comp::attribute_context&;

/**
 * \brief Returns the identifier associated with the currently active attribute context.
 *
 * \param registry the source registry.
 *
 * \return the current context identifier.
 */
[[nodiscard]] auto GetCurrentContextId(const entt::registry& registry) -> ContextID;

/**
 * \brief Adds a property to a attribute context.
 *
 * \pre The supplied name must be unique within the attribute context.
 *
 * \param registry the source registry.
 * \param context the context that will be modified.
 * \param name the name of the property.
 * \param type the type of the property value.
 */
void AddProperty(entt::registry& registry,
                 comp::attribute_context& context,
                 std::string name,
                 PropertyType type);

/**
 * \brief Adds a property to a attribute context.
 *
 * \pre The name must be unique within the attribute context.
 *
 * \param registry the source registry.
 * \param context the context that will be modified.
 * \param name the name of the property.
 * \param value the initial value of the property.
 */
void AddProperty(entt::registry& registry,
                 comp::attribute_context& context,
                 std::string name,
                 attribute_value value);

/**
 * \brief Removes a property from a attribute context.
 *
 * \pre The context must feature a property with the specified name.
 *
 * \param registry the source registry.
 * \param context the context that will be modified.
 * \param name the name of the property that will be removed.
 */
void RemoveProperty(entt::registry& registry,
                    comp::attribute_context& context,
                    std::string_view name);

/**
 * \brief Renames a property in a attribute context.
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
                    comp::attribute_context& context,
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
                    comp::attribute_context& context,
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
                        comp::attribute_context& context,
                        std::string_view name,
                        PropertyType type);

/**
 * \brief Attempts to find a property in a context with a specific name.
 *
 * \param registry the source registry.
 * \param context the attribute context that will be queried.
 * \param name the name of the desired property.
 *
 * \return the found property entity; the null entity is returned if there was no match.
 *
 * \see HasPropertyWithName()
 * \see GetProperty()
 */
[[nodiscard]] auto FindProperty(const entt::registry& registry,
                                const comp::attribute_context& context,
                                std::string_view name) -> entt::entity;

/**
 * \brief Returns a property in a attribute context with a specific name.
 *
 * \param registry the source registry.
 * \param context the attribute context that will be queried.
 * \param name the name of the desired property.
 *
 * \return a reference to the matching property.
 *
 * \throws TactileError if there is no property with the specified name.
 *
 * \see FindProperty()
 */
[[nodiscard]] auto GetProperty(const entt::registry& registry,
                               const comp::attribute_context& context,
                               std::string_view name) -> const comp::property&;

/**
 * \brief Indicates whether or not a attribute context has a property with a certain name.
 *
 * \note Use the `FindProperty()` function if you intend to make use of the property
 * entity, in order to avoid unnecessary lookups.
 *
 * \param registry the source registry.
 * \param context the attribute context that will be queried..
 * \param name the name of the property to look for.
 *
 * \return `true` if the context has a property with the specified name; `false`
 * otherwise.
 *
 * \see FindProperty()
 */
[[nodiscard]] auto HasPropertyWithName(const entt::registry& registry,
                                       const comp::attribute_context& context,
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
