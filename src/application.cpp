#include "application.hpp"

#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <utility>  // move, forward

#include "application_events.hpp"
#include "core/commands/layers/add_layer_cmd.hpp"
#include "core/commands/layers/duplicate_layer_cmd.hpp"
#include "core/commands/layers/move_layer_down_cmd.hpp"
#include "core/commands/layers/move_layer_up_cmd.hpp"
#include "core/commands/layers/remove_layer_cmd.hpp"
#include "core/commands/layers/rename_layer_cmd.hpp"
#include "core/commands/layers/set_layer_opacity_cmd.hpp"
#include "core/commands/layers/set_layer_visibility_cmd.hpp"
#include "core/commands/maps/add_column_cmd.hpp"
#include "core/commands/maps/add_row_cmd.hpp"
#include "core/commands/maps/remove_column_cmd.hpp"
#include "core/commands/maps/remove_row_cmd.hpp"
#include "core/commands/maps/resize_map_cmd.hpp"
#include "core/commands/properties/add_property_cmd.hpp"
#include "core/commands/properties/change_property_type_cmd.hpp"
#include "core/commands/properties/remove_property_cmd.hpp"
#include "core/commands/properties/rename_property_cmd.hpp"
#include "core/commands/properties/update_property_cmd.hpp"
#include "core/commands/tilesets/add_tileset_cmd.hpp"
#include "core/commands/tilesets/remove_tileset_cmd.hpp"
#include "core/commands/tools/bucket_cmd.hpp"
#include "core/commands/tools/eraser_sequence_cmd.hpp"
#include "core/commands/tools/stamp_sequence_cmd.hpp"
#include "core/components/property_context.hpp"
#include "core/mouse.hpp"
#include "core/systems/layer_system.hpp"
#include "core/systems/map_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tool_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "gui/update_gui.hpp"
#include "gui/widgets/dialogs/map_import_error_dialog.hpp"
#include "gui/widgets/dialogs/resize_map_dialog.hpp"
#include "gui/widgets/dialogs/save_as_dialog.hpp"
#include "gui/widgets/layers/layer_dock.hpp"
#include "gui/widgets/toolbar/toolbar.hpp"
#include "gui/widgets/viewport/viewport_widget.hpp"
#include "io/parsing/map_parser.hpp"
#include "io/parsing/to_map_document.hpp"
#include "io/preferences.hpp"
#include "io/saving/save_map_document.hpp"
#include "io/session.hpp"
#include "shortcuts/shortcuts.hpp"
#include "utils/load_texture.hpp"

namespace Tactile {
namespace {

template <typename Command, typename... Args>
void Execute(Model& model, Args&&... args)
{
  if (auto* document = model.GetActiveDocument())
  {
    auto& commands = document->commands;
    commands.Push<Command>(document->registry, std::forward<Args>(args)...);
  }
}

template <typename Command, typename... Args>
void Register(Model& model, Args&&... args)
{
  if (auto* document = model.GetActiveDocument())
  {
    auto& commands = document->commands;
    commands.PushWithoutRedo<Command>(document->registry,
                                      std::forward<Args>(args)...);
  }
}

/* These variables are used by the "Toggle UI" feature */
constinit bool prev_show_layer_dock{true};
constinit bool prev_show_tileset_dock{true};
constinit bool prev_show_properties_dock{true};
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

  if (Prefs::GetRestoreLastSession())
  {
    RestoreLastSession(mModel);
  }

  while (!mQuit)
  {
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
  SavePreferences();
  SaveSession(mModel);
  UnloadTextures();
}

void Application::PollEvents()
{
  cen::event event;
  while (event.poll())
  {
    ImGui_ImplSDL2_ProcessEvent(event.data());

    if (event.is<cen::quit_event>())
    {
      mQuit = true;
      break;
    }
    else if (auto* keyEvent = event.try_get<cen::keyboard_event>())
    {
      // We don't care about these modifiers, they are just noise
      keyEvent->set_modifier(cen::key_mod::caps, false);
      keyEvent->set_modifier(cen::key_mod::num, false);
      keyEvent->set_modifier(cen::key_mod::mode, false);

      UpdateShortcuts(mModel, *keyEvent, mDispatcher);
    }
    else if (const auto* wheelEvent = event.try_get<cen::mouse_wheel_event>())
    {
      if (mModel.HasActiveDocument() &&
          mKeyboard.is_pressed(cen::scancodes::left_ctrl))
      {
        const auto dy = wheelEvent->y_scroll();
        if (dy > 0)
        {
          mDispatcher.enqueue<IncreaseZoomEvent>();
        }
        else if (dy < 0 && mModel.CanDecreaseViewportTileSize())
        {
          mDispatcher.enqueue<DecreaseZoomEvent>();
        }
      }
    }
  }
}

void Application::UpdateFrame()
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    const auto position = ImGui::GetMousePos();
    auto& mouse = registry->ctx<Mouse>();
    mouse.x = position.x;
    mouse.y = position.y;
  }

