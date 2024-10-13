// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path
#include <string>      // string

#include "tactile/base/layer/layer_type.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/map/map_spec.hpp"
#include "tactile/core/ui/fonts.hpp"
#include "tactile/core/ui/theme.hpp"

namespace tactile::core {

/**
 * Event for opening the map document creation dialog.
 */
struct ShowNewMapDialogEvent final
{};

/**
 * Event for opening the map document loading dialog.
 */
struct ShowOpenMapDialogEvent final
{};

/**
 * Event for opening the 'Save As' dialog.
 */
struct ShowSaveAsDialogEvent final
{};

/**
 * Event for opening the settings dialog.
 */
struct ShowSettingsDialogEvent final
{};

/**
 * Event for opening the "About Tactile" dialog.
 */
struct ShowAboutDialogEvent final
{};

/**
 * Event for opening the "Credits" dialog.
 */
struct ShowCreditsDialogEvent final
{};

/**
 * Event for opening the dialog for renaming a layer in the active map.
 */
struct ShowRenameLayerDialogEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

/**
 * Event for opening the component editor dialog.
 */
struct ShowComponentEditorDialogEvent final
{};

/**
 * Event for opening the dialog for creating a new property.
 */
struct ShowNewPropertyDialogEvent final
{
  /** The target meta context. */
  EntityID context_entity;
};

/**
 * Event for opening the dialog for renaming a property.
 */
struct ShowRenamePropertyDialogEvent final
{
  /** The associated meta context. */
  EntityID context_entity;

  /** The target property. */
  std::string name;
};

/**
 * Event for resetting the dock widget layout.
 */
struct ResetLayoutEvent final
{};

/**
 * Event for toggling the visibility of dock widgets.
 */
struct ToggleUiEvent final
{};

/**
 * Event for toggling the visibility of the property dock widget.
 */
struct TogglePropertyDockEvent final
{};

/**
 * Event for toggling the visibility of the component dock widget.
 */
struct ToggleComponentDockEvent final
{};

/**
 * Event for toggling the visibility of the layer dock widget.
 */
struct ToggleLayerDockEvent final
{};

/**
 * Event for toggling the visibility of the tileset dock widget.
 */
struct ToggleTilesetDockEvent final
{};

/**
 * Event for toggling the visibility of the animation dock widget.
 */
struct ToggleAnimationDockEvent final
{};

/**
 * Event for toggling the visibility of the log dock widget.
 */
struct ToggleLogDockEvent final
{};

/**
 * Event for changing the active editor theme.
 */
struct SetThemeEvent final
{
  ui::Theme theme;
};

/**
 * Event for gradually increasing the editor font size.
 */
struct IncreaseFontSizeEvent final
{};

/**
 * Event for gradually decreasing the editor font size.
 */
struct DecreaseFontSizeEvent final
{};

/**
 * Event for resetting the editor font size.
 */
struct ResetFontSizeEvent final
{};

/**
 * Event for setting the editor font.
 */
struct SetFontEvent final
{
  /** The selected font. */
  ui::FontID font;
};

/**
 * Event for reloading the current fonts.
 */
struct ReloadFontsEvent final
{
  /** The current framebuffer scale (usually either 1 or 2). */
  float framebuffer_scale;
};

/**
 * Event for toggling the visibility of the viewport grid.
 */
struct ToggleGridEvent final
{};

/**
 * Event for toggling the highlighting of the active layer.
 */
struct ToggleLayerHighlightEvent final
{};

/**
 * Event for creating and attaching a property to a meta context.
 */
struct CreatePropertyEvent final
{
  /** The target meta context. */
  EntityID context_entity;

  /** The new property name. */
  std::string name;

  /** The initial value of the property. */
  Attribute value;
};

/**
 * Event for updating the value of a property in a meta context.
 */
struct UpdatePropertyEvent final
{
  /** The target meta context. */
  EntityID context_entity;

  /** The target property name. */
  std::string name;

  /** The new value of the property. */
  Attribute value;
};

/**
 * Event for removing a property from a meta context.
 */
struct RemovePropertyEvent final
{
  /** The target meta context. */
  EntityID context_entity;

  /** The target property name. */
  std::string name;
};

/**
 * Event for renaming a property in a meta context.
 */
struct RenamePropertyEvent final
{
  /** The target meta context. */
  EntityID context_entity;

  /** The old property name. */
  std::string old_name;

