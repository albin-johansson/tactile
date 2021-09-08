#pragma once

#include <entt.hpp>     // registry, entity
#include <string>       // string
#include <string_view>  // string_view

#include "core/components/property.hpp"
#include "core/components/property_context.hpp"
#include "core/properties/property_type.hpp"
#include "core/properties/property_value.hpp"
#include "property_context_snapshot.hpp"

namespace Tactile::Sys {

/// \name Property system
/// \{

void RestorePropertyContext(entt::registry& registry,
                            entt::entity source,
                            PropertyContextSnapshot snapshot);

[[nodiscard]] auto CopyPropertyContext(const entt::registry& registry,
                                       entt::entity source)
    -> PropertyContextSnapshot;

void AddProperty(entt::registry& registry, std::string name, PropertyType type);

void RemoveProperty(entt::registry& registry, std::string_view name);

void RenameProperty(entt::registry& registry,
                    std::string_view oldName,
                    std::string newName);

void UpdateProperty(entt::registry& registry,
                    std::string_view name,
                    PropertyValue value);

void ChangePropertyType(entt::registry& registry,
                        std::string_view name,
                        PropertyType type);

[[nodiscard]] auto GetCurrentContext(entt::registry& registry) -> PropertyContext&;

[[nodiscard]] auto GetCurrentContext(const entt::registry& registry)
    -> const PropertyContext&;

[[nodiscard]] auto FindProperty(const entt::registry& registry,
                                std::string_view name) -> entt::entity;

[[nodiscard]] auto HasPropertyWithName(const entt::registry& registry,
                                       std::string_view name) -> bool;

/// \} End of property system

}  // namespace Tactile::Sys
