// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/popups.hpp"

namespace tactile::ui {

PopupScope::PopupScope(const char* id, const ImGuiWindowFlags flags)
  : mIsOpen {ImGui::BeginPopup(id, flags)}
{}

PopupScope::PopupScope(ModalPopupTag, const char* id, const ImGuiWindowFlags flags)
  : mIsOpen {ImGui::BeginPopupModal(id, nullptr, flags)}
{}

PopupScope::~PopupScope() noexcept
{
  if (mIsOpen) {
    ImGui::EndPopup();
  }
}

auto PopupScope::is_open() const -> bool
{
  return mIsOpen;
}

}  // namespace tactile::ui
