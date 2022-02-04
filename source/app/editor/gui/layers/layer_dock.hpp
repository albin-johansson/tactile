#pragma once

#include <entt/entt.hpp>

#include "dialogs/rename_layer_dialog.hpp"
#include "tactile_def.hpp"

namespace tactile {

class Model;
class Icons;

class LayerDock final {
 public:
  void Update(const Model& model, const Icons& icons, entt::dispatcher& dispatcher);

  void ShowRenameLayerDialog(layer_id id);

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  RenameLayerDialog mRenameLayerDialog;
  maybe<layer_id> mRenameTarget;
  bool mHasFocus{};
};

}  // namespace tactile