  /** The new property name. */
  std::string new_name;
};

/**
 * Event for creating a new component.
 */
struct CreateComponentEvent final
{
  std::string name;
};

/**
 * Event for saving the current document to disk.
 */
struct SaveEvent final
{};

/**
 * Event for saving the current document to a new location.
 */
struct SaveAsEvent final
{
  /** The chosen file path. */
  std::filesystem::path path;
};

/**
 * Event for reopening the last closed file.
 */
struct ReopenLastClosedFileEvent final
{};

/**
 * Event for clearing the file history.
 */
struct ClearFileHistoryEvent final
{};

/**
 * Event for closing the current document.
 */
struct CloseEvent final
{};

/**
 * Event for gracefully shutting down the application.
 */
struct QuitEvent final
{};

/**
 * Event for undoing the most recent command.
 */
struct UndoEvent final
{};

/**
 * Event for redoing the most recently reverted command.
 */
struct RedoEvent final
{};

/**
 * Event for enabling the stamp tool.
 */
struct EnableStampToolEvent final
{};

/**
 * Event for enabling the eraser tool.
 */
struct EnableEraserToolEvent final
{};

/**
 * Event for enabling the bucket fill tool.
 */
struct EnableBucketToolEvent final
{};

/**
 * Event for enabling the object selector tool.
 */
struct EnableSelectorToolEvent final
{};

/**
 * Event for enabling the rectangle tool.
 */
struct EnableRectangleToolEvent final
{};

/**
 * Event for enabling the ellipse tool.
 */
struct EnableEllipseToolEvent final
{};

/**
 * Event for enabling the point tool.
 */
struct EnablePointToolEvent final
{};

/**
 * Event for creating a new layer in the active map.
 */
struct CreateLayerEvent final
{
  /** The type of layer to create. */
  LayerType type;
};

/**
 * Event for removing a layer from the active map.
 */
struct RemoveLayerEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

/**
 * Event for duplicating a layer in the active map.
 */
struct DuplicateLayerEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

/**
 * Event for moving a layer up in the active map.
 */
struct MoveLayerUpEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

/**
 * Event for moving a layer down in the active map.
 */
struct MoveLayerDownEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

/**
 * Event for changing the name of a layer in the active map.
 */
struct RenameLayerEvent final
{
  /** The target layer. */
  EntityID layer_entity;

  /** The new layer name. */
  std::string name;
};

/**
 * Event for changing the opacity of a layer in the active map.
 */
struct SetLayerOpacityEvent final
{
  /** The target layer. */
  EntityID layer_entity;

  /** The new layer opacity. */
  float opacity;
};

/**
 * Event for changing the visibility of a layer in the active map.
 */
struct SetLayerVisibleEvent final
{
  /** The target layer. */
  EntityID layer_entity;

  /** The new visibility. */
  bool visible;
};

/**
 * Event for creating new map documents.
 */
struct CreateMapEvent final
{
  /** The requested map specification. */
  MapSpec spec;
};

/**
 * Event for marking the active map as the selected meta context.
 */
struct InspectMapEvent final
{};

/**
 * Event for opening the tileset import dialog.
 */
struct ShowAddTilesetDialogEvent final
{};

/**
 * Event for opening map resize dialog.
 */
struct ShowResizeMapDialogEvent final
{};

/**
 * Event for adding a tile row to the active map.
 */
struct AddRowToMapEvent final
{};

/**
 * Event for adding a tile column to the active map.
 */
struct AddColumnToMapEvent final
{};

/**
 * Event for removing a tile row from the active map.
 */
struct RemoveRowToMapEvent final
{};

/**
 * Event for removing a tile column from the active map.
 */
struct RemoveColumnToMapEvent final
{};

/**
 * Event for fixing all invalid tile identifiers in the active map.
 */
struct FixMapTilesEvent final
{};

/**
 * Event for opening the dialog for exporting the active map as a Godot scene.
 */
struct ShowGodotExportDialogEvent final
{};

/**
 * Event for saving the active map as a Godot scene.
 */
struct ExportAsGodotSceneEvent final
{
  /** The desired major Godot version. */
  int version;

  /** The base directory of the Godot project. */
  std::filesystem::path project_dir;
};

/**
 * Event for opening the tileset creation dialog.
 */
struct ShowNewTilesetDialogEvent final
{};

/**
 * Event for adding a tileset to a map.
 */
struct AddTilesetEvent final
{
  /** The path to the tileset texture. */
  std::filesystem::path texture_path;

  /** The size of tiles in the tileset texture. */
  Int2 tile_size;
};

/**
 * Event for moving a viewport in the current document.
 */
struct OffsetViewportEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;

  /** The offset to apply to the viewport position. */
  Float2 delta;
};

/**
 * Event for changing the size a viewport in the current document.
 */
struct UpdateViewportSizeEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;

  /** The new viewport size. */
  Float2 new_size;
};

/**
 * Event for changing the limits of a viewport in the current document.
 */
struct UpdateViewportLimitsEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;

  /** The new minimum position. */
  Float2 min_pos;

  /** The new maximum position. */
  Float2 max_pos;
};

/**
 * Event for increasing the zoom of a viewport in the current document.
 */
struct IncreaseViewportZoomEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for decreasing the zoom of a viewport in the current document.
 */
struct DecreaseViewportZoomEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for resetting the zoom of a viewport in the current document.
 */
struct ResetViewportZoomEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for centering a viewport in the current document.
 */
struct CenterViewportEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;

  /** The size of the underlying content. */
  Float2 content_size;
};

/**
 * Event for panning a viewport upwards in the current document.
 */
struct PanViewportUpEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for panning a viewport downwards in the current document.
 */
struct PanViewportDownEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for panning a viewport leftwards in the current document.
 */
struct PanViewportLeftEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

/**
 * Event for panning a viewport rightwards in the current document.
 */
struct PanViewportRightEvent final
{
  /** The target viewport. */
  EntityID viewport_entity;
};

}  // namespace tactile::core
