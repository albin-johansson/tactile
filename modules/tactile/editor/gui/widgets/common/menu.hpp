#pragma once

#include <tactile-base/tactile_std.hpp>

#include <imgui.h>

namespace Tactile {

class Menu final
{
 public:
  explicit Menu(const CStr name, const bool enabled = true)
      : mOpen{ImGui::BeginMenu(name, enabled)}
  {}

  ~Menu()
  {
    if (mOpen) {
      ImGui::EndMenu();
    }
  }

  explicit operator bool() const noexcept
  {
    return mOpen;
  }

 private:
  bool mOpen{};
};

}  // namespace Tactile
