#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class Model;

void UpdateMenuBarWidget(const Model& model, entt::dispatcher& dispatcher);

void EnableOpenMapDialog();

void EnableSettingsDialog();

void EnableTilesetDialog();

}  // namespace tactile
