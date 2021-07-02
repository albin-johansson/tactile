#include "application.hpp"

#include <imgui.h>

#include <utility>  // move

#include "gui/cursors.hpp"
#include "gui/icons.hpp"
#include "gui/update_gui.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "gui/widgets/menus/file_menu.hpp"
#include "gui/widgets/viewport/viewport_widget.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "io/preferences.hpp"
#include "utils/load_texture.hpp"

namespace Tactile {

Application::Application(cen::window&& window)
    : mWindow{std::move(window)}
    , mModel{std::make_unique<Model>()}
{
  SubscribeToEvents();
  LoadPreferences();
  LoadCursors();
  LoadIcons();

  OnAddMapEvent();

  auto& map = mModel->GetActiveMap();
  map.AddTileLayer();
  map.AddObjectLayer();
  map.SelectLayer(1_layer);

  auto* document = mModel->GetActiveDocument();

  using namespace std::string_literals;
  document->AddProperty("Aa", "Foo"s);
  document->AddProperty("Ab", "Bar"s);

  document->AddProperty("Ba", 123);
  document->AddProperty("Bb", 1337);

  document->AddProperty("Ca", 12.3f);
  document->AddProperty("Cb", 42.5f);

  document->AddProperty("Da", true);
  document->AddProperty("Db", false);

  document->AddProperty("Ea", std::filesystem::path{"foo.json"});
  document->AddProperty("Eb", std::filesystem::path{"bar.json"});

  document->AddProperty("Fa", object_ref{42});
  document->AddProperty("Fb", object_ref{12});

  document->AddProperty("Ga", cen::colors::magenta);
  document->AddProperty("Gb", cen::colors::cyan);
}

auto Application::Run() -> int
{
  const auto& io = ImGui::GetIO();

  while (!mQuit)
  {
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

void Application::SubscribeToEvents()
{
  // clang-format off
  mDispatcher.sink<UndoEvent>().connect<&Application::OnUndoEvent>(this);
  mDispatcher.sink<RedoEvent>().connect<&Application::OnRedoEvent>(this);

  mDispatcher.sink<AddMapEvent>().connect<&Application::OnAddMapEvent>(this);
  mDispatcher.sink<CloseMapEvent>().connect<&Application::OnCloseMapEvent>(this);
  mDispatcher.sink<OpenMapEvent>().connect<&Application::OnOpenMapEvent>(this);

  mDispatcher.sink<AddTilesetEvent>().connect<&Application::OnAddTilesetEvent>(this);
  mDispatcher.sink<SelectTilesetEvent>().connect<&Application::OnSelectTilesetEvent>(this);
  mDispatcher.sink<RemoveTilesetEvent>().connect<&Application::OnRemoveTilesetEvent>(this);

  mDispatcher.sink<CenterViewportEvent>().connect<&Application::OnCenterViewportEvent>(this);
  mDispatcher.sink<SelectLayerEvent>().connect<&Application::OnSelectLayerEvent>(this);
  mDispatcher.sink<SelectMapEvent>().connect<&Application::OnSelectMapEvent>(this);

  mDispatcher.sink<AddRowEvent>().connect<&Application::OnAddRowEvent>(this);
  mDispatcher.sink<AddColumnEvent>().connect<&Application::OnAddColumnEvent>(this);
  mDispatcher.sink<RemoveRowEvent>().connect<&Application::OnRemoveRowEvent>(this);
  mDispatcher.sink<RemoveColumnEvent>().connect<&Application::OnRemoveColumnEvent>(this);

  mDispatcher.sink<SetPropertyValueEvent>().connect<&Application::OnSetPropertyValueEvent>(this);
  mDispatcher.sink<SetTilesetSelectionEvent>().connect<&Application::OnSetTilesetSelectionEvent>(this);

  mDispatcher.sink<SelectToolEvent>().connect<&Application::OnSelectToolEvent>(this);
  mDispatcher.sink<MousePressedEvent>().connect<&Application::OnMousePressedEvent>(this);
  mDispatcher.sink<MouseReleasedEvent>().connect<&Application::OnMouseReleasedEvent>(this);
  mDispatcher.sink<MouseDragEvent>().connect<&Application::OnMouseDragEvent>(this);

  mDispatcher.sink<QuitEvent>().connect<&Application::OnQuitEvent>(this);
  // clang-format on
}

void Application::OnAboutToExit()
{
  UnloadIcons();
  UnloadCursors();
  SavePreferences();

  for (const auto texture : mTextures)
  {
    CENTURION_LOG_DEBUG("Deleting texture %u...", texture);
    glDeleteTextures(1, &texture);
  }
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
    else if (const auto* keyEvent = event.try_get<cen::keyboard_event>())
    {
      if (keyEvent->pressed())
      {
        const auto ctrl = keyEvent->is_active(cen::key_modifier::ctrl);
        const auto shift = keyEvent->is_active(cen::key_modifier::shift);
        const auto alt = keyEvent->is_active(cen::key_modifier::alt);

        const auto key = keyEvent->scan();
        if (ctrl && alt)
        {
          OnCtrlAltKeyStroke(key);
        }
        else if (alt && shift)
        {
          OnAltShiftKeyStroke(key);
        }
        else if (ctrl)
        {
          OnCtrlKeyStroke(key);
        }
        else if (alt)
        {
          OnAltKeyStroke(key);
        }
        else
        {
          OnKeyStroke(key);
        }
      }
    }
  }
}

void Application::UpdateFrame()
{
  mDispatcher.update();
  UpdateGui(*mModel, mDispatcher);
}

void Application::OnCtrlAltKeyStroke(const cen::scan_code key)
{
  if (key == cen::scancodes::s)
  {
    EnableSettingsDialog();
  }
}

void Application::OnAltShiftKeyStroke(const cen::scan_code key)
{
  if (key == cen::scancodes::r)
  {
    OnRemoveRowEvent();
  }
  else if (key == cen::scancodes::c)
  {
    OnRemoveColumnEvent();
  }
}

void Application::OnCtrlKeyStroke(const cen::scan_code key)
{
  const auto* document = mModel->GetActiveDocument();
  if (key == cen::scancodes::n)
  {
    OnAddMapEvent();
  }
  else if (key == cen::scancodes::g)
  {
    Prefs::SetShowGrid(!Prefs::GetShowGrid());
  }
  else if (key == cen::scancodes::o)
  {
    EnableOpenMapDialog();
  }
  else if (key == cen::scancodes::t)
  {
    EnableTilesetDialog();
  }
  else if (key == cen::scancodes::space)
  {
    OnCenterViewportEvent();
  }
  else if (key == cen::scancodes::z && document && document->CanUndo())
  {
    OnUndoEvent();
  }
  else if (key == cen::scancodes::y && document && document->CanRedo())
  {
    OnRedoEvent();
  }
}

void Application::OnAltKeyStroke(const cen::scan_code key)
{
  if (key == cen::scancodes::r)
  {
    OnAddRowEvent();
  }
  else if (key == cen::scancodes::c)
  {
    OnAddColumnEvent();
  }
}

void Application::OnKeyStroke(const cen::scan_code key)
{
  if (key == cen::scancodes::s)
  {
    OnSelectToolEvent(SelectToolEvent{.type = MouseToolType::Stamp});
  }
  else if (key == cen::scancodes::b)
  {
    OnSelectToolEvent(SelectToolEvent{.type = MouseToolType::Bucket});
  }
  else if (key == cen::scancodes::e)
  {
    OnSelectToolEvent(SelectToolEvent{.type = MouseToolType::Eraser});
  }
}

void Application::OnAddMapEvent()
{
  const auto id = mModel->AddMap();
  mModel->SelectMap(id);
}

void Application::OnCloseMapEvent(const CloseMapEvent& event)
{
  mModel->RemoveMap(event.id);
}

void Application::OnOpenMapEvent(const OpenMapEvent& event)
{}

void Application::OnAddTilesetEvent(const AddTilesetEvent& event)
{
  if (const auto info = LoadTexture(event.path))
  {
    CENTURION_LOG_DEBUG("Loaded texture with ID %u", info->texture);
    mTextures.push_back(info->texture);

    auto* document = mModel->GetActiveDocument();
    document->AddTileset(*info, event.tile_width, event.tile_height);
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
    document->Undo();
  }
}

void Application::OnRedoEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->Redo();
  }
}

