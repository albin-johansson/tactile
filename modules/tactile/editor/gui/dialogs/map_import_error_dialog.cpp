#include "map_import_error_dialog.hpp"

#include <imgui.h>

namespace Tactile {

MapImportErrorDialog::MapImportErrorDialog() : ADialog{"Map import error"}
{
  SetCloseButtonLabel(nullptr);
}

void MapImportErrorDialog::Open(const IO::ParseError error)
{
  mError = error;
  Show();
}

void MapImportErrorDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  ImGui::TextUnformatted("Oops, failed to open the specified map!");
  ImGui::Text("Cause: %s", IO::GetCause(mError.value()));
}

}  // namespace Tactile
