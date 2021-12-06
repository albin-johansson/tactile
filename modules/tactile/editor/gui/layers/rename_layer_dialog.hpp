#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>

#include "editor/gui/dialogs/rename_dialog.hpp"

namespace Tactile {

class RenameLayerDialog final : public ARenameDialog {
 public:
  RenameLayerDialog();

  ~RenameLayerDialog() override = default;

  void Show(LayerID id, std::string oldName);

 protected:
  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto Validate(const entt::registry& registry,
                              std::string_view input) const -> bool override;

 private:
  Maybe<LayerID> mTargetId;
  Maybe<std::string> mOldName;
};

}  // namespace Tactile
