// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/base/prelude.hpp"

namespace tactile::ui {

/**
 * Tag type used to create modal popups.
 */
struct ModalPopupTag final
{};

/**
 * Used to create modal popups.
 */
inline constexpr ModalPopupTag kModalPopup {};

/**
 * RAII helper for the BeginPopup{Modal}/EndPopup ImGui functions.
 */
class PopupScope final
{
 public:
  TACTILE_DELETE_COPY(PopupScope);
  TACTILE_DELETE_MOVE(PopupScope);

  /**
   * Pushes a popup to the widget stack.
   *
   * \param id    The popup identifier.
   * \param flags The window flags to use.
   */
  [[nodiscard]]
  explicit PopupScope(const char* id, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

  /**
   * Pushes a modal popup to the widget stack.
   *
   * \param id    The popup identifier.
   * \param flags The window flags to use.
   */
  [[nodiscard]]
  PopupScope(ModalPopupTag, const char* id, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

  /**
   * Pops the popup from the widget stack.
   */
  ~PopupScope() noexcept;

  /**
   * Indicates whether the popup is open.
   *
   * \return
   * True if the popup is open; false otherwise.
   */
  [[nodiscard]]
  auto is_open() const -> bool;

 private:
  bool mIsOpen {};
};

}  // namespace tactile::ui
