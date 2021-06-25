#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class Model;

void ShowMenuBar(const Model& model, entt::dispatcher& dispatcher);

void EnableOpenMapDialog();

void EnableSettingsDialog();

void EnableTilesetDialog();

void ToggleMapGrid();

}  // namespace tactile
