#pragma once

#include <imgui.h>

#include "aliases/czstring.hpp"

namespace Tactile {

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

}  // namespace Tactile
