#include "application.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <utility>  // move

#include "application_events.hpp"
#include "core/components/property_context.hpp"
#include "core/map.hpp"
#include "core/systems/layer_system.hpp"
#include "core/systems/map_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tool_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "events/viewport/decrease_viewport_zoom_event.hpp"
#include "events/viewport/increase_viewport_zoom_event.hpp"
#include "gui/update_gui.hpp"
#include "gui/widgets/dialogs/map_import_error_dialog.hpp"
#include "gui/widgets/dialogs/save_as_dialog.hpp"
#include "gui/widgets/viewport/viewport_widget.hpp"
#include "io/parsing/map_parser.hpp"
#include "io/parsing/to_map_document.hpp"
#include "io/preferences.hpp"
#include "io/saving/save_map_document.hpp"
#include "io/session.hpp"
#include "shortcuts/shortcuts.hpp"
#include "utils/load_texture.hpp"

namespace Tactile {

Application::Application(cen::window&& window)
    : mWindow{std::move(window)}
    , mModel{std::make_unique<Model>()}
{
  SubscribeToEvents(this, mDispatcher);
  LoadDefaultShortcuts();
}

auto Application::Run() -> int
{
  const auto& io = ImGui::GetIO();

  if (Prefs::GetRestoreLastSession())
  {
    RestoreLastSession(*mModel);
  }

  while (!mQuit)
  {
    mKeyboard.update();
    PollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(mWindow.get());

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
  SaveSession(*mModel);
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

      UpdateShortcuts(*mModel, *keyEvent, mDispatcher);
    }
    else if (const auto* wheelEvent = event.try_get<cen::mouse_wheel_event>())
    {
      if (mModel->HasActiveDocument() &&
          mKeyboard.is_pressed(cen::scancodes::left_ctrl))
      {
        const auto dy = wheelEvent->y_scroll();
        if (dy > 0)
        {
          mDispatcher.enqueue<IncreaseViewportZoomEvent>();
        }
        else if (dy < 0 && mModel->CanDecreaseViewportTileSize())
        {
          mDispatcher.enqueue<DecreaseViewportZoomEvent>();
        }
      }
    }
  }
}

void Application::UpdateFrame()
{
  mDispatcher.update();
  mModel->Update();
  UpdateGui(*mModel, mDispatcher);
}

void Application::OnAddMapEvent(const AddMapEvent& event)
{
  const auto id = mModel->AddMap(event.tile_width, event.tile_height);
  mModel->SelectMap(id);
}

void Application::OnCloseMapEvent(const CloseMapEvent& event)
{
  mModel->RemoveMap(event.id);
}

void Application::OnOpenMapEvent(const OpenMapEvent& event)
{
  IO::MapParser parser{event.path};
  if (parser)
  {
    mModel->AddMap(IO::ToMapDocument(parser.GetData()));
  }
  else
  {
    OpenMapImportErrorDialog(parser.GetError());
  }
}

void Application::OnSaveEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    if (!document->path.empty())
    {
      IO::SaveMapDocument(*document);
      document->commands.MarkAsClean();
    }
    else
    {
      OnSaveAsRequestEvent();
    }
  }
}

void Application::OnSaveAsEvent(const SaveAsEvent& event)
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->path = event.path;
    OnSaveEvent();
  }
}

void Application::OnSaveAsRequestEvent()
{
  if (mModel->GetActiveDocument())
  {
    OpenSaveAsDialog();
  }
}

void Application::OnAddTilesetEvent(const AddTilesetEvent& event)
{
  if (const auto info = LoadTexture(event.path))
  {
    if (auto* registry = mModel->GetActiveRegistry())
    {
      Sys::AddTileset(*registry, *info, event.tile_width, event.tile_height);
    }
  }
  else
  {
    CENTURION_LOG_WARN("Failed to load tileset texture!");
  }
}

void Application::OnUndoEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->commands.Undo();
  }
}

void Application::OnRedoEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->commands.Redo();
  }
}

void Application::OnSelectToolEvent(const SelectToolEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::SelectTool(*registry, event.type);
  }
}

void Application::OnMousePressedEvent(const MousePressedEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::ToolOnPressed(*registry, event.info);
  }
}

void Application::OnMouseReleasedEvent(const MouseReleasedEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::ToolOnReleased(*registry, event.info);
  }
}

void Application::OnMouseDragEvent(const MouseDragEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::ToolOnDragged(*registry, event.info);
  }
}

