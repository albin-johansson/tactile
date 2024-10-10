// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/overlays.hpp"

#include <imgui.h>

#include "tactile/core/ui/imgui_compat.hpp"

namespace tactile::core::ui {
namespace {

inline constexpr auto kOverlayWindowFlags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing |
    ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

void _prepare_overlay_position(const Float2& anchor)
{
  const auto frame_padding = ImGui::GetStyle().FramePadding;
  const auto next_pivot = to_imvec2(anchor);

  auto next_pos = ImGui::GetCursorScreenPos() + ImGui::GetContentRegionAvail() * next_pivot;

  next_pos.x += frame_padding.x * ((next_pivot.x < 0.5f) ? 1.0f : -1.0f);
  next_pos.y += frame_padding.y * ((next_pivot.y < 0.5f) ? 1.0f : -1.0f);

  ImGui::SetNextWindowPos(next_pos, ImGuiCond_Always, next_pivot);
}

}  // namespace

OverlayScope::OverlayScope(const char* id, const Float2& anchor, const float opacity)
{
  _prepare_overlay_position(anchor);
  ImGui::SetNextWindowBgAlpha(opacity);
  mWindow.emplace(id, kOverlayWindowFlags);
}

auto OverlayScope::is_open() const -> bool
{
  return mWindow->is_open();
}

}  // namespace tactile::core::ui
