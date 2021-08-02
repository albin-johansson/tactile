#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateFileMenu(const Model& model, entt::dispatcher& dispatcher);

void UpdateFileMenuWindows(entt::dispatcher& dispatcher);

void EnableAddMapDialog() noexcept;

void EnableOpenMapDialog() noexcept;

void EnableSettingsDialog() noexcept;

}  // namespace Tactile