void Application::OnSelectToolEvent(const SelectToolEvent& event)
{
  mModel->SelectTool(event.type);
}

void Application::OnMousePressedEvent(const MousePressedEvent& event)
{
  mModel->OnMousePressed(event);
}

void Application::OnMouseReleasedEvent(const MouseReleasedEvent& event)
{
  mModel->OnMouseReleased(event);
}

void Application::OnMouseDragEvent(const MouseDragEvent& event)
{
  mModel->OnMouseDragged(event);
}

void Application::OnCenterViewportEvent()
{
  CenterViewport();
}

void Application::OnSelectMapEvent(const SelectMapEvent& event)
{
  mModel->SelectMap(event.id);
}

void Application::OnSelectLayerEvent(const SelectLayerEvent& event)
{
  auto& map = mModel->GetActiveMap();
  map.SelectLayer(event.id);
}

void Application::OnSelectTilesetEvent(const SelectTilesetEvent& event)
{
  auto* document = mModel->GetActiveDocument();
  document->SelectTileset(event.id);
}

void Application::OnRemoveTilesetEvent(const RemoveTilesetEvent& event)
{
  auto* document = mModel->GetActiveDocument();
  document->RemoveTileset(event.id);
}

void Application::OnAddRowEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->AddRow();
  }
}

void Application::OnAddColumnEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->AddColumn();
  }
}

void Application::OnRemoveRowEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->RemoveRow();
  }
}

void Application::OnRemoveColumnEvent()
{
  if (auto* document = mModel->GetActiveDocument())
  {
    document->RemoveColumn();
  }
}

void Application::OnSetPropertyValueEvent(const SetPropertyValueEvent& event)
{
  auto* document = mModel->GetActiveDocument();
  document->SetProperty(event.name, event.property);
}

void Application::OnSetTilesetSelectionEvent(
    const SetTilesetSelectionEvent& event)
{
  auto* document = mModel->GetActiveDocument();
  auto& tilesets = document->GetTilesets();
  tilesets.GetActiveTileset()->SetSelection(event.selection);
}

void Application::OnQuitEvent()
{
  mQuit = true;
}

}  // namespace Tactile
