#include "application_events.hpp"

#include "application.hpp"
#include "core/events/maps/show_map_properties_event.hpp"
#include "core/events/viewport/center_viewport_event.hpp"
#include "core/events/viewport/decrease_viewport_zoom_event.hpp"
#include "core/events/viewport/increase_viewport_zoom_event.hpp"
#include "core/events/viewport/reset_viewport_zoom_event.hpp"

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

  dispatcher.sink<AddTilesetEvent>().connect<&App::OnAddTilesetEvent>(app);
  dispatcher.sink<SelectTilesetEvent>().connect<&App::OnSelectTilesetEvent>(app);
  dispatcher.sink<RemoveTilesetEvent>().connect<&App::OnRemoveTilesetEvent>(app);

  dispatcher.sink<SelectMapEvent>().connect<&App::OnSelectMapEvent>(app);
  dispatcher.sink<CenterViewportEvent>().connect<&App::OnCenterViewportEvent>(app);
  dispatcher.sink<OffsetViewportEvent>().connect<&App::OnOffsetViewportEvent>(app);
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
  dispatcher.sink<SetPropertyValueEvent>().connect<&App::OnSetPropertyValueEvent>(app);
  dispatcher.sink<ChangePropertyTypeEvent>().connect<&App::OnChangePropertyTypeEvent>(app);

  dispatcher.sink<SetTilesetSelectionEvent>().connect<&App::OnSetTilesetSelectionEvent>(app);

  dispatcher.sink<SelectToolEvent>().connect<&App::OnSelectToolEvent>(app);
  dispatcher.sink<MousePressedEvent>().connect<&App::OnMousePressedEvent>(app);
  dispatcher.sink<MouseReleasedEvent>().connect<&App::OnMouseReleasedEvent>(app);
  dispatcher.sink<MouseDragEvent>().connect<&App::OnMouseDragEvent>(app);

  dispatcher.sink<QuitEvent>().connect<&App::OnQuitEvent>(app);
  // clang-format on
}

}  // namespace Tactile
