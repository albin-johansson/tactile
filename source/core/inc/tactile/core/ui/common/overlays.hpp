// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/ui/common/window.hpp"

namespace tactile::ui {

/**
 * RAII utility that represents an overlay window.
 */
class OverlayScope final
{
 public:
  /**
   * Pushes an overlay window to the widget stack.
   *
   * \param id      The overlay identifier.
   * \param anchor  The anchor position anchor. (0, 0) is top-left, (1,1) is
   *                bottom-right.
   * \param opacity The overlay background opacity.
   */
  OverlayScope(const char* id, const Float2& anchor, float opacity);

  /**
   * Indicates whether the overlay is open.
   *
   * \return
   * True if the overlay is open; false otherwise.
   */
  [[nodiscard]]
  auto is_open() const -> bool;

 private:
  Optional<Window> mWindow;
};

}  // namespace tactile::ui
