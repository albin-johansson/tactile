// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/menus.hpp"

#include <imgui.h>

namespace tactile::core::ui {

MenuScope::MenuScope(const char* name)
  : mIsOpen {ImGui::BeginMenu(name)}
{}

MenuScope::~MenuScope() noexcept
{
  if (mIsOpen) {
    ImGui::EndMenu();
  }
}

auto MenuScope::is_open() const -> bool
{
  return mIsOpen;
}

}  // namespace tactile::core::ui
