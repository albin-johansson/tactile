#pragma once

#include <string>       // string
#include <string_view>  // string_view

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry, dispatcher

namespace Tactile {

using RenameDialogInputValidator = bool (*)(const entt::registry&, std::string_view);
using RenameDialogResultCallback = void (*)(entt::dispatcher&, std::string);

void UpdateRenameDialog(NotNull<CStr> title,
                        const entt::registry& registry,
                        entt::dispatcher& dispatcher,
                        RenameDialogInputValidator validator,
                        RenameDialogResultCallback callback);

void OpenRenameDialog(NotNull<CStr> title, std::string_view oldName);

}  // namespace Tactile