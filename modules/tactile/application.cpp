#include "application.hpp"

#include <utility>  // move, forward

#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <imgui_internal.h>

#include "application_events.hpp"
#include "cfg/configuration.hpp"
#include "core/components/attribute_context.hpp"
#include "core/mouse_pos.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tools/tool_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "core/viewport.hpp"
#include "editor/commands/commands.hpp"
#include "editor/gui/dialogs/save_as_dialog.hpp"
#include "editor/gui/toolbar/toolbar.hpp"
#include "editor/gui/viewport/map_view.hpp"
#include "editor/gui/viewport/viewport_widget.hpp"
#include "editor/shortcuts/shortcuts.hpp"
#include "io/history.hpp"
#include "io/maps/parser/map_parser.hpp"
#include "io/maps/restore_document_from_ir.hpp"
#include "io/maps/save_document.hpp"
#include "io/preferences.hpp"
#include "io/session.hpp"
#include "logging.hpp"

namespace tactile {
namespace {

template <typename Command, typename... Args>
void Execute(Model& model, Args&&... args)
{
  if (auto* document = model.GetActiveDocument()) {
    auto& commands = document->commands;
    commands.Push<Command>(document->registry, std::forward<Args>(args)...);
  }
  else {
    log_error("Could not execute a command due to no active document!");
  }
}

template <typename Command, typename... Args>
void Register(Model& model, Args&&... args)
{
  if (auto* document = model.GetActiveDocument()) {
    auto& commands = document->commands;
    commands.PushWithoutRedo<Command>(document->registry, std::forward<Args>(args)...);
  }
  else {
    log_error("Could not register a command due to no active document!");
  }
}

}  // namespace

Application::Application(app_configuration* configuration)
    : mConfiguration{configuration}
    , mIcons{mTextures}
{
  SubscribeToEvents(*this);
  LoadDefaultShortcuts();
}

auto Application::Run() -> int
{
  auto& window = mConfiguration->window();

  load_file_history();

  if (prefs::GetRestoreLastSession()) {
    restore_last_session(mModel, mTextures);
  }

  window.show();

  const auto& io = ImGui::GetIO();
  while (!mQuit) {
    mKeyboard.refresh();
    PollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    UpdateFrame();
    ImGui::EndFrame();

    ImGui::Render();

    glViewport(0,
               0,
               static_cast<int>(io.DisplaySize.x),
               static_cast<int>(io.DisplaySize.y));
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    cen::gl::swap(window);
  }

  OnAboutToExit();
  window.hide();

  return 0;
}

void Application::OnAboutToExit()
{
  SaveCurrentFilesToHistory();
  SavePreferences();
  save_session(mModel);
  save_file_history();
}

void Application::SaveCurrentFilesToHistory()
{
  for (const auto& [id, document] : mModel) {
    if (!document->path.empty()) {
      add_file_to_history(document->path);
    }
  }
}

void Application::PollEvents()
{
  cen::event_handler handler;
  while (handler.poll()) {
    ImGui_ImplSDL2_ProcessEvent(handler.data());

    switch (handler.type().value()) {
      case cen::event_type::quit:
        mQuit = true;
        break;

      case cen::event_type::key_up:
        [[fallthrough]];
      case cen::event_type::key_down:
        OnKeyboardEvent(handler.get<cen::keyboard_event>());
        break;

      case cen::event_type::mouse_wheel:
        OnMouseWheelEvent(handler.get<cen::mouse_wheel_event>());
        break;

      default:
        break;
    }
  }
}

void Application::OnKeyboardEvent(cen::keyboard_event event)
{
  /* We don't care about these modifiers, they are just noise */
  event.set_modifier(cen::key_mod::caps, false);
  event.set_modifier(cen::key_mod::num, false);
  event.set_modifier(cen::key_mod::mode, false);

  UpdateShortcuts(mModel, mWidgets, event, mDispatcher);
}

void Application::OnMouseWheelEvent(const cen::mouse_wheel_event& event)
{
  constexpr float scaling = 4.0f;

  const auto* registry = mModel.GetActiveRegistry();
  if (registry && !ImGui::GetTopMostPopupModal()) {
    if (IsMouseWithinViewport()) {
      const auto& viewport = registry->ctx<Viewport>();
      const auto dx = static_cast<float>(event.x()) * (viewport.tile_width / scaling);
      const auto dy = static_cast<float>(event.y()) * (viewport.tile_height / scaling);
      mDispatcher.enqueue<OffsetViewportEvent>(-dx, dy);
    }
    else if (mWidgets.IsTilesetDockHovered()) {
      const auto width = mWidgets.GetTilesetViewWidth();
      const auto height = mWidgets.GetTilesetViewHeight();
      if (width && height) {
        const auto entity = sys::find_active_tileset(*registry);
        TACTILE_ASSERT(entity != entt::null);

        const auto& viewport = registry->get<Viewport>(entity);

        const auto dx = static_cast<float>(event.x()) * (viewport.tile_width / scaling);
        const auto dy = static_cast<float>(event.y()) * (viewport.tile_height / scaling);
        mDispatcher.enqueue<OffsetBoundViewportEvent>(entity, -dx, dy, *width, *height);
      }
    }
  }
}

void Application::UpdateFrame()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    const auto position = ImGui::GetMousePos();
    auto& mouse = registry->ctx<MousePos>();
    mouse.x = position.x;
    mouse.y = position.y;
  }

  mDispatcher.update();
  mModel.Update();
  mWidgets.Update(mModel, mIcons, mDispatcher);
}

