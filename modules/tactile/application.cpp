#include "application.hpp"

#include <utility>  // move, forward

#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include "application_events.hpp"
#include "core/components/property_context.hpp"
#include "core/mouse_pos.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tools/tool_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "core/utils/load_texture.hpp"
#include "editor/commands/commands.hpp"
#include "editor/gui/update_gui.hpp"
#include "editor/gui/widgets/dialogs/map_import_error_dialog.hpp"
#include "editor/gui/widgets/dialogs/resize_map_dialog.hpp"
#include "editor/gui/widgets/dialogs/save_as_dialog.hpp"
#include "editor/gui/widgets/layers/layer_dock.hpp"
#include "editor/gui/widgets/toolbar/toolbar.hpp"
#include "editor/gui/widgets/viewport/viewport_widget.hpp"
#include "editor/shortcuts/shortcuts.hpp"
#include "io/create_document_from_ir.hpp"
#include "io/history.hpp"
#include "io/map_parser.hpp"
#include "io/preferences.hpp"
#include "io/save_document.hpp"
#include "io/session.hpp"

namespace Tactile {
namespace {

template <typename Command, typename... Args>
void Execute(Model& model, Args&&... args)
{
  if (auto* document = model.GetActiveDocument()) {
    auto& commands = document->commands;
    commands.Push<Command>(document->registry, std::forward<Args>(args)...);
  }
}

template <typename Command, typename... Args>
void Register(Model& model, Args&&... args)
{
  if (auto* document = model.GetActiveDocument()) {
    auto& commands = document->commands;
    commands.PushWithoutRedo<Command>(document->registry, std::forward<Args>(args)...);
  }
}

/* These variables are used by the "Toggle UI" feature */
constinit bool prev_show_layer_dock{true};
constinit bool prev_show_tileset_dock{true};
constinit bool prev_show_properties_dock{true};
constinit bool prev_show_log_dock{false};
constinit bool prev_show_toolbar{true};

}  // namespace

Application::Application(cen::window&& window) : mWindow{std::move(window)}
{
  SubscribeToEvents(this, mDispatcher);
  LoadDefaultShortcuts();
}

auto Application::Run() -> int
{
  const auto& io = ImGui::GetIO();

  if (Prefs::GetRestoreLastSession()) {
    RestoreLastSession(mModel);
  }

  LoadFileHistory();

  while (!mQuit) {
    mKeyboard.update();
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

    cen::gl::swap(mWindow);
  }

  OnAboutToExit();

  mWindow.hide();
  return 0;
}

void Application::OnAboutToExit()
{
  SaveCurrentFilesToHistory();
  SavePreferences();
  SaveSession(mModel);
  SaveFileHistory();
  UnloadTextures();
}

void Application::SaveCurrentFilesToHistory()
{
  for (const auto& [id, document] : mModel) {
    if (!document->path.empty()) {
      AddFileToHistory(document->path);
    }
  }
}

void Application::PollEvents()
{
  cen::event event;
  while (event.poll()) {
    ImGui_ImplSDL2_ProcessEvent(event.data());

    if (event.is<cen::quit_event>()) {
      mQuit = true;
      break;
    }
    else if (auto* keyEvent = event.try_get<cen::keyboard_event>()) {
      // We don't care about these modifiers, they are just noise
      keyEvent->set_modifier(cen::key_mod::caps, false);
      keyEvent->set_modifier(cen::key_mod::num, false);
      keyEvent->set_modifier(cen::key_mod::mode, false);

      UpdateShortcuts(mModel, *keyEvent, mDispatcher);
    }
    else if (const auto* wheelEvent = event.try_get<cen::mouse_wheel_event>()) {
      // TODO support zooming in tilesets

      if (mModel.HasActiveDocument() && mKeyboard.is_pressed(cen::scancodes::left_ctrl)) {
        const auto dy = wheelEvent->y_scroll();
        if (dy > 0) {
          mDispatcher.enqueue<IncreaseZoomEvent>();
        }
        else if (dy < 0 && mModel.CanDecreaseViewportTileSize()) {
          mDispatcher.enqueue<DecreaseZoomEvent>();
        }
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
  UpdateGui(mModel, mDispatcher);
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
  mModel.OnCommandCapacityChanged(event);
}

void Application::OnSave()
{
  if (auto* document = mModel.GetActiveDocument()) {
    if (!document->path.empty()) {
      SaveDocument(*document);
      document->commands.MarkAsClean();

      auto& context = document->registry.ctx<PropertyContext>();
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
    OpenSaveAsDialog();
  }
}

void Application::OnShowMapProperties()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    auto& current = registry->ctx<ActivePropertyContext>();
    current.entity = entt::null;
  }
}

void Application::OnAddMap(const AddMapEvent& event)
{
  const auto id = mModel.AddMap(event.tile_width, event.tile_height);
  mModel.SelectMap(id);
}

void Application::OnCloseMap(const CloseMapEvent& event)
{
  SetLastClosedFile(mModel.GetPath(event.id));
  mModel.RemoveMap(event.id);
}

void Application::OnOpenMap(const OpenMapEvent& event)
{
  /* Just silently ignore the request if the map is already open */
  if (mModel.HasDocumentWithPath(event.path)) {
    cen::log::warn("Tried to open map that was already open!");
    return;
  }

  MapParser parser{event.path};
  if (parser) {
    mModel.AddMap(CreateDocumentFromIR(parser.GetData()));
    AddFileToHistory(event.path);
  }
  else {
    OpenMapImportErrorDialog(parser.GetError());
  }
}

void Application::OnSelectMap(const SelectMapEvent& event)
{
  mModel.SelectMap(event.id);
}

void Application::OnSelectTool(const SelectToolEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::SelectTool(*registry, event.type);
  }
}

void Application::OnMousePressed(const MousePressedEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::ToolOnPressed(*registry, mDispatcher, event.info);
  }
}

void Application::OnMouseDrag(const MouseDragEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::ToolOnDragged(*registry, mDispatcher, event.info);
  }
}

