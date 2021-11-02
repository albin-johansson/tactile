#pragma once

#include <string>       // string
#include <string_view>  // string_view

#include <tactile-base/property_type.hpp>

#include <entt.hpp>  // registry, entity

#include "core/components/property.hpp"
#include "core/components/property_context.hpp"
#include "core/property_value.hpp"
#include "property_context_snapshot.hpp"

namespace Tactile::Sys {

/// \name Property system
/// \{

auto AddPropertyContext(entt::registry& registry, entt::entity entity)
    -> PropertyContext&;

void RestorePropertyContext(entt::registry& registry,
                            entt::entity source,
                            PropertyContextSnapshot snapshot);

[[nodiscard]] auto CopyPropertyContext(const entt::registry& registry,
                                       entt::entity source) -> PropertyContextSnapshot;

void AddProperty(entt::registry& registry,
                 ContextID id,
                 std::string name,
                 PropertyType type);

void AddProperty(entt::registry& registry,
                 ContextID id,
                 std::string name,
                 PropertyValue value);

void RemoveProperty(entt::registry& registry, ContextID id, std::string_view name);

void RenameProperty(entt::registry& registry,
                    ContextID id,
                    std::string_view oldName,
                    std::string newName);

void RenameProperty(entt::registry& registry,
                    std::string_view oldName,
                    std::string newName);

void UpdateProperty(entt::registry& registry,
                    ContextID id,
                    std::string_view name,
                    PropertyValue value);

void ChangePropertyType(entt::registry& registry,
                        ContextID id,
                        std::string_view name,
                        PropertyType type);

[[nodiscard]] auto GetCurrentContext(entt::registry& registry) -> PropertyContext&;

[[nodiscard]] auto GetCurrentContext(const entt::registry& registry)
    -> const PropertyContext&;

[[nodiscard]] auto GetContext(entt::registry& registry, ContextID id) -> PropertyContext&;

[[nodiscard]] auto GetContext(const entt::registry& registry, ContextID id)
    -> const PropertyContext&;

[[nodiscard]] auto GetPropertyValue(const entt::registry& registry,
                                    ContextID id,
                                    std::string_view name) -> const PropertyValue&;

[[nodiscard]] auto GetPropertyType(const entt::registry& registry,
                                   ContextID id,
                                   std::string_view name) -> PropertyType;

[[nodiscard]] auto FindProperty(const entt::registry& registry, std::string_view name)
    -> entt::entity;

[[nodiscard]] auto FindProperty(const entt::registry& registry,
                                const PropertyContext& context,
                                std::string_view name) -> entt::entity;

[[nodiscard]] auto HasPropertyWithName(const entt::registry& registry,
                                       std::string_view name) -> bool;

/// \} End of property system

}  // namespace Tactile::Sys
