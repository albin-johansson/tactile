#pragma once

#include <imgui.h>

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

struct ScopeID final
{
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
