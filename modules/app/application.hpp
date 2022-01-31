#pragma once

#include <centurion.hpp>
#include <entt/entt.hpp>

#include "core/utils/texture_manager.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/component_events.hpp"
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
#include "tactile_def.hpp"

namespace tactile {

class app_configuration;

/// Tracks visibility of widgets for the "Toggle UI" feature.
struct WidgetShowState final
{
  bool prev_show_layer_dock{};
  bool prev_show_tileset_dock{};
  bool prev_show_properties_dock{};
  bool prev_show_log_dock{};
  bool prev_show_toolbar{};
  bool prev_show_component_dock{};
};

class Application final {
  friend void SubscribeToEvents(Application&);

 public:
  explicit Application(app_configuration* configuration);

  auto Run() -> int;

 private:
  app_configuration* mConfiguration{}; /* Non-owning */

  entt::dispatcher mDispatcher;
  cen::keyboard mKeyboard;

  Model mModel;
  texture_manager mTextures;
  Icons mIcons;

  WidgetManager mWidgets;
  WidgetShowState mWidgetShowState;

  bool mQuit{};

  template <typename Event, auto Slot>
  void Connect()
  {
    mDispatcher.sink<Event>().template connect<Slot>(this);
  }

  void OnAboutToExit();
  void SaveCurrentFilesToHistory();

  void PollEvents();
  void OnKeyboardEvent(cen::keyboard_event event);
  void OnMouseWheelEvent(const cen::mouse_wheel_event& event);

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
  void OnCreateMap(const CreateMapEvent& event);
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

  void OnSetObjectName(const SetObjectNameEvent& event);
  void OnMoveObject(const MoveObjectEvent& event);
  void OnSetObjectVisibility(const SetObjectVisibilityEvent& event);
  void OnSetObjectTag(const SetObjectTagEvent& event);
  void OnSpawnObjectContextMenu(const SpawnObjectContextMenuEvent& event);

  void OnShowAddPropertyDialog();
  void OnShowRenamePropertyDialog(const ShowRenamePropertyDialogEvent& event);
  void OnShowChangePropertyTypeDialog(const ShowChangePropertyTypeDialogEvent& event);
  void OnAddProperty(const AddPropertyEvent& event);
  void OnRemoveProperty(const RemovePropertyEvent& event);
  void OnRenameProperty(const RenamePropertyEvent& event);
  void OnUpdateProperty(const UpdatePropertyEvent& event);
  void OnChangePropertyType(const ChangePropertyTypeEvent& event);
  void OnInspectContext(const InspectContextEvent& event);

  void OnOpenComponentEditor();
  void OnCreateComponentDef(const CreateComponentDefEvent& event);
  void OnRemoveComponentDef(const RemoveComponentDefEvent& event);
  void OnRenameComponentDef(const RenameComponentDefEvent& event);
  void OnCreateComponentAttribute(const CreateComponentAttributeEvent& event);
  void OnRemoveComponentAttribute(const RemoveComponentAttributeEvent& event);
  void OnRenameComponentAttribute(const RenameComponentAttributeEvent& event);
  void OnDuplicateComponentAttribute(const DuplicateComponentAttributeEvent& event);
  void OnSetComponentAttributeType(const SetComponentAttributeTypeEvent& event);
  void OnUpdateComponentDefAttribute(const UpdateComponentDefAttributeEvent& event);

  void OnAddComponent(const AddComponentEvent& event);
  void OnRemoveComponent(const RemoveComponentEvent& event);
  void OnUpdateComponent(const UpdateComponentEvent& event);
  void OnResetComponentValues(const ResetComponentValuesEvent& event);

  void OnToggleUi();
  void OnQuit();
};

}  // namespace tactile