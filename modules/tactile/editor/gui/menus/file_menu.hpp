#pragma once

#include <entt/entt.hpp>

namespace Tactile {

class Model;

/// \addtogroup gui
/// \{

class FileMenu final {
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

  /* Updates all associated windows. */
  void UpdateWindows(entt::dispatcher& dispatcher);

  void ShowNewMapDialog();

  void ShowOpenMapDialog();

 private:
  bool mShowNewMapDialog{};
  bool mShowOpenMapDialog{};

  void UpdateRecentFilesMenu(entt::dispatcher& dispatcher);
  void UpdateMapFileDialog(entt::dispatcher& dispatcher);
};

/// \} End of group gui

}  // namespace Tactile
