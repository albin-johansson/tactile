#pragma once

#include <tactile_def.hpp>

#include <imgui.h>

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