void Application::OnUndo()
{
  if (auto* document = mModel.GetActiveDocument()) {
    document->commands.Undo();
  }
}

void Application::OnRedo()
{
  if (auto* document = mModel.GetActiveDocument()) {
    document->commands.Redo();
  }
}

void Application::OnSetCommandCapacity(const SetCommandCapacityEvent& event)
{
  mModel.SetCommandCapacity(event.capacity);
}

void Application::OnSave()
{
  if (auto* document = mModel.GetActiveDocument()) {
    if (!document->path.empty()) {
      SaveDocument(*document);
      document->commands.MarkAsClean();

      auto& context = document->registry.ctx<comp::attribute_context>();
      context.name = document->path.filename().string();
    }
    else {
      OnOpenSaveAsDialog();
    }
  }
}

void Application::OnSaveAs(const SaveAsEvent& event)
{
  if (auto* document = mModel.GetActiveDocument()) {
    document->path = event.path;
    OnSave();
  }
}

void Application::OnOpenSaveAsDialog()
{
  if (mModel.HasActiveDocument()) {
    OpenSaveAsDialog(mDispatcher);
  }
}

void Application::OnShowSettings()
{
  mWidgets.ShowSettings();
}

void Application::OnShowNewMapDialog()
{
  mWidgets.ShowNewMapDialog();
}

void Application::OnShowOpenMapDialog()
{
  mWidgets.ShowOpenMapDialog();
}

void Application::OnShowMapProperties()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    auto& current = registry->ctx<comp::active_attribute_context>();
    current.entity = entt::null;
  }
}

void Application::OnCreateMap(const CreateMapEvent& event)
{
  const auto id = mModel.AddMap(event.tile_width,
                                event.tile_height,
                                event.row_count,
                                event.column_count);
  mModel.SelectMap(id);
}

void Application::OnCloseMap(const CloseMapEvent& event)
{
  if (mModel.HasPath(event.id)) {
    set_last_closed_file(mModel.GetPath(event.id));
  }
  mModel.RemoveMap(event.id);
}

void Application::OnOpenMap(const OpenMapEvent& event)
{
  /* Just silently ignore the request if the map is already open */
  if (mModel.HasDocumentWithPath(event.path)) {
    log_warning("Tried to open map that was already open!");
    return;
  }

  parsing::map_parser parser;
  parser.parse_map(event.path);

  if (parser.is_okay()) {
    mModel.AddMap(restore_document(parser.data(), mTextures));
    add_file_to_history(event.path);
  }
  else {
    mWidgets.ShowMapImportErrorDialog(parser.error());
  }
}

