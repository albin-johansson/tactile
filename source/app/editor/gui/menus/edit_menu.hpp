#pragma once

#include <entt/entt.hpp>

#include "editor/gui/components/component_editor.hpp"
#include "editor/gui/dialogs/settings_dialog.hpp"

namespace tactile {

class document_model;

/// \addtogroup gui
/// \{

class EditMenu final
{
 public:
  void Update(const document_model& model, entt::dispatcher& dispatcher);

  /* Updates all associated windows. */
  void UpdateWindows(const document_model& model, entt::dispatcher& dispatcher);

  void OpenSettingsModal();

  void ShowComponentEditor(const document_model& model);

 private:
  settings_dialog mSettingsDialog;
  component_editor mComponentEditor;
};

/// \} End of group gui

}  // namespace tactile
