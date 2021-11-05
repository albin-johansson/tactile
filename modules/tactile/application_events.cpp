#include "application_events.hpp"

#include "application.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/view_events.hpp"
#include "editor/events/viewport_events.hpp"

namespace Tactile {

using App = Application;

void SubscribeToEvents(Application* app, entt::dispatcher& d)
{
  d.sink<UndoEvent>().connect<&App::OnUndo>(app);
  d.sink<RedoEvent>().connect<&App::OnRedo>(app);
  d.sink<SetCommandCapacityEvent>().connect<&App::OnSetCommandCapacity>(app);

  d.sink<SaveEvent>().connect<&App::OnSave>(app);
  d.sink<SaveAsEvent>().connect<&App::OnSaveAs>(app);
  d.sink<OpenSaveAsDialogEvent>().connect<&App::OnOpenSaveAsDialog>(app);

  d.sink<ShowMapPropertiesEvent>().connect<&App::OnShowMapProperties>(app);
  d.sink<AddMapEvent>().connect<&App::OnAddMap>(app);
  d.sink<CloseMapEvent>().connect<&App::OnCloseMap>(app);
  d.sink<OpenMapEvent>().connect<&App::OnOpenMap>(app);
  d.sink<SelectMapEvent>().connect<&App::OnSelectMap>(app);

  d.sink<SelectToolEvent>().connect<&App::OnSelectTool>(app);
  d.sink<MousePressedEvent>().connect<&App::OnMousePressed>(app);
  d.sink<MouseDragEvent>().connect<&App::OnMouseDrag>(app);
  d.sink<MouseReleasedEvent>().connect<&App::OnMouseReleased>(app);
  d.sink<StampSequenceEvent>().connect<&App::OnStampSequence>(app);
  d.sink<EraserSequenceEvent>().connect<&App::OnEraserSequence>(app);
  d.sink<FloodEvent>().connect<&App::OnFlood>(app);

  d.sink<CenterViewportEvent>().connect<&App::OnCenterViewport>(app);
  d.sink<OffsetViewportEvent>().connect<&App::OnOffsetViewport>(app);
  d.sink<PanLeftEvent>().connect<&App::OnPanLeft>(app);
  d.sink<PanRightEvent>().connect<&App::OnPanRight>(app);
  d.sink<PanUpEvent>().connect<&App::OnPanUp>(app);
  d.sink<PanDownEvent>().connect<&App::OnPanDown>(app);
  d.sink<IncreaseZoomEvent>().connect<&App::OnIncreaseZoom>(app);
  d.sink<DecreaseZoomEvent>().connect<&App::OnDecreaseZoom>(app);
  d.sink<ResetZoomEvent>().connect<&App::OnResetZoom>(app);

  d.sink<AddTilesetEvent>().connect<&App::OnAddTileset>(app);
  d.sink<RemoveTilesetEvent>().connect<&App::OnRemoveTileset>(app);
  d.sink<SelectTilesetEvent>().connect<&App::OnSelectTileset>(app);
  d.sink<SetTilesetSelectionEvent>().connect<&App::OnSetTilesetSelection>(app);
  d.sink<ShowTilesetPropertiesEvent>().connect<&App::OnShowTilesetProperties>(app);

  d.sink<AddRowEvent>().connect<&App::OnAddRow>(app);
  d.sink<AddColumnEvent>().connect<&App::OnAddColumn>(app);
  d.sink<RemoveRowEvent>().connect<&App::OnRemoveRow>(app);
  d.sink<RemoveColumnEvent>().connect<&App::OnRemoveColumn>(app);
  d.sink<ResizeMapEvent>().connect<&App::OnResizeMap>(app);
  d.sink<OpenResizeMapDialogEvent>().connect<&App::OnOpenResizeMapDialog>(app);

  d.sink<AddLayerEvent>().connect<&App::OnAddLayer>(app);
  d.sink<RemoveLayerEvent>().connect<&App::OnRemoveLayer>(app);
  d.sink<SelectLayerEvent>().connect<&App::OnSelectLayer>(app);
  d.sink<MoveLayerUpEvent>().connect<&App::OnMoveLayerUp>(app);
  d.sink<MoveLayerDownEvent>().connect<&App::OnMoveLayerDown>(app);
  d.sink<DuplicateLayerEvent>().connect<&App::OnDuplicateLayer>(app);
  d.sink<SetLayerOpacityEvent>().connect<&App::OnSetLayerOpacity>(app);
  d.sink<SetLayerVisibleEvent>().connect<&App::OnSetLayerVisible>(app);
  d.sink<OpenRenameLayerDialogEvent>().connect<&App::OnOpenRenameLayerDialog>(app);
  d.sink<RenameLayerEvent>().connect<&App::OnRenameLayer>(app);
  d.sink<ShowLayerPropertiesEvent>().connect<&App::OnShowLayerProperties>(app);

  d.sink<MoveObjectEvent>().connect<&App::OnMoveObject>(app);
  d.sink<SetObjectVisibilityEvent>().connect<&App::OnSetObjectVisibility>(app);
  d.sink<SetObjectTagEvent>().connect<&App::OnSetObjectTag>(app);

  d.sink<AddPropertyEvent>().connect<&App::OnAddProperty>(app);
  d.sink<RemovePropertyEvent>().connect<&App::OnRemoveProperty>(app);
  d.sink<RenamePropertyEvent>().connect<&App::OnRenameProperty>(app);
  d.sink<UpdatePropertyEvent>().connect<&App::OnUpdateProperty>(app);
  d.sink<ChangePropertyTypeEvent>().connect<&App::OnChangePropertyType>(app);
  d.sink<SetPropertyContextEvent>().connect<&App::OnSetPropertyContext>(app);

  d.sink<ToggleUiEvent>().connect<&App::OnToggleUi>(app);
  d.sink<QuitEvent>().connect<&App::OnQuit>(app);
}

}  // namespace Tactile
