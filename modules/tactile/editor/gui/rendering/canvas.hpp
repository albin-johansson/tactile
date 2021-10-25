#pragma once

#include <imgui.h>

namespace Tactile {

/// \addtogroup rendering
/// \{

/**
 * \brief Provides useful information when rendering to a canvas.
 *
 * \see `GetCanvasInfo()`
 * \see `ClearBackground()`
 */
struct CanvasInfo final
{
  ImVec2 size{};
  ImVec2 tl{};  /// Top-left
  ImVec2 br{};  /// Bottom-right
};

/**
 * \brief Returns information about the current canvas.
 *
 * \return information about the current canvas.
 */
[[nodiscard]] auto GetCanvasInfo() -> CanvasInfo;

/**
 * \brief Fills an entire canvas.
 *
 * \param canvas the canvas that will be filled with a background color.
 */
void ClearBackground(const CanvasInfo& canvas);

/// \} End of group rendering

}  // namespace Tactile
