#pragma once

#include "editor/gui/dialogs/string_input_dialog.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Used to change the name of an existing layer.
 * \ingroup gui
 */
class rename_layer_dialog final : public string_input_dialog
{
 public:
  rename_layer_dialog();

  void show(layer_id id, std::string oldName);

 protected:
  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto validate(const document_model& model, std::string_view input) const
      -> bool override;

 private:
  maybe<layer_id> mTargetId;
  maybe<std::string> mOldName;
};

}  // namespace tactile
