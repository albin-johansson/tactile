#pragma once

#include "aliases/czstring.hpp"
#include "imgui.h"

namespace tactile {

struct ScopeID final
{
  explicit ScopeID(const void* ptr)
  {
    ImGui::PushID(ptr);
  }

  explicit ScopeID(const czstring str)
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

}  // namespace tactile
