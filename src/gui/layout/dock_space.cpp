#include "dock_space.hpp"

#include "aliases/maybe.hpp"
#include "load_default_layout.hpp"

namespace Tactile {
namespace {

inline bool initialized = false;
inline Maybe<ImGuiID> root_id;

}  // namespace

void UpdateDockSpace()
{
  root_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  if (!initialized)
  {
    const auto size = ImGui::GetMainViewport()->Size;
    if (size.x > 0 && size.y > 0)
    {
      LoadDefaultLayout(root_id.value(), false);
      initialized = true;
    }
  }
}

void ResetLayout()
{
  LoadDefaultLayout(root_id.value(), true);
}

}  // namespace Tactile
