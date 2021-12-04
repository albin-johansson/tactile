#pragma once

#include <tactile_def.hpp>

#include <imgui.h>

namespace Tactile {

struct ScopeID final {
  explicit ScopeID(const void* ptr)
  {
    ImGui::PushID(ptr);
  }

  explicit ScopeID(const CStr str)
  {
    ImGui::PushID(str);
  }

  explicit ScopeID(const int id)
  {
    ImGui::PushID(id);
  }

  ~ScopeID()
  {
    ImGui::PopID();
  }
};

}  // namespace Tactile
