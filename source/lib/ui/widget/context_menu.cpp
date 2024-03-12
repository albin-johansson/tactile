// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "context_menu.hpp"

#include "tactile/core/debug/exception.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {

ContextMenu::ContextMenu(const char* name)
    : mName {name}
{
  if (!mName) {
    throw Exception {"Invalid null context menu name"};
  }
}

void ContextMenu::update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (const Popup popup {mName}; popup.is_open()) {
    on_update(model, dispatcher);
  }

  if (mShow) {
    ImGui::OpenPopup(mName);
    mShow = false;
  }
}

void ContextMenu::show()
{
  mShow = true;
}

}  // namespace tactile::ui
