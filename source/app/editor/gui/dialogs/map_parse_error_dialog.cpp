#include "map_parse_error_dialog.hpp"

#include <imgui.h>

#include "editor/gui/icons.hpp"

namespace tactile {

map_parse_error_dialog::map_parse_error_dialog() : dialog_base{"Map Parse Error"}
{
  set_close_button_label(nullptr);
}

void map_parse_error_dialog::show(const parsing::parse_error error)
{
  mError = error;
  make_visible();
}

void map_parse_error_dialog::on_update(const Model&, entt::dispatcher&)
{
  ImGui::TextUnformatted(
      "Oops, something went wrong when parsing the map! " ICON_FA_SAD_TEAR);
  ImGui::Text("Cause: %s", parsing::to_cause(mError.value()).data());
}

}  // namespace tactile