void Application::OnSelectMap(const SelectMapEvent& event)
{
  mModel.SelectMap(event.id);
}

void Application::OnSelectTool(const SelectToolEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::SelectTool(registry, event.type);
}

void Application::OnMousePressed(const MousePressedEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::ToolOnPressed(registry, mDispatcher, event.info);
}

void Application::OnMouseDrag(const MouseDragEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::ToolOnDragged(registry, mDispatcher, event.info);
}

void Application::OnMouseReleased(const MouseReleasedEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::ToolOnReleased(registry, mDispatcher, event.info);
}

void Application::OnStampSequence(StampSequenceEvent event)
{
  Register<StampSequenceCmd>(mModel,
                             std::move(event.old_state),
                             std::move(event.sequence));
}

void Application::OnEraserSequence(EraserSequenceEvent event)
{
  Register<EraserSequenceCmd>(mModel, std::move(event.old_state));
}

void Application::OnFlood(const FloodEvent& event)
{
  Execute<BucketCmd>(mModel, event.origin, event.replacement);
}

void Application::OnCenterViewport()
{
  CenterViewport();
}

void Application::OnOffsetViewport(const OffsetViewportEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::OffsetViewport(registry, event.dx, event.dy);
}

void Application::OnOffsetBoundViewport(const OffsetBoundViewportEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::OffsetBoundViewport(registry,
                           event.entity,
                           event.dx,
                           event.dy,
                           event.view_width,
                           event.view_height);
}

void Application::OnPanLeft()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::PanViewportLeft(registry);
}

void Application::OnPanRight()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::PanViewportRight(registry);
}

void Application::OnPanUp()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::PanViewportUp(registry);
}

void Application::OnPanDown()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::PanViewportDown(registry);
}

void Application::OnIncreaseZoom()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::IncreaseViewportZoom(registry);
}

void Application::OnDecreaseZoom()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::DecreaseViewportZoom(registry);
}

void Application::OnResetZoom()
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::ResetViewportZoom(registry);
}

void Application::OnShowAddTilesetDialog()
{
  mWidgets.ShowAddTilesetDialog();
}

void Application::OnAddTileset(const AddTilesetEvent& event)
{
  if (auto info = mTextures.load(event.path)) {
    Execute<AddTilesetCmd>(mModel, std::move(*info), event.tile_width, event.tile_height);
  }
  else {
    log_error("Failed to load tileset texture!");
  }
}

void Application::OnRemoveTileset(const RemoveTilesetEvent& event)
{
  Execute<RemoveTilesetCmd>(mModel, event.id);
}

void Application::OnSelectTileset(const SelectTilesetEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::select_tileset(registry, event.id);
}

void Application::OnSetTilesetSelection(const SetTilesetSelectionEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  sys::update_tileset_selection(registry, event.selection);
}

void Application::OnSetTilesetName(const SetTilesetNameEvent& event)
{
  Execute<SetTilesetNameCmd>(mModel, event.id, event.name);
}

void Application::OnAddRow()
{
  Execute<AddRowCmd>(mModel);
}

void Application::OnAddColumn()
{
  Execute<AddColumnCmd>(mModel);
}

void Application::OnRemoveRow()
{
  Execute<RemoveRowCmd>(mModel);
}

void Application::OnRemoveColumn()
{
  Execute<RemoveColumnCmd>(mModel);
}

void Application::OnResizeMap(const ResizeMapEvent& event)
{
  Execute<ResizeMapCmd>(mModel, event.row_count, event.col_count);
}

void Application::OnOpenResizeMapDialog()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    const auto& map = registry->ctx<MapInfo>();
    mWidgets.ShowResizeMapDialog(map.row_count, map.column_count);
  }
}

void Application::OnAddLayer(const AddLayerEvent& event)
{
  Execute<AddLayerCmd>(mModel, event.type);
}

