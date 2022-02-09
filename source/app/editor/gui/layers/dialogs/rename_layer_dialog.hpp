#pragma once

#include <entt/entt.hpp>

#include "editor/gui/dialogs/string_input_dialog.hpp"
#include "tactile.hpp"

namespace tactile {

class RenameLayerDialog final : public string_input_dialog
{
 public:
  RenameLayerDialog();

  ~RenameLayerDialog() override = default;

  void Show(layer_id id, std::string oldName);

 protected:
  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto validate(const document_model& model, std::string_view input) const
      -> bool override;

 private:
  maybe<layer_id> mTargetId;
  maybe<std::string> mOldName;
};

}  // namespace tactile
