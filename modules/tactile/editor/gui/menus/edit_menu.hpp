#pragma once

#include <entt/entt.hpp>

#include "editor/gui/dialogs/settings_dialog.hpp"

namespace Tactile {

class Model;

/// \addtogroup gui
/// \{

class EditMenu final {
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

  /* Updates all associated windows. */
  void UpdateWindows(const Model& model, entt::dispatcher& dispatcher);

  void OpenSettingsModal();

 private:
  SettingsDialog mSettingsDialog;
};

/// \} End of group gui

}  // namespace Tactile
