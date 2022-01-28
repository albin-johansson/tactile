#pragma once

#include <IconsFontAwesome5.h>

#include "core/utils/texture_manager.hpp"
#include "tactile_def.hpp"

/// \addtogroup gui
/// \{

/// \name Icon literals
/// \{

#define TAC_ICON_UNDO ICON_FA_UNDO_ALT
#define TAC_ICON_REDO ICON_FA_REDO_ALT

#define TAC_ICON_FILE ICON_FA_FILE
#define TAC_ICON_OPEN ICON_FA_FOLDER_OPEN
#define TAC_ICON_CLOSE ICON_FA_TIMES_CIRCLE
#define TAC_ICON_SELECT_FILE ICON_FA_ELLIPSIS_H
#define TAC_ICON_SAVE ICON_FA_SAVE
#define TAC_ICON_SETTINGS ICON_FA_COG
#define TAC_ICON_EXIT ICON_FA_WINDOW_CLOSE

#define TAC_ICON_COPY ICON_FA_COPY
#define TAC_ICON_PASTE ICON_FA_PASTE

#define TAC_ICON_ADD ICON_FA_PLUS_CIRCLE
#define TAC_ICON_REMOVE ICON_FA_MINUS_CIRCLE
#define TAC_ICON_EDIT ICON_FA_EDIT
#define TAC_ICON_DUPLICATE ICON_FA_CLONE
#define TAC_ICON_RESIZE ICON_FA_EXPAND_ARROWS_ALT

#define TAC_ICON_MOVE_UP ICON_FA_ARROW_UP
#define TAC_ICON_MOVE_DOWN ICON_FA_ARROW_DOWN
#define TAC_ICON_MOVE_LEFT ICON_FA_ARROW_LEFT
#define TAC_ICON_MOVE_RIGHT ICON_FA_ARROW_RIGHT

#define TAC_ICON_INSPECT ICON_FA_INFO_CIRCLE
#define TAC_ICON_PROPERTIES ICON_FA_CODE
#define TAC_ICON_VISIBILITY ICON_FA_EYE
#define TAC_ICON_OPACITY ICON_FA_CHESS_BOARD

#define TAC_ICON_TILE_LAYER ICON_FA_TH
#define TAC_ICON_OBJECT_LAYER ICON_FA_SHAPES
#define TAC_ICON_GROUP_LAYER ICON_FA_LAYER_GROUP

#define TAC_ICON_METRICS ICON_FA_TACHOMETER_ALT
#define TAC_ICON_ABOUT ICON_FA_QUESTION_CIRCLE

#define TAC_ICON_STAMP ICON_FA_STAMP
#define TAC_ICON_BUCKET ICON_FA_FILL
#define TAC_ICON_ERASER ICON_FA_ERASER
#define TAC_ICON_OBJECT_SELECTION ICON_FA_OBJECT_GROUP

#define TAC_ICON_TILESET ICON_FA_IMAGE
#define TAC_ICON_GRID ICON_FA_TH

#define TAC_ICON_CENTER ICON_FA_CROSSHAIRS
#define TAC_ICON_ZOOM_IN ICON_FA_SEARCH_PLUS
#define TAC_ICON_ZOOM_OUT ICON_FA_SEARCH_MINUS
#define TAC_ICON_RESET_ZOOM ICON_FA_SEARCH

#define TAC_ICON_LINK ICON_FA_LINK
#define TAC_ICON_BUG ICON_FA_BUG
#define TAC_ICON_ERROR ICON_FA_EXCLAMATION_TRIANGLE

#define TAC_ICON_HISTORY ICON_FA_HISTORY
#define TAC_ICON_CLEAR_HISTORY ICON_FA_TRASH_ALT

#define TAC_ICON_RESET ICON_FA_SLIDERS_H
#define TAC_ICON_THREE_DOTS ICON_FA_ELLIPSIS_V

#define TAC_ICON_COMPONENT ICON_FA_CUBES

/// \} End of icon literals

/// \} End of group gui

namespace tactile {

/// \addtogroup gui
/// \{

/// \name Icon API
/// \{

class Icons final {
 public:
  explicit Icons(texture_manager& textures);

  /**
   * \brief Returns the icon string literal for the specified layer type.
   *
   * \param type the layer type to obtain the icon for.
   *
   * \return an icon literal for the specified layer type.
   *
   * \throws TactileError if the layer type isn't recognized.
   */
  [[nodiscard]] auto GetIcon(LayerType type) const -> CStr;

  /**
   * \brief Returns the texture identifier for the Tactile icon.
   *
   * \return the OpenGL texture ID for the Tactile icon.
   */
  [[nodiscard]] auto GetTactileIcon() const noexcept -> uint { return mTactileIcon; }

 private:
  uint mTactileIcon{};
};

/// \} End of icon API

/// \} End of group gui

}  // namespace tactile
