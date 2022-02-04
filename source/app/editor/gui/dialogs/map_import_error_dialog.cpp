#include "map_import_error_dialog.hpp"

#include <imgui.h>

namespace tactile {

MapImportErrorDialog::MapImportErrorDialog() : ADialog{"Map import error"}
{
  SetCloseButtonLabel(nullptr);
}

void MapImportErrorDialog::Open(const parsing::parse_error error)
{
  mError = error;
  Show();
}

void MapImportErrorDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  ImGui::TextUnformatted("Oops, failed to open the specified map!");
  ImGui::Text("Cause: %s", parsing::to_cause(mError.value()).data());
}

}  // namespace tactile
