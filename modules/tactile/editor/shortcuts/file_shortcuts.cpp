#include "file_shortcuts.hpp"

#include "editor/events/save_events.hpp"
#include "editor/gui/widgets/menus/edit_menu.hpp"
#include "editor/gui/widgets/menus/file_menu.hpp"
#include "editor/model.hpp"
#include "mappings.hpp"

namespace Tactile {

NewMapShortcut::NewMapShortcut() : AShortcut{cen::scancodes::n, primary_modifier}
{}

void NewMapShortcut::Activate(entt::dispatcher&)
{
  ShowAddMapDialog();
}

OpenMapShortcut::OpenMapShortcut() : AShortcut{cen::scancodes::o, primary_modifier}
{}

void OpenMapShortcut::Activate(entt::dispatcher&)
{
  ShowOpenMapDialog();
}

OpenSettingsShortcut::OpenSettingsShortcut()
    : AShortcut{SDL_SCANCODE_COMMA, primary_modifier}
{}

void OpenSettingsShortcut::Activate(entt::dispatcher&)
{
  ShowSettingsDialog();
}

SaveShortcut::SaveShortcut() : AShortcut{cen::scancodes::s, primary_modifier}
{}

void SaveShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SaveEvent>();
}

auto SaveShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanSaveDocument();
}

SaveAsShortcut::SaveAsShortcut()
    : AShortcut{cen::scancodes::s, primary_modifier | KMOD_LSHIFT}
{}

void SaveAsShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<OpenSaveAsDialogEvent>();
}

auto SaveAsShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanSaveDocument();
}

}  // namespace Tactile
