#pragma once

#include <tactile_def.hpp>

#include <centurion.hpp>
#include <entt/entt.hpp>

#include "core/utils/texture_manager.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/object_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/events/quit_event.hpp"
#include "editor/events/save_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"

namespace Tactile {

class ApplicationConfiguration;

class Application final {
  friend void SubscribeToEvents(Application*, entt::dispatcher&);

 public:
  explicit Application(ApplicationConfiguration* configuration);

  auto Run() -> int;

 private:
  ApplicationConfiguration* mConfiguration{}; /* Non-owning */
  cen::keyboard mKeyboard;
  entt::dispatcher mDispatcher;
  Model mModel;
  TextureManager mTextures;
  Icons mIcons;
  WidgetManager mWidgets;
  bool mQuit{};

  void OnAboutToExit();
  void SaveCurrentFilesToHistory();

  void PollEvents();
  void OnKeyboardEvent(SDL_KeyboardEvent event);
  void OnMouseWheelEvent(const SDL_MouseWheelEvent& event);

  void UpdateFrame();

  void OnUndo();
  void OnRedo();
  void OnSetCommandCapacity(const SetCommandCapacityEvent& event);

  void OnSave();
  void OnSaveAs(const SaveAsEvent& event);
  void OnOpenSaveAsDialog();
  void OnShowSettings();

  void OnShowNewMapDialog();
  void OnShowOpenMapDialog();
  void OnShowMapProperties();
  void OnAddMap(const AddMapEvent& event);
  void OnCloseMap(const CloseMapEvent& event);
  void OnOpenMap(const OpenMapEvent& event);
  void OnSelectMap(const SelectMapEvent& event);

  void OnSelectTool(const SelectToolEvent& event);
  void OnMousePressed(const MousePressedEvent& event);
  void OnMouseDrag(const MouseDragEvent& event);
  void OnMouseReleased(const MouseReleasedEvent& event);
  void OnStampSequence(StampSequenceEvent event);
  void OnEraserSequence(EraserSequenceEvent event);
  void OnFlood(const FloodEvent& event);

  void OnCenterViewport();
  void OnOffsetViewport(const OffsetViewportEvent& event);
  void OnOffsetBoundViewport(const OffsetBoundViewportEvent& event);
  void OnPanLeft();
  void OnPanRight();
  void OnPanUp();
  void OnPanDown();
  void OnIncreaseZoom();
  void OnDecreaseZoom();
  void OnResetZoom();

  void OnShowAddTilesetDialog();
  void OnAddTileset(const AddTilesetEvent& event);
  void OnRemoveTileset(const RemoveTilesetEvent& event);
  void OnSelectTileset(const SelectTilesetEvent& event);
  void OnSetTilesetSelection(const SetTilesetSelectionEvent& event);
  void OnShowTilesetProperties(const ShowTilesetPropertiesEvent& event);
  void OnSetTilesetName(const SetTilesetNameEvent& event);

  void OnAddRow();
  void OnAddColumn();
  void OnRemoveRow();
  void OnRemoveColumn();
  void OnResizeMap(const ResizeMapEvent& event);
  void OnOpenResizeMapDialog();

  void OnAddLayer(const AddLayerEvent& event);
  void OnRemoveLayer(const RemoveLayerEvent& event);
  void OnSelectLayer(const SelectLayerEvent& event);
  void OnMoveLayerUp(const MoveLayerUpEvent& event);
  void OnMoveLayerDown(const MoveLayerDownEvent& event);
  void OnDuplicateLayer(const DuplicateLayerEvent& event);
  void OnSetLayerOpacity(const SetLayerOpacityEvent& event);
  void OnSetLayerVisible(const SetLayerVisibleEvent& event);
  void OnOpenRenameLayerDialog(const OpenRenameLayerDialogEvent& event);
  void OnRenameLayer(const RenameLayerEvent& event);
  void OnShowLayerProperties(const ShowLayerPropertiesEvent& event);

  void OnSetObjectName(const SetObjectNameEvent& event);
  void OnMoveObject(const MoveObjectEvent& event);
  void OnSetObjectVisibility(const SetObjectVisibilityEvent& event);
  void OnSetObjectTag(const SetObjectTagEvent& event);
  void OnSpawnObjectContextMenu(const SpawnObjectContextMenuEvent& event);

  void OnShowRenamePropertyDialog(const ShowRenamePropertyDialogEvent& event);
  void OnAddProperty(const AddPropertyEvent& event);
  void OnRemoveProperty(const RemovePropertyEvent& event);
  void OnRenameProperty(const RenamePropertyEvent& event);
  void OnUpdateProperty(const UpdatePropertyEvent& event);
  void OnChangePropertyType(const ChangePropertyTypeEvent& event);
  void OnSetPropertyContext(const SetPropertyContextEvent& event);

  void OnToggleUi();
  void OnQuit();
};

}  // namespace Tactile
