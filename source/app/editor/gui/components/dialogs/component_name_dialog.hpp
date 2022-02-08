#pragma once

#include "editor/gui/dialogs/string_input_dialog.hpp"
#include "tactile.hpp"

namespace tactile {

class component_name_dialog : public string_input_dialog
{
 protected:
  using string_input_dialog::string_input_dialog;

  [[nodiscard]] auto validate(const Model& model, std::string_view input) const
      -> bool final;
};

}  // namespace tactile