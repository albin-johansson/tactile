#pragma once

#include <centurion.hpp>  // window, keyboard
#include <entt.hpp>       // dispatcher

#include "aliases/ints.hpp"
#include "core/model.hpp"
#include "events/command_events.hpp"
#include "events/layer_events.hpp"
#include "events/map_events.hpp"
#include "events/property_events.hpp"
#include "events/quit_event.hpp"
#include "events/save_as_event.hpp"
#include "events/tileset_events.hpp"
#include "events/tool_events.hpp"
#include "events/viewport_events.hpp"
#include "gui/icons.hpp"

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
  void PollEvents();
  void UpdateFrame();

  void OnShowMapPropertiesEvent();
  void OnAddMapEvent(const AddMapEvent& event);
  void OnCloseMapEvent(const CloseMapEvent& event);
  void OnOpenMapEvent(const OpenMapEvent& event);
  void OnSelectMapEvent(const SelectMapEvent& event);

  void OnSaveEvent();
  void OnSaveAsEvent(const SaveAsEvent& event);
  void OnSaveAsRequestEvent();

  void OnUndoEvent();
  void OnRedoEvent();
  void OnChangeCommandCapacityEvent(const ChangeCommandCapacityEvent& event);

  void OnSelectToolEvent(const SelectToolEvent& event);
  void OnMousePressedEvent(const MousePressedEvent& event);
  void OnMouseReleasedEvent(const MouseReleasedEvent& event);
  void OnMouseDragEvent(const MouseDragEvent& event);
  void OnStampSequenceEvent(StampSequenceEvent event);
  void OnEraserSequenceEvent(EraserSequenceEvent event);
  void OnFloodEvent(const FloodEvent& event);

  void OnCenterViewportEvent();
  void OnOffsetViewportEvent(const OffsetViewportEvent& event);
  void OnPanLeftEvent();
  void OnPanRightEvent();
  void OnPanUpEvent();
  void OnPanDownEvent();
  void OnIncreaseViewportZoomEvent();
  void OnDecreaseViewportZoomEvent();
  void OnResetViewportZoomEvent();

  void OnAddTilesetEvent(const AddTilesetEvent& event);
  void OnRemoveTilesetEvent(const RemoveTilesetEvent& event);
  void OnSelectTilesetEvent(const SelectTilesetEvent& event);
  void OnSetTilesetSelectionEvent(const SetTilesetSelectionEvent& event);

  void OnAddRowEvent();
  void OnAddColumnEvent();
  void OnRemoveRowEvent();
  void OnRemoveColumnEvent();
  void OnResizeMapEvent(const ResizeMapEvent& event);
  void OnOpenResizeMapDialogEvent();

  void OnAddLayerEvent(const AddLayerEvent& event);
  void OnRemoveLayerEvent(const RemoveLayerEvent& event);
  void OnSelectLayerEvent(const SelectLayerEvent& event);
  void OnMoveLayerUpEvent(const MoveLayerUpEvent& event);
  void OnMoveLayerDownEvent(const MoveLayerDownEvent& event);
  void OnDuplicateLayerEvent(const DuplicateLayerEvent& event);
  void OnSetLayerOpacityEvent(const SetLayerOpacityEvent& event);
  void OnSetLayerVisibleEvent(const SetLayerVisibleEvent& event);
  void OnRenameLayerRequestEvent(const RenameLayerRequestEvent& event);
  void OnRenameLayerEvent(const RenameLayerEvent& event);
  void OnShowLayerPropertiesEvent(const ShowLayerPropertiesEvent& event);

  void OnAddPropertyEvent(const AddPropertyEvent& event);
  void OnRemovePropertyEvent(const RemovePropertyEvent& event);
  void OnRenamePropertyEvent(const RenamePropertyEvent& event);
  void OnUpdatePropertyEvent(const UpdatePropertyEvent& event);
  void OnChangePropertyTypeEvent(const ChangePropertyTypeEvent& event);

  void OnToggleUiEvent();
  void OnQuitEvent();
};

}  // namespace Tactile
