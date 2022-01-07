#include "application_events.hpp"

#include "application.hpp"
#include "editor/events/edit_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/view_events.hpp"
#include "editor/events/viewport_events.hpp"

namespace Tactile {

using App = Application;

void SubscribeToEvents(App& app)
{
  app.Connect<UndoEvent, &App::OnUndo>();
  app.Connect<RedoEvent, &App::OnRedo>();
  app.Connect<SetCommandCapacityEvent, &App::OnSetCommandCapacity>();

  app.Connect<SaveEvent, &App::OnSave>();
  app.Connect<SaveAsEvent, &App::OnSaveAs>();
  app.Connect<OpenSaveAsDialogEvent, &App::OnOpenSaveAsDialog>();
  app.Connect<ShowSettingsEvent, &App::OnShowSettings>();

  app.Connect<ShowNewMapDialogEvent, &App::OnShowNewMapDialog>();
  app.Connect<ShowOpenMapDialogEvent, &App::OnShowOpenMapDialog>();
  app.Connect<ShowMapPropertiesEvent, &App::OnShowMapProperties>();
  app.Connect<CreateMapEvent, &App::OnCreateMap>();
  app.Connect<CloseMapEvent, &App::OnCloseMap>();
  app.Connect<OpenMapEvent, &App::OnOpenMap>();
  app.Connect<SelectMapEvent, &App::OnSelectMap>();

  app.Connect<SelectToolEvent, &App::OnSelectTool>();
  app.Connect<MousePressedEvent, &App::OnMousePressed>();
  app.Connect<MouseDragEvent, &App::OnMouseDrag>();
  app.Connect<MouseReleasedEvent, &App::OnMouseReleased>();
  app.Connect<StampSequenceEvent, &App::OnStampSequence>();
  app.Connect<EraserSequenceEvent, &App::OnEraserSequence>();
  app.Connect<FloodEvent, &App::OnFlood>();

  app.Connect<CenterViewportEvent, &App::OnCenterViewport>();
  app.Connect<OffsetViewportEvent, &App::OnOffsetViewport>();
  app.Connect<OffsetBoundViewportEvent, &App::OnOffsetBoundViewport>();
  app.Connect<PanLeftEvent, &App::OnPanLeft>();
  app.Connect<PanRightEvent, &App::OnPanRight>();
  app.Connect<PanUpEvent, &App::OnPanUp>();
  app.Connect<PanDownEvent, &App::OnPanDown>();
  app.Connect<IncreaseZoomEvent, &App::OnIncreaseZoom>();
  app.Connect<DecreaseZoomEvent, &App::OnDecreaseZoom>();
  app.Connect<ResetZoomEvent, &App::OnResetZoom>();

  app.Connect<ShowAddTilesetDialogEvent, &App::OnShowAddTilesetDialog>();
  app.Connect<AddTilesetEvent, &App::OnAddTileset>();
  app.Connect<RemoveTilesetEvent, &App::OnRemoveTileset>();
  app.Connect<SelectTilesetEvent, &App::OnSelectTileset>();
  app.Connect<SetTilesetSelectionEvent, &App::OnSetTilesetSelection>();
  app.Connect<ShowTilesetPropertiesEvent, &App::OnShowTilesetProperties>();
  app.Connect<SetTilesetNameEvent, &App::OnSetTilesetName>();

  app.Connect<AddRowEvent, &App::OnAddRow>();
  app.Connect<AddColumnEvent, &App::OnAddColumn>();
  app.Connect<RemoveRowEvent, &App::OnRemoveRow>();
  app.Connect<RemoveColumnEvent, &App::OnRemoveColumn>();
  app.Connect<ResizeMapEvent, &App::OnResizeMap>();
  app.Connect<OpenResizeMapDialogEvent, &App::OnOpenResizeMapDialog>();

  app.Connect<AddLayerEvent, &App::OnAddLayer>();
  app.Connect<RemoveLayerEvent, &App::OnRemoveLayer>();
  app.Connect<SelectLayerEvent, &App::OnSelectLayer>();
  app.Connect<MoveLayerUpEvent, &App::OnMoveLayerUp>();
  app.Connect<MoveLayerDownEvent, &App::OnMoveLayerDown>();
  app.Connect<DuplicateLayerEvent, &App::OnDuplicateLayer>();
  app.Connect<SetLayerOpacityEvent, &App::OnSetLayerOpacity>();
  app.Connect<SetLayerVisibleEvent, &App::OnSetLayerVisible>();
  app.Connect<OpenRenameLayerDialogEvent, &App::OnOpenRenameLayerDialog>();
  app.Connect<RenameLayerEvent, &App::OnRenameLayer>();
  app.Connect<ShowLayerPropertiesEvent, &App::OnShowLayerProperties>();

  app.Connect<SetObjectNameEvent, &App::OnSetObjectName>();
  app.Connect<MoveObjectEvent, &App::OnMoveObject>();
  app.Connect<SetObjectVisibilityEvent, &App::OnSetObjectVisibility>();
  app.Connect<SetObjectTagEvent, &App::OnSetObjectTag>();
  app.Connect<SpawnObjectContextMenuEvent, &App::OnSpawnObjectContextMenu>();

  app.Connect<ShowAddPropertyDialogEvent, &App::OnShowAddPropertyDialog>();
  app.Connect<ShowRenamePropertyDialogEvent, &App::OnShowRenamePropertyDialog>();
  app.Connect<ShowChangePropertyTypeDialogEvent, &App::OnShowChangePropertyTypeDialog>();
  app.Connect<AddPropertyEvent, &App::OnAddProperty>();
  app.Connect<RemovePropertyEvent, &App::OnRemoveProperty>();
  app.Connect<RenamePropertyEvent, &App::OnRenameProperty>();
  app.Connect<UpdatePropertyEvent, &App::OnUpdateProperty>();
  app.Connect<ChangePropertyTypeEvent, &App::OnChangePropertyType>();
  app.Connect<SetPropertyContextEvent, &App::OnSetPropertyContext>();

  app.Connect<OpenComponentEditorEvent, &App::OnOpenComponentEditor>();
  app.Connect<CreateComponentDefEvent, &App::OnCreateComponentDef>();
  app.Connect<RemoveComponentDefEvent, &App::OnRemoveComponentDef>();
  app.Connect<RenameComponentDefEvent, &App::OnRenameComponentDef>();
  app.Connect<CreateComponentAttributeEvent, &App::OnCreateComponentAttribute>();
  app.Connect<RemoveComponentAttributeEvent, &App::OnRemoveComponentAttribute>();
  app.Connect<RenameComponentAttributeEvent, &App::OnRenameComponentAttribute>();
  app.Connect<DuplicateComponentAttributeEvent, &App::OnDuplicateComponentAttribute>();
  app.Connect<SetComponentAttributeTypeEvent, &App::OnSetComponentAttributeType>();
  app.Connect<UpdateComponentDefAttributeEvent, &App::OnUpdateComponentDefAttribute>();

  app.Connect<AddComponentEvent, &App::OnAddComponent>();
  app.Connect<UpdateComponentEvent, &App::OnUpdateComponent>();
  app.Connect<ResetComponentValuesEvent, &App::OnResetComponentValues>();

  app.Connect<ToggleUiEvent, &App::OnToggleUi>();
  app.Connect<QuitEvent, &App::OnQuit>();
}

}  // namespace Tactile
