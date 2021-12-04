#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

class Model;

/// \addtogroup gui
/// \{

class EditMenu final {
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

  /* Updates all associated windows. */
  void UpdateWindows(entt::dispatcher& dispatcher);

  void OpenSettingsModal();

 private:
  bool mOpenSettings{};
};

/// \} End of group gui

}  // namespace Tactile
