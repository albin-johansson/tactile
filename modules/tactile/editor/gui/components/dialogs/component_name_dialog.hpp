#pragma once

#include <tactile_def.hpp>

#include "editor/gui/dialogs/string_input_dialog.hpp"

namespace tactile {

class AComponentNameDialog : public AStringInputDialog {
 protected:
  using AStringInputDialog::AStringInputDialog;

  [[nodiscard]] auto Validate(const Model& model, std::string_view input) const
      -> bool final;
};

}  // namespace tactile