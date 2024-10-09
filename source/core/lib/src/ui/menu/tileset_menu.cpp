// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/menu/tileset_menu.hpp"

#include <imgui.h>

#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/menus.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::core::ui {

void TilesetMenu::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();
  const MenuScope menu {language.get(StringID::kTilesetMenu)};
  if (menu.is_open()) {
  }
}

}  // namespace tactile::core::ui