void Application::OnCenterViewportEvent()
{
  CenterViewport();
}

void Application::OnOffsetViewportEvent(const OffsetViewportEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::OffsetViewport(*registry, event.dx, event.dy);
  }
}

void Application::OnPanLeftEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::PanViewportLeft(*registry);
  }
}

void Application::OnPanRightEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::PanViewportRight(*registry);
  }
}

void Application::OnPanUpEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::PanViewportUp(*registry);
  }
}

void Application::OnPanDownEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::PanViewportDown(*registry);
  }
}

void Application::OnIncreaseViewportZoomEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::IncreaseViewportZoom(*registry);
  }
}

void Application::OnDecreaseViewportZoomEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::DecreaseViewportZoom(*registry);
  }
}

void Application::OnResetViewportZoomEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::ResetViewportZoom(*registry);
  }
}

void Application::OnSelectMapEvent(const SelectMapEvent& event)
{
  mModel->SelectMap(event.id);
}

void Application::OnSelectTilesetEvent(const SelectTilesetEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::SelectTileset(*registry, event.id);
  }
}

void Application::OnRemoveTilesetEvent(const RemoveTilesetEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::RemoveTileset(*registry, event.id);
  }
}

void Application::OnAddRowEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::AddRow(*registry);
  }
}

void Application::OnAddColumnEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::AddColumn(*registry);
  }
}

void Application::OnRemoveRowEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::RemoveRow(*registry);
  }
}

void Application::OnRemoveColumnEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::RemoveColumn(*registry);
  }
}

void Application::OnAddLayerEvent(const AddLayerEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    switch (event.type)
    {
      case LayerType::TileLayer:
        Sys::AddTileLayer(*registry);
        break;

      case LayerType::ObjectLayer:
        Sys::AddObjectLayer(*registry);
        break;

      case LayerType::GroupLayer:
        Sys::AddGroupLayer(*registry);
        break;
    }
  }
}

void Application::OnRemoveLayerEvent(const RemoveLayerEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::RemoveLayer(*registry, event.id);
  }
}

void Application::OnMoveLayerUpEvent(const MoveLayerUpEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::MoveLayerUp(*registry, event.id);
  }
}

void Application::OnMoveLayerDownEvent(const MoveLayerDownEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::MoveLayerDown(*registry, event.id);
  }
}

void Application::OnDuplicateLayerEvent(const DuplicateLayerEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::DuplicateLayer(*registry, event.id);
  }
}

void Application::OnSetLayerOpacityEvent(const SetLayerOpacityEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::SetLayerOpacity(*registry, event.id, event.opacity);
  }
}

void Application::OnSetLayerVisibleEvent(const SetLayerVisibleEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::SetLayerVisible(*registry, event.id, event.visible);
  }
}

void Application::OnShowLayerPropertiesEvent(const ShowLayerPropertiesEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    auto& current = registry->ctx<ActivePropertyContext>();
    current.entity = Sys::FindLayer(*registry, event.id);
  }
}

void Application::OnShowMapPropertiesEvent()
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    auto& current = registry->ctx<ActivePropertyContext>();
    current.entity = entt::null;
  }
}

void Application::OnSelectLayerEvent(const SelectLayerEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::SelectLayer(*registry, event.id);
  }
}

void Application::OnAddPropertyEvent(const AddPropertyEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::AddProperty(*registry, event.name, event.type);
  }
}

void Application::OnRemovePropertyEvent(const RemovePropertyEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::RemoveProperty(*registry, event.name);
  }
}

void Application::OnRenamePropertyEvent(const RenamePropertyEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::RenameProperty(*registry, event.old_name, event.new_name);
  }
}

void Application::OnSetPropertyValueEvent(const SetPropertyValueEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::UpdateProperty(*registry, event.name, event.property);
  }
}

void Application::OnChangePropertyTypeEvent(const ChangePropertyTypeEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::ChangePropertyType(*registry, event.name, event.type);
  }
}

void Application::OnSetTilesetSelectionEvent(const SetTilesetSelectionEvent& event)
{
  if (auto* registry = mModel->GetActiveRegistry())
  {
    Sys::UpdateTilesetSelection(*registry, event.selection);
  }
}

void Application::OnChangeCommandCapacityEvent(
    const ChangeCommandCapacityEvent& event)
{
  mModel->OnCommandCapacityChanged(event);
}

void Application::OnQuitEvent()
{
  mQuit = true;
}

}  // namespace Tactile
