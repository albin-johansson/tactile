// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/icons.hpp"

#include <IconsFontAwesome6.h>

#define TACTILE_ICON_CREATE ICON_FA_CIRCLE_PLUS
#define TACTILE_ICON_REMOVE ICON_FA_CIRCLE_MINUS
#define TACTILE_ICON_DUPLICATE ICON_FA_CLONE
#define TACTILE_ICON_MOVE_UP ICON_FA_ARROW_UP
#define TACTILE_ICON_MOVE_DOWN ICON_FA_ARROW_DOWN
#define TACTILE_ICON_MOVE_LEFT ICON_FA_ARROW_LEFT
#define TACTILE_ICON_MOVE_RIGHT ICON_FA_ARROW_RIGHT
#define TACTILE_ICON_ALIGN_CENTER ICON_FA_CROSSHAIRS
#define TACTILE_ICON_ZOOM_IN ICON_FA_MAGNIFYING_GLASS_PLUS
#define TACTILE_ICON_ZOOM_OUT ICON_FA_MAGNIFYING_GLASS_MINUS
#define TACTILE_ICON_ZOOM_RESET ICON_FA_MAGNIFYING_GLASS
#define TACTILE_ICON_GRID ICON_FA_TABLE_CELLS
#define TACTILE_ICON_BUG ICON_FA_BUG
#define TACTILE_ICON_TOOLBOX ICON_FA_TOOLBOX
#define TACTILE_ICON_BRUSH ICON_FA_BRUSH

namespace tactile::ui {

auto to_string(const Icon icon) noexcept -> const char*
{
  switch (icon) {
    case Icon::kCreate:      return TACTILE_ICON_CREATE;
    case Icon::kRemove:      return TACTILE_ICON_REMOVE;
    case Icon::kDuplicate:   return TACTILE_ICON_DUPLICATE;
    case Icon::kMoveUp:      return TACTILE_ICON_MOVE_UP;
    case Icon::kMoveDown:    return TACTILE_ICON_MOVE_DOWN;
    case Icon::kMoveLeft:    return TACTILE_ICON_MOVE_LEFT;
    case Icon::kMoveRight:   return TACTILE_ICON_MOVE_RIGHT;
    case Icon::kAlignCenter: return TACTILE_ICON_ALIGN_CENTER;
    case Icon::kZoomIn:      return TACTILE_ICON_ZOOM_IN;
    case Icon::kZoomOut:     return TACTILE_ICON_ZOOM_OUT;
    case Icon::kZoomReset:   return TACTILE_ICON_ZOOM_RESET;
    case Icon::kGrid:        return TACTILE_ICON_GRID;
    case Icon::kBug:         return TACTILE_ICON_BUG;
    case Icon::kToolbox:     return TACTILE_ICON_TOOLBOX;
    case Icon::kBrush:       return TACTILE_ICON_BRUSH;
  }

  return "";
}

auto to_prefix_string(const Icon icon) noexcept -> const char*
{
  switch (icon) {
    case Icon::kCreate:      return TACTILE_ICON_CREATE " ";
    case Icon::kRemove:      return TACTILE_ICON_REMOVE " ";
    case Icon::kDuplicate:   return TACTILE_ICON_DUPLICATE " ";
    case Icon::kMoveUp:      return TACTILE_ICON_MOVE_UP " ";
    case Icon::kMoveDown:    return TACTILE_ICON_MOVE_DOWN " ";
    case Icon::kMoveLeft:    return TACTILE_ICON_MOVE_LEFT " ";
    case Icon::kMoveRight:   return TACTILE_ICON_MOVE_RIGHT " ";
    case Icon::kAlignCenter: return TACTILE_ICON_ALIGN_CENTER " ";
    case Icon::kZoomIn:      return TACTILE_ICON_ZOOM_IN " ";
    case Icon::kZoomOut:     return TACTILE_ICON_ZOOM_OUT " ";
    case Icon::kZoomReset:   return TACTILE_ICON_ZOOM_RESET " ";
    case Icon::kGrid:        return TACTILE_ICON_GRID " ";
    case Icon::kBug:         return TACTILE_ICON_BUG " ";
    case Icon::kToolbox:     return TACTILE_ICON_TOOLBOX " ";
    case Icon::kBrush:       return TACTILE_ICON_BRUSH " ";
  }

  return "";
}

}  // namespace tactile::ui
