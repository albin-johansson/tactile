#pragma once

#include "dialog.hpp"
#include "io/persistence/preferences.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Provides the interface for all available persistent settings.
 *
 * \ingroup gui
 */
class settings_dialog final : public dialog_base
{
 public:
  TACTILE_DEFAULT_COPY(settings_dialog)
  TACTILE_DEFAULT_MOVE(settings_dialog)

  settings_dialog();

  ~settings_dialog() override = default;

  void show();

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;

  void on_cancel() override;

  void on_accept(entt::dispatcher& dispatcher) override;

  void on_apply(entt::dispatcher& dispatcher) override;

 private:
  preference_state mSnapshot;     ///< The original settings when the dialog was opened.
  preference_state mGuiSettings;  ///< The value of the settings in the GUI.

  void apply_settings(entt::dispatcher& dispatcher);

  void update_behavior_tab();

  void update_appearance_tab();

  void update_export_tab();
};

}  // namespace tactile
