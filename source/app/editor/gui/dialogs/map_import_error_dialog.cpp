#include "map_import_error_dialog.hpp"

#include <imgui.h>

namespace tactile {

MapImportErrorDialog::MapImportErrorDialog() : dialog_base{"Map import error"}
{
  set_close_button_label(nullptr);
}

void MapImportErrorDialog::Open(const parsing::parse_error error)
{
  mError = error;
  make_visible();
}

void MapImportErrorDialog::on_update(const Model&, entt::dispatcher&)
{
  ImGui::TextUnformatted("Oops, failed to open the specified map!");
  ImGui::Text("Cause: %s", parsing::to_cause(mError.value()).data());
}

}  // namespace tactile
