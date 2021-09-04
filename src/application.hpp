#pragma once

#include <centurion.hpp>  // window, keyboard
#include <entt.hpp>       // dispatcher

#include "aliases/ints.hpp"
#include "core/model.hpp"
#include "events/change_command_capacity_event.hpp"
#include "events/flood_event.hpp"
#include "events/layers/add_layer_event.hpp"
#include "events/layers/duplicate_layer_event.hpp"
#include "events/layers/move_layer_down_event.hpp"
#include "events/layers/move_layer_up_event.hpp"
#include "events/layers/remove_layer_event.hpp"
#include "events/layers/select_layer_event.hpp"
#include "events/layers/set_layer_opacity_event.hpp"
#include "events/layers/set_layer_visible_event.hpp"
#include "events/layers/show_layer_properties_event.hpp"
#include "events/maps/add_column_event.hpp"
#include "events/maps/add_map_event.hpp"
#include "events/maps/add_row_event.hpp"
#include "events/maps/close_map_event.hpp"
#include "events/maps/open_map_event.hpp"
#include "events/maps/remove_column_event.hpp"
#include "events/maps/remove_row_event.hpp"
#include "events/maps/select_map_event.hpp"
#include "events/properties/add_property_event.hpp"
#include "events/properties/change_property_type_event.hpp"
#include "events/properties/remove_property_event.hpp"
#include "events/properties/rename_property_event.hpp"
#include "events/properties/set_property_value_event.hpp"
#include "events/quit_event.hpp"
#include "events/redo_event.hpp"
#include "events/save_as_event.hpp"
#include "events/stamp_sequence_event.hpp"
#include "events/tilesets/add_tileset_event.hpp"
#include "events/tilesets/remove_tileset_event.hpp"
#include "events/tilesets/select_tileset_event.hpp"
#include "events/tilesets/set_tileset_selection_event.hpp"
#include "events/tool_events.hpp"
#include "events/undo_event.hpp"
#include "events/viewport/offset_viewport_event.hpp"
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

  void OnAddMapEvent(const AddMapEvent& event);

  void OnCloseMapEvent(const CloseMapEvent& event);

  void OnOpenMapEvent(const OpenMapEvent& event);

  void OnSaveEvent();

  void OnSaveAsEvent(const SaveAsEvent& event);

  void OnSaveAsRequestEvent();

  void OnAddTilesetEvent(const AddTilesetEvent& event);

  void OnUndoEvent();

  void OnRedoEvent();

  void OnSelectToolEvent(const SelectToolEvent& event);

  void OnMousePressedEvent(const MousePressedEvent& event);

  void OnMouseReleasedEvent(const MouseReleasedEvent& event);

  void OnMouseDragEvent(const MouseDragEvent& event);

  void OnStampSequenceEvent(const StampSequenceEvent& event);

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

  void OnSelectMapEvent(const SelectMapEvent& event);

  void OnSelectLayerEvent(const SelectLayerEvent& event);

  void OnSelectTilesetEvent(const SelectTilesetEvent& event);

  void OnRemoveTilesetEvent(const RemoveTilesetEvent& event);

  void OnAddRowEvent();

  void OnAddColumnEvent();

  void OnRemoveRowEvent();

  void OnRemoveColumnEvent();

  void OnAddLayerEvent(const AddLayerEvent& event);

  void OnRemoveLayerEvent(const RemoveLayerEvent& event);

  void OnMoveLayerUpEvent(const MoveLayerUpEvent& event);

  void OnMoveLayerDownEvent(const MoveLayerDownEvent& event);

  void OnDuplicateLayerEvent(const DuplicateLayerEvent& event);

  void OnSetLayerOpacityEvent(const SetLayerOpacityEvent& event);

  void OnSetLayerVisibleEvent(const SetLayerVisibleEvent& event);

  void OnShowLayerPropertiesEvent(const ShowLayerPropertiesEvent& event);

  void OnShowMapPropertiesEvent();

  void OnAddPropertyEvent(const AddPropertyEvent& event);

  void OnRemovePropertyEvent(const RemovePropertyEvent& event);

  void OnRenamePropertyEvent(const RenamePropertyEvent& event);

  void OnSetPropertyValueEvent(const SetPropertyValueEvent& event);

  void OnChangePropertyTypeEvent(const ChangePropertyTypeEvent& event);

  void OnSetTilesetSelectionEvent(const SetTilesetSelectionEvent& event);

  void OnChangeCommandCapacityEvent(const ChangeCommandCapacityEvent& event);

  void OnQuitEvent();
};

}  // namespace Tactile
