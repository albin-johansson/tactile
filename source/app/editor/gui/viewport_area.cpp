#include "viewport_area.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/mouse_info.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto check_for(auto&& query) -> maybe<cen::mouse_button>
{
  const auto left = query(ImGuiMouseButton_Left);
  const auto mid = query(ImGuiMouseButton_Middle);
  const auto right = query(ImGuiMouseButton_Right);

  if (left) {
    return cen::mouse_button::left;
  }
  else if (mid) {
    return cen::mouse_button::middle;
  }
  else if (right) {
    return cen::mouse_button::right;
  }
  else {
    return nothing;
  }
}

}  // namespace

auto viewport_area::get_size() const -> ImVec2
{
  const auto pos = ImGui::GetWindowPos();
  return (pos + ImGui::GetWindowSize()) - pos;
}

void viewport_area::check_mouse_events(const entt::registry& registry,
                                       entt::dispatcher& dispatcher)
{
  mContainsMouse = false;

  if (!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered()) {
    return;
  }

  ImGui::InvisibleButton("InvisibleViewportAreaButton", get_size());

  if (ImGui::IsItemHovered()) {
    mContainsMouse = true;

    check_clicked(registry, dispatcher);
    check_dragged(registry, dispatcher);
    check_released(registry, dispatcher);
  }
}

auto viewport_area::contains_mouse() const -> bool
{
  return mContainsMouse;
}

void viewport_area::check_clicked(const entt::registry& registry,
                                  entt::dispatcher& dispatcher)
{
  const auto button = check_for(
      [](const ImGuiMouseButton button) { return ImGui::IsMouseClicked(button); });

  if (button) {
    on_clicked(*button, registry, dispatcher);
  }
}

void viewport_area::check_dragged(const entt::registry& registry,
                                  entt::dispatcher& dispatcher)
{
  const auto button = check_for(
      [](const ImGuiMouseButton button) { return ImGui::IsMouseDragging(button); });

  if (button) {
    on_dragged(*button, registry, dispatcher);
  }
}

void viewport_area::check_released(const entt::registry& registry,
                                   entt::dispatcher& dispatcher)
{
  const auto button = check_for(
      [](const ImGuiMouseButton button) { return ImGui::IsMouseReleased(button); });

  if (button) {
    on_released(*button, registry, dispatcher);
  }
}

}  // namespace tactile