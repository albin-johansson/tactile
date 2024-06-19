// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/icons.hpp"

#include <IconsFontAwesome6.h>

// clang-format off
#define TACTILE_ICON_NEW_FILE     ICON_FA_FILE_CIRCLE_PLUS
#define TACTILE_ICON_OPEN         ICON_FA_FOLDER_OPEN
#define TACTILE_ICON_CLOSE_FILE   ICON_FA_FILE_CIRCLE_MINUS
#define TACTILE_ICON_SAVE         ICON_FA_FLOPPY_DISK
#define TACTILE_ICON_SAVE_AS      ICON_FA_COPY
#define TACTILE_ICON_EDIT         ICON_FA_PEN_TO_SQUARE
#define TACTILE_ICON_QUIT         ICON_FA_POWER_OFF
#define TACTILE_ICON_CREATE       ICON_FA_SQUARE_PLUS
#define TACTILE_ICON_REMOVE       ICON_FA_SQUARE_MINUS
#define TACTILE_ICON_DUPLICATE    ICON_FA_CLONE
#define TACTILE_ICON_UNDO         ICON_FA_ROTATE_LEFT
#define TACTILE_ICON_REDO         ICON_FA_ROTATE_RIGHT
#define TACTILE_ICON_STAMP        ICON_FA_STAMP
#define TACTILE_ICON_ERASER       ICON_FA_ERASER
#define TACTILE_ICON_BUCKET       ICON_FA_FILL_DRIP
#define TACTILE_ICON_SELECT       ICON_FA_HAND_POINTER
#define TACTILE_ICON_RECTANGLE    ICON_FA_SQUARE
#define TACTILE_ICON_ELLIPSE      ICON_FA_CIRCLE
#define TACTILE_ICON_POINT        ICON_FA_LOCATION_DOT
#define TACTILE_ICON_COMPONENT    ICON_FA_CUBES
#define TACTILE_ICON_SETTINGS     ICON_FA_GEAR
#define TACTILE_ICON_WIDGET       ICON_FA_WINDOW_RESTORE
#define TACTILE_ICON_THEME        ICON_FA_DROPLET
#define TACTILE_ICON_MOVE_UP      ICON_FA_ARROW_UP
#define TACTILE_ICON_MOVE_DOWN    ICON_FA_ARROW_DOWN
#define TACTILE_ICON_MOVE_LEFT    ICON_FA_ARROW_LEFT
#define TACTILE_ICON_MOVE_RIGHT   ICON_FA_ARROW_RIGHT
#define TACTILE_ICON_RESIZE       ICON_FA_CROP_SIMPLE
#define TACTILE_ICON_EYE          ICON_FA_EYE
#define TACTILE_ICON_IMAGE        ICON_FA_IMAGE
#define TACTILE_ICON_EXPORT       ICON_FA_FILE_EXPORT
#define TACTILE_ICON_ALIGN_CENTER ICON_FA_CROSSHAIRS
#define TACTILE_ICON_ZOOM_IN      ICON_FA_MAGNIFYING_GLASS_PLUS
#define TACTILE_ICON_ZOOM_OUT     ICON_FA_MAGNIFYING_GLASS_MINUS
#define TACTILE_ICON_ZOOM_RESET   ICON_FA_MAGNIFYING_GLASS
#define TACTILE_ICON_GRID         ICON_FA_TABLE_CELLS
#define TACTILE_ICON_BUG          ICON_FA_BUG
#define TACTILE_ICON_CODE         ICON_FA_CODE
#define TACTILE_ICON_TOOLBOX      ICON_FA_TOOLBOX
#define TACTILE_ICON_REPAIR       ICON_FA_HAMMER
#define TACTILE_ICON_BRUSH        ICON_FA_BRUSH
#define TACTILE_ICON_INFO         ICON_FA_CIRCLE_INFO
#define TACTILE_ICON_QUESTION     ICON_FA_CIRCLE_QUESTION
#define TACTILE_ICON_HISTORY      ICON_FA_CLOCK_ROTATE_LEFT
#define TACTILE_ICON_GARBAGE      ICON_FA_TRASH_CAN
#define TACTILE_ICON_DEBUG        ICON_FA_BUG_SLASH
#define TACTILE_ICON_PERFORMANCE  ICON_FA_GAUGE_HIGH
#define TACTILE_ICON_STORAGE      ICON_FA_DATABASE
#define TACTILE_ICON_ELLIPSIS     ICON_FA_ELLIPSIS
// clang-format on

