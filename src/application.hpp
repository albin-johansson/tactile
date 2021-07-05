#pragma once

#include <centurion.hpp>  // window
#include <entt.hpp>       // dispatcher
#include <vector>         // vector

#include "aliases/ints.hpp"
#include "aliases/unique.hpp"
#include "core/events/center_viewport_event.hpp"
#include "core/events/layers/add_layer_event.hpp"
#include "core/events/layers/duplicate_layer_event.hpp"
#include "core/events/layers/move_layer_down_event.hpp"
#include "core/events/layers/move_layer_up_event.hpp"
#include "core/events/layers/remove_layer_event.hpp"
#include "core/events/layers/select_layer_event.hpp"
#include "core/events/layers/set_layer_opacity_event.hpp"
#include "core/events/layers/set_layer_visible_event.hpp"
#include "core/events/layers/show_layer_properties_event.hpp"
#include "core/events/maps/add_column_event.hpp"
#include "core/events/maps/add_map_event.hpp"
#include "core/events/maps/add_row_event.hpp"
#include "core/events/maps/close_map_event.hpp"
#include "core/events/maps/open_map_event.hpp"
#include "core/events/maps/remove_column_event.hpp"
#include "core/events/maps/remove_row_event.hpp"
#include "core/events/maps/select_map_event.hpp"
#include "core/events/properties/add_property_event.hpp"
#include "core/events/properties/change_property_type_event.hpp"
#include "core/events/properties/remove_property_event.hpp"
#include "core/events/properties/rename_property_event.hpp"
#include "core/events/properties/set_property_value_event.hpp"
#include "core/events/quit_event.hpp"
#include "core/events/redo_event.hpp"
#include "core/events/tilesets/add_tileset_event.hpp"
#include "core/events/tilesets/remove_tileset_event.hpp"
#include "core/events/tilesets/select_tileset_event.hpp"
#include "core/events/tilesets/set_tileset_selection_event.hpp"
#include "core/events/tools/mouse_drag_event.hpp"
#include "core/events/tools/mouse_pressed_event.hpp"
#include "core/events/tools/mouse_released_event.hpp"
#include "core/events/tools/select_tool_event.hpp"
#include "core/events/undo_event.hpp"
#include "core/model.hpp"

namespace Tactile {

class Application final
{
  friend void SubscribeToEvents(Application*, entt::dispatcher&);

 public:
  Application(cen::window&& window);

  auto Run() -> int;

 private:
  cen::window mWindow;
  entt::dispatcher mDispatcher;
  Unique<Model> mModel;
  std::vector<uint> mTextures;
  bool mQuit{};

  void OnAboutToExit();

  void PollEvents();

  void UpdateFrame();

  void OnCtrlAltKeyStroke(cen::scan_code key);

  void OnAltShiftKeyStroke(cen::scan_code key);

  void OnCtrlKeyStroke(cen::scan_code key);

  void OnAltKeyStroke(cen::scan_code key);

  void OnKeyStroke(cen::scan_code key);

  void OnAddMapEvent();

  void OnCloseMapEvent(const CloseMapEvent& event);

  void OnOpenMapEvent(const OpenMapEvent& event);

  void OnAddTilesetEvent(const AddTilesetEvent& event);

  void OnUndoEvent();

  void OnRedoEvent();

  void OnSelectToolEvent(const SelectToolEvent& event);

  void OnMousePressedEvent(const MousePressedEvent& event);

  void OnMouseReleasedEvent(const MouseReleasedEvent& event);

  void OnMouseDragEvent(const MouseDragEvent& event);

  void OnCenterViewportEvent();

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

  void OnQuitEvent();
};

}  // namespace Tactile
