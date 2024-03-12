// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "map_parse_error_dialog.hpp"

#include <fmt/format.h>
#include <imgui.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/base/container/string.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

struct MapParseErrorDialogState final {
  String cause;
  bool open_dialog {};
};

inline MapParseErrorDialogState gDialogState;

}  // namespace

void open_map_parse_error_dialog(const ParseError error)
{
  const auto& lang = get_current_language();
  gDialogState.cause = fmt::format("{}: {}", lang.misc.cause, to_cause(error));
  gDialogState.open_dialog = true;
}

void update_map_parse_error_dialog()
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.map_parse_error.c_str(),
      .close_label = lang.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  if (const ScopedDialog dialog {options}; dialog.was_opened()) {
    ImGui::TextUnformatted(lang.misc.map_parse_error.c_str());
    ImGui::TextUnformatted(gDialogState.cause.c_str());
  }
}

}  // namespace tactile::ui
