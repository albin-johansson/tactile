// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tileset_menu.hpp"

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void update_tileset_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  const Disable disable {!model.is_tileset_active()};
  if (const Menu menu {lang.menu.tileset.c_str()}; menu.is_open()) {
    ui_menu_item(dispatcher, MenuAction::InspectTileset);
  }
}

}  // namespace tactile::ui