void Application::OnRemoveLayer(const RemoveLayerEvent& event)
{
  Execute<RemoveLayerCmd>(mModel, event.id);
}

void Application::OnSelectLayer(const SelectLayerEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    auto& active = registry->ctx<comp::active_layer>();
    active.entity = sys::find_layer(*registry, event.id);
  }
}

void Application::OnMoveLayerUp(const MoveLayerUpEvent& event)
{
  Execute<MoveLayerUpCmd>(mModel, event.id);
}

void Application::OnMoveLayerDown(const MoveLayerDownEvent& event)
{
  Execute<MoveLayerDownCmd>(mModel, event.id);
}

void Application::OnDuplicateLayer(const DuplicateLayerEvent& event)
{
  Execute<DuplicateLayerCmd>(mModel, event.id);
}

void Application::OnSetLayerOpacity(const SetLayerOpacityEvent& event)
{
  Execute<SetLayerOpacityCmd>(mModel, event.id, event.opacity);
}

void Application::OnSetLayerVisible(const SetLayerVisibleEvent& event)
{
  Execute<SetLayerVisibilityCmd>(mModel, event.id, event.visible);
}

void Application::OnOpenRenameLayerDialog(const OpenRenameLayerDialogEvent& event)
{
  mWidgets.ShowRenameLayerDialog(event.id);
}

void Application::OnRenameLayer(const RenameLayerEvent& event)
{
  Execute<RenameLayerCmd>(mModel, event.id, event.name);
}

void Application::OnSetObjectName(const SetObjectNameEvent& event)
{
  Execute<SetObjectNameCmd>(mModel, event.id, event.name);
}

void Application::OnMoveObject(const MoveObjectEvent& event)
{
  Register<MoveObjectCmd>(mModel,
                          event.id,
                          event.old_x,
                          event.old_y,
                          event.new_x,
                          event.new_y);
}

void Application::OnSetObjectVisibility(const SetObjectVisibilityEvent& event)
{
  Execute<SetObjectVisibilityCmd>(mModel, event.id, event.visible);
}

void Application::OnSetObjectTag(const SetObjectTagEvent& event)
{
  Execute<SetObjectTagCmd>(mModel, event.id, event.tag);
}

void Application::OnSpawnObjectContextMenu(const SpawnObjectContextMenuEvent&)
{
  OpenObjectContextMenu();
}

void Application::OnShowAddPropertyDialog()
{
  mWidgets.ShowAddPropertyDialog();
}

void Application::OnShowRenamePropertyDialog(const ShowRenamePropertyDialogEvent& event)
{
  mWidgets.ShowRenamePropertyDialog(event.name);
}

void Application::OnShowChangePropertyTypeDialog(
    const ShowChangePropertyTypeDialogEvent& event)
{
  mWidgets.ShowChangePropertyTypeDialog(event.name, event.current_type);
}

void Application::OnAddProperty(const AddPropertyEvent& event)
{
  Execute<AddPropertyCmd>(mModel, event.name, event.type);
}

void Application::OnRemoveProperty(const RemovePropertyEvent& event)
{
  Execute<RemovePropertyCmd>(mModel, event.name);
}

void Application::OnRenameProperty(const RenamePropertyEvent& event)
{
  Execute<RenamePropertyCmd>(mModel, event.old_name, event.new_name);
}

void Application::OnUpdateProperty(const UpdatePropertyEvent& event)
{
  Execute<UpdatePropertyCmd>(mModel, event.name, event.value);
}

void Application::OnChangePropertyType(const ChangePropertyTypeEvent& event)
{
  Execute<ChangePropertyTypeCmd>(mModel, event.name, event.type);
}

void Application::OnInspectContext(const InspectContextEvent& event)
{
  auto& registry = mModel.GetActiveRegistryRef();
  auto& current = registry.ctx<comp::active_attribute_context>();
  current.entity = event.entity;
}

void Application::OnOpenComponentEditor()
{
  mWidgets.ShowComponentEditor(mModel);
}

