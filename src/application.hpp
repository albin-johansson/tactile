#pragma once

#include <centurion.hpp>  // window, keyboard
#include <entt.hpp>       // dispatcher

#include "editor/events/command_events.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/events/quit_event.hpp"
#include "editor/events/save_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/model.hpp"
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

class Application final
{
  friend void SubscribeToEvents(Application*, entt::dispatcher&);

 public:
  Application(cen::window&& window);

  auto Run() -> int;

 private:
  cen::window mWindow;
  cen::keyboard mKeyboard;
  entt::dispatcher mDispatcher;
  Model mModel;
  [[no_unique_address]] Icons mIcons;
  bool mQuit{};

  void OnAboutToExit();
  void SaveCurrentFilesToHistory();
  void PollEvents();
  void UpdateFrame();

  void OnUndo();
  void OnRedo();
  void OnSetCommandCapacity(const SetCommandCapacityEvent& event);

  void OnSave();
  void OnSaveAs(const SaveAsEvent& event);
  void OnOpenSaveAsDialog();

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
  void OnPanLeft();
  void OnPanRight();
  void OnPanUp();
  void OnPanDown();
  void OnIncreaseZoom();
  void OnDecreaseZoom();
  void OnResetZoom();

  void OnAddTileset(const AddTilesetEvent& event);
  void OnRemoveTileset(const RemoveTilesetEvent& event);
  void OnSelectTileset(const SelectTilesetEvent& event);
  void OnSetTilesetSelection(const SetTilesetSelectionEvent& event);
  void OnShowTilesetProperties(const ShowTilesetPropertiesEvent& event);

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

  void OnAddProperty(const AddPropertyEvent& event);
  void OnRemoveProperty(const RemovePropertyEvent& event);
  void OnRenameProperty(const RenamePropertyEvent& event);
  void OnUpdateProperty(const UpdatePropertyEvent& event);
  void OnChangePropertyType(const ChangePropertyTypeEvent& event);

  void OnToggleUi();
  void OnQuit();
};

}  // namespace Tactile
