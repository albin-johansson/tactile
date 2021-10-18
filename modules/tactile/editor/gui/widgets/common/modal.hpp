#pragma once

#include <imgui.h>

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

class Modal final
{
 public:
  explicit Modal(const CStr name, const ImGuiWindowFlags flags = 0, bool* open = nullptr)
      : mOpen{ImGui::BeginPopupModal(name, open, flags)}
  {}

  ~Modal()
  {
    if (mOpen) {
      ImGui::EndPopup();
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