#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateMapMenu(const Model& model, entt::dispatcher& dispatcher);

void UpdateMapMenuWindows(entt::dispatcher& dispatcher);

void ShowTilesetDialog() noexcept;

}  // namespace Tactile
