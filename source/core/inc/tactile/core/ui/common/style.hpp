// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::ui {

/// \addtogroup UI
/// \{

/**
 * RAII helper for the PushStyleVar/PopStyleVar ImGui functions.
 */
class StyleVarScope final
{
 public:
  TACTILE_DELETE_COPY(StyleVarScope);
  TACTILE_DELETE_MOVE(StyleVarScope);

  [[nodiscard]]
  StyleVarScope(ImGuiStyleVar id, ImVec2 value);

  [[nodiscard]]
  StyleVarScope(ImGuiStyleVar id, float value);

  ~StyleVarScope() noexcept;

  void pop() noexcept;

 private:
  bool mPopped {false};
};

/**
 * RAII helper for the PushStyleColor/PopStyleColor ImGui functions.
 */
class StyleColorScope final
{
 public:
  TACTILE_DELETE_COPY(StyleColorScope);
  TACTILE_DELETE_MOVE(StyleColorScope);

  [[nodiscard]]
  StyleColorScope(ImGuiCol id, const ImVec4& color);

  [[nodiscard]]
  StyleColorScope(ImGuiCol id, uint32 color);

  ~StyleColorScope() noexcept;

  void pop() noexcept;

 private:
  bool mPopped {false};
};

/// \}

}  // namespace tactile::ui