namespace tactile::ui {

auto to_string(const Icon icon) noexcept -> const char*
{
  switch (icon) {
    case Icon::kNewFile:     return TACTILE_ICON_NEW_FILE;
    case Icon::kOpen:        return TACTILE_ICON_OPEN;
    case Icon::kCloseFile:   return TACTILE_ICON_CLOSE_FILE;
    case Icon::kSave:        return TACTILE_ICON_SAVE;
    case Icon::kSaveAs:      return TACTILE_ICON_SAVE_AS;
    case Icon::kEdit:        return TACTILE_ICON_EDIT;
    case Icon::kQuit:        return TACTILE_ICON_QUIT;
    case Icon::kCreate:      return TACTILE_ICON_CREATE;
    case Icon::kRemove:      return TACTILE_ICON_REMOVE;
    case Icon::kDuplicate:   return TACTILE_ICON_DUPLICATE;
    case Icon::kUndo:        return TACTILE_ICON_UNDO;
    case Icon::kRedo:        return TACTILE_ICON_REDO;
    case Icon::kStamp:       return TACTILE_ICON_STAMP;
    case Icon::kEraser:      return TACTILE_ICON_ERASER;
    case Icon::kBucket:      return TACTILE_ICON_BUCKET;
    case Icon::kSelect:      return TACTILE_ICON_SELECT;
    case Icon::kRectangle:   return TACTILE_ICON_RECTANGLE;
    case Icon::kEllipse:     return TACTILE_ICON_ELLIPSE;
    case Icon::kPoint:       return TACTILE_ICON_POINT;
    case Icon::kComponent:   return TACTILE_ICON_COMPONENT;
    case Icon::kSettings:    return TACTILE_ICON_SETTINGS;
    case Icon::kWidget:      return TACTILE_ICON_WIDGET;
    case Icon::kTheme:       return TACTILE_ICON_THEME;
    case Icon::kMoveUp:      return TACTILE_ICON_MOVE_UP;
    case Icon::kMoveDown:    return TACTILE_ICON_MOVE_DOWN;
    case Icon::kMoveLeft:    return TACTILE_ICON_MOVE_LEFT;
    case Icon::kMoveRight:   return TACTILE_ICON_MOVE_RIGHT;
    case Icon::kResize:      return TACTILE_ICON_RESIZE;
    case Icon::kEye:         return TACTILE_ICON_EYE;
    case Icon::kImage:       return TACTILE_ICON_IMAGE;
    case Icon::kExport:      return TACTILE_ICON_EXPORT;
    case Icon::kAlignCenter: return TACTILE_ICON_ALIGN_CENTER;
    case Icon::kZoomIn:      return TACTILE_ICON_ZOOM_IN;
    case Icon::kZoomOut:     return TACTILE_ICON_ZOOM_OUT;
    case Icon::kZoomReset:   return TACTILE_ICON_ZOOM_RESET;
    case Icon::kGrid:        return TACTILE_ICON_GRID;
    case Icon::kBug:         return TACTILE_ICON_BUG;
    case Icon::kCode:        return TACTILE_ICON_CODE;
    case Icon::kToolbox:     return TACTILE_ICON_TOOLBOX;
    case Icon::kRepair:      return TACTILE_ICON_REPAIR;
    case Icon::kBrush:       return TACTILE_ICON_BRUSH;
    case Icon::kInfo:        return TACTILE_ICON_INFO;
    case Icon::kQuestion:    return TACTILE_ICON_QUESTION;
    case Icon::kHistory:     return TACTILE_ICON_HISTORY;
    case Icon::kGarbage:     return TACTILE_ICON_GARBAGE;
    case Icon::kDebug:       return TACTILE_ICON_DEBUG;
    case Icon::kPerformance: return TACTILE_ICON_PERFORMANCE;
    case Icon::kStorage:     return TACTILE_ICON_STORAGE;
    case Icon::kEllipsis:    return TACTILE_ICON_ELLIPSIS;
  }

  return "";
}

auto to_prefix_string(const Icon icon) noexcept -> const char*
{
  switch (icon) {
    case Icon::kNewFile:     return TACTILE_ICON_NEW_FILE " ";
    case Icon::kOpen:        return TACTILE_ICON_OPEN " ";
    case Icon::kCloseFile:   return TACTILE_ICON_CLOSE_FILE " ";
    case Icon::kSave:        return TACTILE_ICON_SAVE " ";
    case Icon::kSaveAs:      return TACTILE_ICON_SAVE_AS " ";
    case Icon::kEdit:        return TACTILE_ICON_EDIT " ";
    case Icon::kQuit:        return TACTILE_ICON_QUIT " ";
    case Icon::kCreate:      return TACTILE_ICON_CREATE " ";
    case Icon::kRemove:      return TACTILE_ICON_REMOVE " ";
    case Icon::kDuplicate:   return TACTILE_ICON_DUPLICATE " ";
    case Icon::kUndo:        return TACTILE_ICON_UNDO " ";
    case Icon::kRedo:        return TACTILE_ICON_REDO " ";
    case Icon::kStamp:       return TACTILE_ICON_STAMP " ";
    case Icon::kEraser:      return TACTILE_ICON_ERASER " ";
    case Icon::kBucket:      return TACTILE_ICON_BUCKET " ";
    case Icon::kSelect:      return TACTILE_ICON_SELECT " ";
    case Icon::kRectangle:   return TACTILE_ICON_RECTANGLE " ";
    case Icon::kEllipse:     return TACTILE_ICON_ELLIPSE " ";
    case Icon::kPoint:       return TACTILE_ICON_POINT " ";
    case Icon::kComponent:   return TACTILE_ICON_COMPONENT " ";
    case Icon::kSettings:    return TACTILE_ICON_SETTINGS " ";
    case Icon::kWidget:      return TACTILE_ICON_WIDGET " ";
    case Icon::kTheme:       return TACTILE_ICON_THEME " ";
    case Icon::kMoveUp:      return TACTILE_ICON_MOVE_UP " ";
    case Icon::kMoveDown:    return TACTILE_ICON_MOVE_DOWN " ";
    case Icon::kMoveLeft:    return TACTILE_ICON_MOVE_LEFT " ";
    case Icon::kMoveRight:   return TACTILE_ICON_MOVE_RIGHT " ";
    case Icon::kResize:      return TACTILE_ICON_RESIZE " ";
    case Icon::kEye:         return TACTILE_ICON_EYE " ";
    case Icon::kImage:       return TACTILE_ICON_IMAGE " ";
    case Icon::kExport:      return TACTILE_ICON_EXPORT " ";
    case Icon::kAlignCenter: return TACTILE_ICON_ALIGN_CENTER " ";
    case Icon::kZoomIn:      return TACTILE_ICON_ZOOM_IN " ";
    case Icon::kZoomOut:     return TACTILE_ICON_ZOOM_OUT " ";
    case Icon::kZoomReset:   return TACTILE_ICON_ZOOM_RESET " ";
    case Icon::kGrid:        return TACTILE_ICON_GRID " ";
    case Icon::kBug:         return TACTILE_ICON_BUG " ";
    case Icon::kCode:        return TACTILE_ICON_CODE " ";
    case Icon::kToolbox:     return TACTILE_ICON_TOOLBOX " ";
    case Icon::kRepair:      return TACTILE_ICON_REPAIR " ";
    case Icon::kBrush:       return TACTILE_ICON_BRUSH " ";
    case Icon::kInfo:        return TACTILE_ICON_INFO " ";
    case Icon::kQuestion:    return TACTILE_ICON_QUESTION " ";
    case Icon::kHistory:     return TACTILE_ICON_HISTORY " ";
    case Icon::kGarbage:     return TACTILE_ICON_GARBAGE " ";
    case Icon::kDebug:       return TACTILE_ICON_DEBUG " ";
    case Icon::kPerformance: return TACTILE_ICON_PERFORMANCE " ";
    case Icon::kStorage:     return TACTILE_ICON_STORAGE " ";
    case Icon::kEllipsis:    return TACTILE_ICON_ELLIPSIS " ";
  }

  return "";
}

}  // namespace tactile::ui
