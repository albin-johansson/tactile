// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/menu/debug_menu.hpp"

#include <imgui.h>

#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/menus.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {

void DebugMenu::push(const Model& model)
{
  const auto& language = model.get_language();

  const MenuScope menu {language.get(StringID::kDebugMenu)};
  if (menu.is_open()) {
  }
}

}  // namespace tactile::ui
