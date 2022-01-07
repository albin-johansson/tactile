#pragma once

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "rename_layer_dialog.hpp"

namespace Tactile {

class Model;
class Icons;

class LayerDock final {
 public:
  void Update(const Model& model, const Icons& icons, entt::dispatcher& dispatcher);

  void ShowRenameLayerDialog(LayerID id);

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  RenameLayerDialog mRenameLayerDialog;
  Maybe<LayerID> mRenameTarget;
  bool mHasFocus{};
};

}  // namespace Tactile
