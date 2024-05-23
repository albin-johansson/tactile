// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/menu/file_menu.hpp"

#include <imgui.h>

#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/common/menus.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {

void FileMenu::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();
  const auto* current_document = model.get_current_document();

  const MenuScope menu {language.get(StringID::kFileMenu)};
  if (menu.is_open()) {
  }
}

}  // namespace tactile::ui
