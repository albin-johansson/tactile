#include "view_shortcuts.hpp"

#include "editor/events/view_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/widgets/focus.hpp"
#include "editor/model.hpp"
#include "io/preferences.hpp"
#include "mappings.hpp"

namespace Tactile {

CenterViewportShortcut::CenterViewportShortcut()
    : AShortcut{cen::scancodes::space, KMOD_LSHIFT}
{}

void CenterViewportShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CenterViewportEvent>();
}

auto CenterViewportShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

DecreaseViewportZoomShortcut::DecreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_MINUS}, primary_modifier}
{}

void DecreaseViewportZoomShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<DecreaseZoomEvent>();
}

auto DecreaseViewportZoomShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.CanDecreaseViewportTileSize();
}

IncreaseViewportZoomShortcut::IncreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_PLUS}, primary_modifier}
{}

void IncreaseViewportZoomShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<IncreaseZoomEvent>();
}

auto IncreaseViewportZoomShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

PanUpShortcut::PanUpShortcut()
    : AShortcut{cen::scancodes::up, primary_modifier | KMOD_LSHIFT}
{}

void PanUpShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanUpEvent>();
}

auto PanUpShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

PanDownShortcut::PanDownShortcut()
    : AShortcut{cen::scancodes::down, primary_modifier | KMOD_LSHIFT}
{}

void PanDownShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanDownEvent>();
}

auto PanDownShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

PanLeftShortcut::PanLeftShortcut()
    : AShortcut{cen::scancodes::left, primary_modifier | KMOD_LSHIFT}
{}

void PanLeftShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanLeftEvent>();
}

auto PanLeftShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

PanRightShortcut::PanRightShortcut()
    : AShortcut{cen::scancodes::right, primary_modifier | KMOD_LSHIFT}
{}

void PanRightShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanRightEvent>();
}

auto PanRightShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

ToggleGridShortcut::ToggleGridShortcut() : AShortcut{cen::scancodes::g, primary_modifier}
{}

void ToggleGridShortcut::Activate(entt::dispatcher&)
{
  Prefs::SetShowGrid(!Prefs::GetShowGrid());
}

ToggleUiShortcut::ToggleUiShortcut() : AShortcut{cen::scancodes::tab}
{}

void ToggleUiShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ToggleUiEvent>();
}

auto ToggleUiShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument() && IsEditorFocused();
}

}  // namespace Tactile