  mDispatcher.update();
  mModel.Update();
  UpdateGui(mModel, mDispatcher);
}

void Application::OnUndo()
{
  if (auto* document = mModel.GetActiveDocument())
  {
    document->commands.Undo();
  }
}

void Application::OnRedo()
{
  if (auto* document = mModel.GetActiveDocument())
  {
    document->commands.Redo();
  }
}

void Application::OnSetCommandCapacity(const SetCommandCapacityEvent& event)
{
  mModel.OnCommandCapacityChanged(event);
}

void Application::OnSave()
{
  if (auto* document = mModel.GetActiveDocument())
  {
    if (!document->path.empty())
    {
      IO::SaveMapDocument(*document);
      document->commands.MarkAsClean();

      auto& context = document->registry.ctx<PropertyContext>();
      context.name = document->path.filename().string();
    }
    else
    {
      OnOpenSaveAsDialog();
    }
  }
}

void Application::OnSaveAs(const SaveAsEvent& event)
{
  if (auto* document = mModel.GetActiveDocument())
  {
    document->path = event.path;
    OnSave();
  }
}

void Application::OnOpenSaveAsDialog()
{
  if (mModel.GetActiveDocument())
  {
    OpenSaveAsDialog();
  }
}

void Application::OnShowMapProperties()
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
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
  mModel.RemoveMap(event.id);
}

void Application::OnOpenMap(const OpenMapEvent& event)
{
  IO::MapParser parser{event.path};
  if (parser)
  {
    mModel.AddMap(IO::ToMapDocument(parser.GetData()));
  }
  else
  {
    OpenMapImportErrorDialog(parser.GetError());
  }
}

void Application::OnSelectMap(const SelectMapEvent& event)
{
  mModel.SelectMap(event.id);
}

void Application::OnSelectTool(const SelectToolEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::SelectTool(*registry, event.type);
  }
}

void Application::OnMousePressed(const MousePressedEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::ToolOnPressed(*registry, mDispatcher, event.info);
  }
}

void Application::OnMouseDrag(const MouseDragEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::ToolOnDragged(*registry, mDispatcher, event.info);
  }
}

void Application::OnMouseReleased(const MouseReleasedEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
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
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::OffsetViewport(*registry, event.dx, event.dy);
  }
}

void Application::OnPanLeft()
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::PanViewportLeft(*registry);
  }
}

void Application::OnPanRight()
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::PanViewportRight(*registry);
  }
}

void Application::OnPanUp()
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::PanViewportUp(*registry);
  }
}

void Application::OnPanDown()
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::PanViewportDown(*registry);
  }
}

void Application::OnIncreaseZoom()
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::IncreaseViewportZoom(*registry);
  }
}

void Application::OnDecreaseZoom()
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::DecreaseViewportZoom(*registry);
  }
}

void Application::OnResetZoom()
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::ResetViewportZoom(*registry);
  }
}

void Application::OnAddTileset(const AddTilesetEvent& event)
{
  if (auto info = LoadTexture(event.path))
  {
    Execute<AddTilesetCmd>(mModel,
                           std::move(*info),
                           event.tile_width,
                           event.tile_height);
  }
  else
  {
    CENTURION_LOG_WARN("Failed to load tileset texture!");
  }
}

void Application::OnRemoveTileset(const RemoveTilesetEvent& event)
{
  Execute<RemoveTilesetCmd>(mModel, event.id);
}

void Application::OnSelectTileset(const SelectTilesetEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::SelectTileset(*registry, event.id);
  }
}

void Application::OnSetTilesetSelection(const SetTilesetSelectionEvent& event)
{
  if (auto* registry = mModel.GetActiveRegistry())
  {
    Sys::UpdateTilesetSelection(*registry, event.selection);
  }
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
  if (auto* registry = mModel.GetActiveRegistry())
  {
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
  if (auto* registry = mModel.GetActiveRegistry())
  {
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
  if (auto* registry = mModel.GetActiveRegistry())
  {
    auto& current = registry->ctx<ActivePropertyContext>();
    current.entity = Sys::FindLayer(*registry, event.id);
  }
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

void Application::OnToggleUi()
{
  static bool show = false;

  if (!show)
  {
    prev_show_layer_dock = Prefs::GetShowLayerDock();
    prev_show_tileset_dock = Prefs::GetShowTilesetDock();
    prev_show_properties_dock = Prefs::GetShowPropertiesDock();
    prev_show_toolbar = IsToolbarVisible();
  }

  Prefs::SetShowLayerDock(show);
  Prefs::SetShowTilesetDock(show);
  Prefs::SetShowPropertiesDock(show);
  SetToolbarVisible(show);

  if (show)
  {
    Prefs::SetShowLayerDock(prev_show_layer_dock);
    Prefs::SetShowTilesetDock(prev_show_tileset_dock);
    Prefs::SetShowPropertiesDock(prev_show_properties_dock);
    SetToolbarVisible(prev_show_toolbar);
  }

  show = !show;
}

void Application::OnQuit()
{
  mQuit = true;
}

}  // namespace Tactile
