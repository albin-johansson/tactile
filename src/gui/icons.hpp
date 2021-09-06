#pragma once

#include <IconsFontAwesome5.h>

#include "aliases/cstr.hpp"
#include "aliases/ints.hpp"
#include "core/layer_type.hpp"

/// \addtogroup gui
/// \{

/// \name Icon literals
/// \{

#define TAC_ICON_UNDO ICON_FA_UNDO
#define TAC_ICON_REDO ICON_FA_REDO

#define TAC_ICON_FILE ICON_FA_FILE
#define TAC_ICON_OPEN ICON_FA_FOLDER_OPEN
#define TAC_ICON_SAVE ICON_FA_SAVE
#define TAC_ICON_SETTINGS ICON_FA_COG
#define TAC_ICON_EXIT ICON_FA_WINDOW_CLOSE

#define TAC_ICON_COPY ICON_FA_COPY
#define TAC_ICON_PASTE ICON_FA_PASTE

#define TAC_ICON_ADD ICON_FA_PLUS_CIRCLE
#define TAC_ICON_REMOVE ICON_FA_MINUS_CIRCLE
#define TAC_ICON_EDIT ICON_FA_EDIT
#define TAC_ICON_DUPLICATE ICON_FA_CLONE
#define TAC_ICON_RESIZE ICON_FA_CROP_ALT

#define TAC_ICON_MOVE_UP ICON_FA_ARROW_UP
#define TAC_ICON_MOVE_DOWN ICON_FA_ARROW_DOWN
#define TAC_ICON_MOVE_LEFT ICON_FA_ARROW_LEFT
#define TAC_ICON_MOVE_RIGHT ICON_FA_ARROW_RIGHT

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

#define TAC_ICON_TILESET ICON_FA_IMAGE
#define TAC_ICON_GRID ICON_FA_TH

#define TAC_ICON_CENTER ICON_FA_CROSSHAIRS
#define TAC_ICON_ZOOM_IN ICON_FA_SEARCH_PLUS
#define TAC_ICON_ZOOM_OUT ICON_FA_SEARCH_MINUS
#define TAC_ICON_RESET_ZOOM ICON_FA_SEARCH

#define TAC_ICON_LINK ICON_FA_LINK

/// \} End of icon literals

/// \} End of group gui

namespace Tactile {

/// \addtogroup gui
/// \{

/// \name Icon API
/// \{

/**
 * \brief Returns the texture identifier for the Tactile icon.
 *
 * \return the OpenGL texture ID for the Tactile icon.
 */
[[nodiscard]] auto GetTactileIcon() noexcept -> uint;

/**
 * \brief Returns the icon string literal for the specified layer type.
 *
 * \param type the layer type to obtain the icon for.
 *
 * \return an icon literal for the specified layer type.
 *
 * \throws TactileError if the layer type isn't recognized.
 */
[[nodiscard]] auto GetIcon(LayerType type) -> CStr;

/**
 * \brief Simple RAII wrapper for loading and unloading icons.
 */
class Icons final
{
 public:
  /**
   * \brief Loads all icons.
   *
   * \throws TactileError if any of the icons couldn't be loaded.
   */
  Icons();

  /// Unloads all icons.
  ~Icons();
};

/// \} End of icon API

/// \} End of group gui

}  // namespace Tactile
