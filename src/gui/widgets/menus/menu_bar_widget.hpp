#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

void UpdateMenuBarWidget(const Model& model, entt::dispatcher& dispatcher);

void EnableOpenMapDialog();

void EnableSettingsDialog();

}  // namespace Tactile
