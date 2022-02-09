#pragma once

#include <entt/entt.hpp>
#include <imgui.h>

#include "tactile.hpp"

namespace tactile {

class document_model;

class dock_widget
{
 public:
  TACTILE_DEFAULT_COPY(dock_widget)
  TACTILE_DEFAULT_MOVE(dock_widget)

  dock_widget(const char* title, ImGuiWindowFlags flags);

  virtual ~dock_widget() noexcept = default;

  void update(const document_model& model, entt::dispatcher& dispatcher);

  [[nodiscard]] auto has_focus() const noexcept -> bool { return mHasFocus; }

 protected:
  virtual void on_update([[maybe_unused]] const document_model& model,
                         [[maybe_unused]] entt::dispatcher& dispatcher)
  {}

  void set_close_button_enabled(bool enabled);

  void set_focus_flags(ImGuiFocusedFlags flags);

  virtual void set_visible(bool visible) = 0;

  [[nodiscard]] virtual auto is_visible() const -> bool = 0;

 private:
  const char* mTitle{};
  ImGuiWindowFlags mWindowFlags{};
  ImGuiFocusedFlags mFocusFlags{};
  bool mHasFocus{};
  bool mHasCloseButton{};
};

}  // namespace tactile