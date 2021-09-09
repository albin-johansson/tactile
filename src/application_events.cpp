#include "application_events.hpp"

#include "application.hpp"
#include "events/map_events.hpp"
#include "events/save_as_request_event.hpp"
#include "events/save_event.hpp"
#include "events/viewport_events.hpp"

namespace Tactile {

using App = Application;

void SubscribeToEvents(Application* app, entt::dispatcher& dispatcher)
{
  // clang-format off
  dispatcher.sink<UndoEvent>().connect<&App::OnUndoEvent>(app);
  dispatcher.sink<RedoEvent>().connect<&App::OnRedoEvent>(app);

  dispatcher.sink<AddMapEvent>().connect<&App::OnAddMapEvent>(app);
  dispatcher.sink<CloseMapEvent>().connect<&App::OnCloseMapEvent>(app);
  dispatcher.sink<OpenMapEvent>().connect<&App::OnOpenMapEvent>(app);

  dispatcher.sink<SaveEvent>().connect<&App::OnSaveEvent>(app);
  dispatcher.sink<SaveAsEvent>().connect<&App::OnSaveAsEvent>(app);
  dispatcher.sink<SaveAsRequestEvent>().connect<&App::OnSaveAsRequestEvent>(app);

  dispatcher.sink<AddTilesetEvent>().connect<&App::OnAddTilesetEvent>(app);
  dispatcher.sink<SelectTilesetEvent>().connect<&App::OnSelectTilesetEvent>(app);
  dispatcher.sink<RemoveTilesetEvent>().connect<&App::OnRemoveTilesetEvent>(app);

  dispatcher.sink<SelectMapEvent>().connect<&App::OnSelectMapEvent>(app);
  dispatcher.sink<CenterViewportEvent>().connect<&App::OnCenterViewportEvent>(app);
  dispatcher.sink<OffsetViewportEvent>().connect<&App::OnOffsetViewportEvent>(app);
  dispatcher.sink<PanLeftEvent>().connect<&App::OnPanLeftEvent>(app);
  dispatcher.sink<PanRightEvent>().connect<&App::OnPanRightEvent>(app);
  dispatcher.sink<PanUpEvent>().connect<&App::OnPanUpEvent>(app);
  dispatcher.sink<PanDownEvent>().connect<&App::OnPanDownEvent>(app);
  dispatcher.sink<IncreaseViewportZoomEvent>().connect<&App::OnIncreaseViewportZoomEvent>(app);
  dispatcher.sink<DecreaseViewportZoomEvent>().connect<&App::OnDecreaseViewportZoomEvent>(app);
  dispatcher.sink<ResetViewportZoomEvent>().connect<&App::OnResetViewportZoomEvent>(app);

  dispatcher.sink<AddRowEvent>().connect<&App::OnAddRowEvent>(app);
  dispatcher.sink<AddColumnEvent>().connect<&App::OnAddColumnEvent>(app);
  dispatcher.sink<RemoveRowEvent>().connect<&App::OnRemoveRowEvent>(app);
  dispatcher.sink<RemoveColumnEvent>().connect<&App::OnRemoveColumnEvent>(app);

  dispatcher.sink<AddLayerEvent>().connect<&App::OnAddLayerEvent>(app);
  dispatcher.sink<RemoveLayerEvent>().connect<&App::OnRemoveLayerEvent>(app);
  dispatcher.sink<SelectLayerEvent>().connect<&App::OnSelectLayerEvent>(app);
  dispatcher.sink<MoveLayerUpEvent>().connect<&App::OnMoveLayerUpEvent>(app);
  dispatcher.sink<MoveLayerDownEvent>().connect<&App::OnMoveLayerDownEvent>(app);
  dispatcher.sink<DuplicateLayerEvent>().connect<&App::OnDuplicateLayerEvent>(app);
  dispatcher.sink<SetLayerOpacityEvent>().connect<&App::OnSetLayerOpacityEvent>(app);
  dispatcher.sink<SetLayerVisibleEvent>().connect<&App::OnSetLayerVisibleEvent>(app);

  dispatcher.sink<ShowLayerPropertiesEvent>().connect<&App::OnShowLayerPropertiesEvent>(app);
  dispatcher.sink<ShowMapPropertiesEvent>().connect<&App::OnShowMapPropertiesEvent>(app);

  dispatcher.sink<AddPropertyEvent>().connect<&App::OnAddPropertyEvent>(app);
  dispatcher.sink<RemovePropertyEvent>().connect<&App::OnRemovePropertyEvent>(app);
  dispatcher.sink<RenamePropertyEvent>().connect<&App::OnRenamePropertyEvent>(app);
  dispatcher.sink<UpdatePropertyEvent>().connect<&App::OnUpdatePropertyEvent>(app);
  dispatcher.sink<ChangePropertyTypeEvent>().connect<&App::OnChangePropertyTypeEvent>(app);

  dispatcher.sink<SetTilesetSelectionEvent>().connect<&App::OnSetTilesetSelectionEvent>(app);

  dispatcher.sink<SelectToolEvent>().connect<&App::OnSelectToolEvent>(app);
  dispatcher.sink<MousePressedEvent>().connect<&App::OnMousePressedEvent>(app);
  dispatcher.sink<MouseReleasedEvent>().connect<&App::OnMouseReleasedEvent>(app);
  dispatcher.sink<MouseDragEvent>().connect<&App::OnMouseDragEvent>(app);
  dispatcher.sink<StampSequenceEvent>().connect<&App::OnStampSequenceEvent>(app);
  dispatcher.sink<EraserSequenceEvent>().connect<&App::OnEraserSequenceEvent>(app);
  dispatcher.sink<FloodEvent>().connect<&App::OnFloodEvent>(app);

  dispatcher.sink<ChangeCommandCapacityEvent>().connect<&App::OnChangeCommandCapacityEvent>(app);
  dispatcher.sink<QuitEvent>().connect<&App::OnQuitEvent>(app);
  // clang-format on
}

}  // namespace Tactile
