#include "properties_dock.hpp"

#include <imgui.h>

#include "core/components/property_context.hpp"
#include "editor/gui/widgets/common/window.hpp"
#include "editor/gui/widgets/properties/dialogs/add_property_dialog.hpp"
#include "editor/gui/widgets/properties/dialogs/change_property_type_dialog.hpp"
#include "editor/gui/widgets/properties/dialogs/rename_property_dialog.hpp"
#include "io/preferences.hpp"
#include "property_table.hpp"

namespace Tactile {
namespace {

constinit bool gHasFocus = false;

}  // namespace

void UpdatePropertiesDock(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowPropertiesDock()) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
  bool isVisible = Prefs::GetShowPropertiesDock();

  Window dock{"Properties", flags, &isVisible};
  gHasFocus = dock.IsFocused();

  if (dock) {
    UpdatePropertyTable(registry, dispatcher);
  }

  UpdateAddPropertyDialog(registry, dispatcher);
  UpdateRenamePropertyDialog(registry, dispatcher);
  UpdateChangePropertyTypeDialog(dispatcher);

  Prefs::SetShowPropertiesDock(isVisible);
}

auto IsPropertyDockFocused() noexcept -> bool
{
  return gHasFocus;
}

}  // namespace Tactile
