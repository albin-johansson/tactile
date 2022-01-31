#pragma once

#include "dialog.hpp"
#include "io/persistence/preferences.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup gui
/// \{

class SettingsDialog final : public ADialog {
 public:
  TACTILE_DEFAULT_COPY(SettingsDialog)
  TACTILE_DEFAULT_MOVE(SettingsDialog)

  SettingsDialog();

  ~SettingsDialog() override = default;

  void Open();

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

  void OnCancel() override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  void OnApply(entt::dispatcher& dispatcher) override;

 private:
  preference_state mSnapshot;     ///< The original settings when the dialog was opened.
  preference_state mGuiSettings;  ///< The value of the settings in the GUI.

  void ApplySettings(entt::dispatcher& dispatcher);

  void UpdateBehaviorTab();

  void UpdateAppearanceTab();

  void UpdateExportTab();
};

/// \} End of group gui

}  // namespace tactile