void Application::OnCreateComponentDef(const CreateComponentDefEvent& event)
{
  Execute<CreateComponentDefCmd>(mModel, event.name);
}

void Application::OnRemoveComponentDef(const RemoveComponentDefEvent& event)
{
  Execute<RemoveComponentDefCmd>(mModel, event.id);
}

void Application::OnRenameComponentDef(const RenameComponentDefEvent& event)
{
  Execute<RenameComponentCmd>(mModel, event.id, event.name);
}

void Application::OnCreateComponentAttribute(const CreateComponentAttributeEvent& event)
{
  Execute<CreateComponentAttributeCmd>(mModel, event.id, event.name);
}

void Application::OnRemoveComponentAttribute(const RemoveComponentAttributeEvent& event)
{
  Execute<RemoveComponentAttributeCmd>(mModel, event.id, event.name);
}

void Application::OnRenameComponentAttribute(const RenameComponentAttributeEvent& event)
{
  Execute<RenameComponentAttributeCmd>(mModel, event.id, event.previous, event.updated);
}

void Application::OnDuplicateComponentAttribute(
    const DuplicateComponentAttributeEvent& event)
{
  Execute<DuplicateComponentAttributeCmd>(mModel, event.id, event.attribute);
}

void Application::OnSetComponentAttributeType(const SetComponentAttributeTypeEvent& event)
{
  Execute<SetComponentAttributeTypeCmd>(mModel, event.id, event.attribute, event.type);
}

void Application::OnUpdateComponentDefAttribute(
    const UpdateComponentDefAttributeEvent& event)
{
  Execute<UpdateComponentAttributeCmd>(mModel, event.id, event.attribute, event.value);
}

void Application::OnAddComponent(const AddComponentEvent& event)
{
  Execute<AddComponentCmd>(mModel, event.context, event.component);
}

void Application::OnRemoveComponent(const RemoveComponentEvent& event)
{
  Execute<RemoveComponentCmd>(mModel, event.context, event.component);
}

void Application::OnUpdateComponent(const UpdateComponentEvent& event)
{
  Execute<UpdateComponentCmd>(mModel,
                              event.context,
                              event.component,
                              event.attribute,
                              event.value);
}

void Application::OnResetComponentValues(const ResetComponentValuesEvent& event)
{
  Execute<ResetComponentCmd>(mModel, event.context, event.component);
}

void Application::OnToggleUi()
{
  if (ImGui::GetTopMostPopupModal() != nullptr) {
    return;
  }

  static bool show = false;

  if (!show) {
    mWidgetShowState.prev_show_layer_dock = prefs::GetShowLayerDock();
    mWidgetShowState.prev_show_tileset_dock = prefs::GetShowTilesetDock();
    mWidgetShowState.prev_show_properties_dock = prefs::GetShowPropertiesDock();
    mWidgetShowState.prev_show_log_dock = prefs::GetShowLogDock();
    mWidgetShowState.prev_show_component_dock = prefs::GetShowComponentDock();
    mWidgetShowState.prev_show_toolbar = mWidgets.IsToolbarVisible();
  }

  prefs::SetShowLayerDock(show);
  prefs::SetShowTilesetDock(show);
  prefs::SetShowPropertiesDock(show);
  prefs::SetShowLogDock(show);
  prefs::SetShowComponentDock(show);
  mWidgets.SetToolbarVisible(show);

  if (show) {
    prefs::SetShowLayerDock(mWidgetShowState.prev_show_layer_dock);
    prefs::SetShowTilesetDock(mWidgetShowState.prev_show_tileset_dock);
    prefs::SetShowPropertiesDock(mWidgetShowState.prev_show_properties_dock);
    prefs::SetShowLogDock(mWidgetShowState.prev_show_log_dock);
    prefs::SetShowComponentDock(mWidgetShowState.prev_show_component_dock);
    mWidgets.SetToolbarVisible(mWidgetShowState.prev_show_toolbar);
  }

  show = !show;
}

void Application::OnQuit()
{
  mQuit = true;
}

}  // namespace tactile