void Application::OnMouseReleased(const MouseReleasedEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::ToolOnReleased(*registry, mDispatcher, event.info);
  }
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
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::OffsetViewport(*registry, event.dx, event.dy);
  }
}

void Application::OnPanLeft()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::PanViewportLeft(*registry);
  }
}

void Application::OnPanRight()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::PanViewportRight(*registry);
  }
}

void Application::OnPanUp()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::PanViewportUp(*registry);
  }
}

void Application::OnPanDown()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::PanViewportDown(*registry);
  }
}

void Application::OnIncreaseZoom()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::IncreaseViewportZoom(*registry);
  }
}

void Application::OnDecreaseZoom()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::DecreaseViewportZoom(*registry);
  }
}

void Application::OnResetZoom()
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::ResetViewportZoom(*registry);
  }
}

void Application::OnAddTileset(const AddTilesetEvent& event)
{
  if (auto info = LoadTexture(event.path)) {
    Execute<AddTilesetCmd>(mModel, std::move(*info), event.tile_width, event.tile_height);
  }
  else {
    cen::log::error("Failed to load tileset texture!");
  }
}

void Application::OnRemoveTileset(const RemoveTilesetEvent& event)
{
  Execute<RemoveTilesetCmd>(mModel, event.id);
}

void Application::OnSelectTileset(const SelectTilesetEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::SelectTileset(*registry, event.id);
  }
}

void Application::OnSetTilesetSelection(const SetTilesetSelectionEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    Sys::UpdateTilesetSelection(*registry, event.selection);
  }
}

void Application::OnShowTilesetProperties(const ShowTilesetPropertiesEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    auto& current = registry->ctx<ActivePropertyContext>();
    current.entity = Sys::FindTileset(*registry, event.id);
  }
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
    const auto& map = registry->ctx<Map>();
    OpenResizeMapDialog(map.row_count, map.column_count);
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
    Sys::SelectLayer(*registry, event.id);
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
  OpenRenameLayerDialog(event.id);
}

void Application::OnRenameLayer(const RenameLayerEvent& event)
{
  Execute<RenameLayerCmd>(mModel, event.id, event.name);
}

void Application::OnShowLayerProperties(const ShowLayerPropertiesEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    auto& current = registry->ctx<ActivePropertyContext>();
    current.entity = Sys::FindLayer(*registry, event.id);
  }
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

void Application::OnSetPropertyContext(const SetPropertyContextEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry()) {
    auto& current = registry->ctx<ActivePropertyContext>();
    current.entity = event.entity;
  }
}

void Application::OnToggleUi()
{
  static bool show = false;

  if (!show) {
    prev_show_layer_dock = Prefs::GetShowLayerDock();
    prev_show_tileset_dock = Prefs::GetShowTilesetDock();
    prev_show_properties_dock = Prefs::GetShowPropertiesDock();
    prev_show_log_dock = Prefs::GetShowLogDock();
    prev_show_toolbar = IsToolbarVisible();
  }

  Prefs::SetShowLayerDock(show);
  Prefs::SetShowTilesetDock(show);
  Prefs::SetShowPropertiesDock(show);
  Prefs::SetShowLogDock(show);
  SetToolbarVisible(show);

  if (show) {
    Prefs::SetShowLayerDock(prev_show_layer_dock);
    Prefs::SetShowTilesetDock(prev_show_tileset_dock);
    Prefs::SetShowPropertiesDock(prev_show_properties_dock);
    Prefs::SetShowLogDock(prev_show_log_dock);
    SetToolbarVisible(prev_show_toolbar);
  }

  show = !show;
}

void Application::OnQuit()
{
  mQuit = true;
}

}  // namespace Tactile
