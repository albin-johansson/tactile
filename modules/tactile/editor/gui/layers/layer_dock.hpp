#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>

#include "rename_layer_dialog.hpp"

namespace Tactile {

class Icons;

class LayerDock final {
 public:
  void Update(const entt::registry& registry,
              const Icons& icons,
              entt::dispatcher& dispatcher);

  void ShowRenameLayerDialog(LayerID id);

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

 private:
  RenameLayerDialog mRenameLayerDialog;
  Maybe<LayerID> mRenameTarget;
  bool mHasFocus{};
};

}  // namespace Tactile
