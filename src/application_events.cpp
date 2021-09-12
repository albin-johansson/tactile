#include "application_events.hpp"

#include "application.hpp"
#include "events/map_events.hpp"
#include "events/view_events.hpp"
#include "events/viewport_events.hpp"

namespace Tactile {

using App = Application;

void SubscribeToEvents(Application* app, entt::dispatcher& d)
{
  d.sink<UndoEvent>().connect<&App::OnUndoEvent>(app);
  d.sink<RedoEvent>().connect<&App::OnRedoEvent>(app);
  d.sink<SetCommandCapacityEvent>().connect<&App::OnSetCommandCapacityEvent>(app);

  d.sink<SaveEvent>().connect<&App::OnSaveEvent>(app);
  d.sink<SaveAsEvent>().connect<&App::OnSaveAsEvent>(app);
  d.sink<OpenSaveAsDialogEvent>().connect<&App::OnOpenSaveAsDialogEvent>(app);

  d.sink<ShowMapPropertiesEvent>().connect<&App::OnShowMapPropertiesEvent>(app);
  d.sink<AddMapEvent>().connect<&App::OnAddMapEvent>(app);
  d.sink<CloseMapEvent>().connect<&App::OnCloseMapEvent>(app);
  d.sink<OpenMapEvent>().connect<&App::OnOpenMapEvent>(app);
  d.sink<SelectMapEvent>().connect<&App::OnSelectMapEvent>(app);

  d.sink<SelectToolEvent>().connect<&App::OnSelectToolEvent>(app);
  d.sink<MousePressedEvent>().connect<&App::OnMousePressedEvent>(app);
  d.sink<MouseDragEvent>().connect<&App::OnMouseDragEvent>(app);
  d.sink<MouseReleasedEvent>().connect<&App::OnMouseReleasedEvent>(app);
  d.sink<StampSequenceEvent>().connect<&App::OnStampSequenceEvent>(app);
  d.sink<EraserSequenceEvent>().connect<&App::OnEraserSequenceEvent>(app);
  d.sink<FloodEvent>().connect<&App::OnFloodEvent>(app);

  d.sink<CenterViewportEvent>().connect<&App::OnCenterViewportEvent>(app);
  d.sink<OffsetViewportEvent>().connect<&App::OnOffsetViewportEvent>(app);
  d.sink<PanLeftEvent>().connect<&App::OnPanLeftEvent>(app);
  d.sink<PanRightEvent>().connect<&App::OnPanRightEvent>(app);
  d.sink<PanUpEvent>().connect<&App::OnPanUpEvent>(app);
  d.sink<PanDownEvent>().connect<&App::OnPanDownEvent>(app);
  d.sink<IncreaseZoomEvent>().connect<&App::OnIncreaseZoomEvent>(app);
  d.sink<DecreaseZoomEvent>().connect<&App::OnDecreaseZoomEvent>(app);
  d.sink<ResetZoomEvent>().connect<&App::OnResetZoomEvent>(app);

  d.sink<AddTilesetEvent>().connect<&App::OnAddTilesetEvent>(app);
  d.sink<RemoveTilesetEvent>().connect<&App::OnRemoveTilesetEvent>(app);
  d.sink<SelectTilesetEvent>().connect<&App::OnSelectTilesetEvent>(app);
  d.sink<SetTilesetSelectionEvent>().connect<&App::OnSetTilesetSelectionEvent>(app);

  d.sink<AddRowEvent>().connect<&App::OnAddRowEvent>(app);
  d.sink<AddColumnEvent>().connect<&App::OnAddColumnEvent>(app);
  d.sink<RemoveRowEvent>().connect<&App::OnRemoveRowEvent>(app);
  d.sink<RemoveColumnEvent>().connect<&App::OnRemoveColumnEvent>(app);
  d.sink<ResizeMapEvent>().connect<&App::OnResizeMapEvent>(app);
  d.sink<OpenResizeMapDialogEvent>().connect<&App::OnOpenResizeMapDialogEvent>(app);

  d.sink<AddLayerEvent>().connect<&App::OnAddLayerEvent>(app);
  d.sink<RemoveLayerEvent>().connect<&App::OnRemoveLayerEvent>(app);
  d.sink<SelectLayerEvent>().connect<&App::OnSelectLayerEvent>(app);
  d.sink<MoveLayerUpEvent>().connect<&App::OnMoveLayerUpEvent>(app);
  d.sink<MoveLayerDownEvent>().connect<&App::OnMoveLayerDownEvent>(app);
  d.sink<DuplicateLayerEvent>().connect<&App::OnDuplicateLayerEvent>(app);
  d.sink<SetLayerOpacityEvent>().connect<&App::OnSetLayerOpacityEvent>(app);
  d.sink<SetLayerVisibleEvent>().connect<&App::OnSetLayerVisibleEvent>(app);
  d.sink<OpenRenameLayerDialogEvent>().connect<&App::OnOpenRenameLayerDialogEvent>(app);
  d.sink<RenameLayerEvent>().connect<&App::OnRenameLayerEvent>(app);
  d.sink<ShowLayerPropertiesEvent>().connect<&App::OnShowLayerPropertiesEvent>(app);

  d.sink<AddPropertyEvent>().connect<&App::OnAddPropertyEvent>(app);
  d.sink<RemovePropertyEvent>().connect<&App::OnRemovePropertyEvent>(app);
  d.sink<RenamePropertyEvent>().connect<&App::OnRenamePropertyEvent>(app);
  d.sink<UpdatePropertyEvent>().connect<&App::OnUpdatePropertyEvent>(app);
  d.sink<ChangePropertyTypeEvent>().connect<&App::OnChangePropertyTypeEvent>(app);

  d.sink<ToggleUiEvent>().connect<&App::OnToggleUiEvent>(app);
  d.sink<QuitEvent>().connect<&App::OnQuitEvent>(app);
}

}  // namespace Tactile
