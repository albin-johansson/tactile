#pragma once

#include <tactile_def.hpp>

#include <imgui.h>

namespace Tactile::Scoped {

struct ID final {
  explicit ID(const void* ptr)
  {
    ImGui::PushID(ptr);
  }

  explicit ID(const CStr str)
  {
    ImGui::PushID(str);
  }

  explicit ID(const int id)
  {
    ImGui::PushID(id);
  }

  ~ID()
  {
    ImGui::PopID();
  }
};

struct Disable final {
  explicit Disable(const bool disable = true)
  {
    ImGui::BeginDisabled(disable);
  }

  ~Disable()
  {
    ImGui::EndDisabled();
  }
};

}  // namespace Tactile::Scoped