#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateEditMenu(const Model& model, entt::dispatcher& dispatcher);

void UpdateEditMenuWindows(entt::dispatcher& dispatcher);

void EnableTilesetDialog() noexcept;

}  // namespace Tactile
