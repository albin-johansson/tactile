#pragma once

#include <entt/entt.hpp>

#include "editor/gui/dialogs/create_map_dialog.hpp"

namespace tactile {

class Model;

/// \addtogroup gui
/// \{

class FileMenu final
{
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

  /* Updates all associated windows. */
  void UpdateWindows(const Model& model, entt::dispatcher& dispatcher);

  void ShowNewMapDialog();

  void ShowOpenMapDialog();

 private:
  create_map_dialog mCreateMapDialog;
  bool mShowOpenMapDialog{};

  void UpdateRecentFilesMenu(entt::dispatcher& dispatcher);
  void UpdateMapFileDialog(entt::dispatcher& dispatcher);
};

/// \} End of group gui

}  // namespace tactile
