#pragma once

#include <entt/entt.hpp>
#include <imgui.h>

namespace tactile {

class dock_widget
{
 public:
  dock_widget(const char* title, ImGuiWindowFlags flags);

  virtual ~dock_widget() noexcept = default;

  void update(const entt::registry& registry, entt::dispatcher& dispatcher);

 protected:
  virtual void on_update([[maybe_unused]] const entt::registry& registry,
                         [[maybe_unused]] entt::dispatcher& dispatcher)
  {}

  void set_close_button_enabled(bool enabled);

  virtual void set_visible(bool visible) = 0;

  [[nodiscard]] virtual auto is_visible() const -> bool = 0;

 private:
  const char* mTitle{};
  ImGuiWindowFlags mFlags{};
  bool mHasFocus{};
  bool mHasCloseButton{};
};

}  // namespace tactile