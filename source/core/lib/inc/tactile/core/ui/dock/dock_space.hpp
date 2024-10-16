// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path
#include <optional>    // optional

#include "tactile/base/prelude.hpp"

namespace tactile::core::ui {

class Language;

/**
 * Manages the ImGui "dock space", handling the layout of dock widgets.
 */
class DockSpace final
{
 public:
  DockSpace();

  /**
   * Updates the state of the dock space.
   *
   * \details
   * This function should be called once per frame.
   *
   * \param language The current language.
   */
  void update(const Language& language);

  /**
   * Resets the current layout to the default state.
   *
   * \param language The current language.
   */
  void reset_layout(const Language& language);

 private:
  std::filesystem::path mImGuiIniPath;
  std::optional<unsigned> mRootId {};
  bool mDidInit {false};
};

}  // namespace tactile::core::ui
