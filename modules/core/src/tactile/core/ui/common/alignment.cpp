// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/alignment.hpp"

#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/debug/validation.hpp"

namespace tactile::core {
namespace {

inline constexpr float kMinFloat = std::numeric_limits<float>::min();

}  // namespace

auto calculate_item_width(const char* text) -> float
{
  TACTILE_ASSERT(text);
  return ImGui::CalcTextSize(text).x + (ImGui::GetStyle().FramePadding.x * 2.0f);
}

void right_align_next_item()
{
  right_align_next_item(-kMinFloat);
}

auto right_align_next_item(const float item_width) -> float
{
  const auto cursor_x =
      ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - item_width);
  ImGui::SetCursorPosX(cursor_x);
  return cursor_x;
}

auto right_align_next_item(const char* text) -> float
{
  return right_align_next_item(calculate_item_width(text));
}

void center_next_item_horizontally(const float item_width)
{
  const auto available_region = ImGui::GetContentRegionAvail();
  const auto half_available_region = available_region * ImVec2 {0.5f, 0.5f};
  ImGui::SetCursorPos(ImGui::GetCursorPos() +
                      ImVec2 {half_available_region.x - (item_width * 0.5f), 0});
}

void center_next_window_on_appearance()
{
  const auto* viewport = require_not_null(ImGui::GetMainViewport(), "null viewport");

  const auto next_pos = viewport->WorkPos + (viewport->WorkSize * ImVec2 {0.5f, 0.5f});
  const auto next_pivot = ImVec2 {0.5f, 0.5f};

  ImGui::SetNextWindowPos(next_pos, ImGuiCond_Appearing, next_pivot);
}

void span_next_item_width_to_end()
{
  ImGui::SetNextItemWidth(-kMinFloat);
}

void prepare_for_vertically_center_aligned_items(const float count)
{
  const auto available_region = ImGui::GetContentRegionAvail();
  const auto font_size = ImGui::GetFontSize();
  const auto frame_padding = ImGui::GetStyle().FramePadding;

  const auto item_height = font_size + (2.0f * frame_padding.y);
  const auto total_height = count * item_height;

  const auto cursor_pos = ImGui::GetCursorPos();
  const auto cursor_offset = ImVec2 {0, (available_region.y - total_height) * 0.5f};

  ImGui::SetCursorPos(cursor_pos + cursor_offset);
}

}  // namespace tactile::core
