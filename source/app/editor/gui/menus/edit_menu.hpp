#pragma once

#include <entt/entt.hpp>

#include "editor/gui/components/component_editor.hpp"
#include "editor/gui/dialogs/settings_dialog.hpp"

namespace tactile {

class Model;

/// \addtogroup gui
/// \{

class EditMenu final {
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

  /* Updates all associated windows. */
  void UpdateWindows(const Model& model, entt::dispatcher& dispatcher);

  void OpenSettingsModal();

  void ShowComponentEditor(const Model& model);

 private:
  SettingsDialog mSettingsDialog;
  component_editor mComponentEditor;
};

/// \} End of group gui

}  // namespace tactile
