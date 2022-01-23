#include "view_shortcuts.hpp"

#include "editor/events/view_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "io/preferences.hpp"
#include "mappings.hpp"

namespace tactile {

CenterViewportShortcut::CenterViewportShortcut()
    : AShortcut{cen::scancodes::space, KMOD_LSHIFT}
{}

void CenterViewportShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CenterViewportEvent>();
}

auto CenterViewportShortcut::IsEnabled(const Model& model, const WidgetManager&) const
    -> bool
{
  return model.HasActiveDocument();
}

DecreaseViewportZoomShortcut::DecreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_MINUS}, gPrimaryModifier}
{}

void DecreaseViewportZoomShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<DecreaseZoomEvent>();
}

auto DecreaseViewportZoomShortcut::IsEnabled(const Model& model,
                                             const WidgetManager&) const -> bool
{
  return model.CanDecreaseViewportTileSize();
}

IncreaseViewportZoomShortcut::IncreaseViewportZoomShortcut()
    : AShortcut{cen::scan_code{SDLK_PLUS}, gPrimaryModifier}
{}

void IncreaseViewportZoomShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<IncreaseZoomEvent>();
}

auto IncreaseViewportZoomShortcut::IsEnabled(const Model& model,
                                             const WidgetManager&) const -> bool
{
  return model.HasActiveDocument();
}

PanUpShortcut::PanUpShortcut()
    : AShortcut{cen::scancodes::up, gPrimaryModifier | KMOD_LSHIFT}
{}

void PanUpShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanUpEvent>();
}

auto PanUpShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.HasActiveDocument();
}

PanDownShortcut::PanDownShortcut()
    : AShortcut{cen::scancodes::down, gPrimaryModifier | KMOD_LSHIFT}
{}

void PanDownShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanDownEvent>();
}

auto PanDownShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.HasActiveDocument();
}

PanLeftShortcut::PanLeftShortcut()
    : AShortcut{cen::scancodes::left, gPrimaryModifier | KMOD_LSHIFT}
{}

void PanLeftShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanLeftEvent>();
}

auto PanLeftShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.HasActiveDocument();
}

PanRightShortcut::PanRightShortcut()
    : AShortcut{cen::scancodes::right, gPrimaryModifier | KMOD_LSHIFT}
{}

void PanRightShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanRightEvent>();
}

auto PanRightShortcut::IsEnabled(const Model& model, const WidgetManager&) const -> bool
{
  return model.HasActiveDocument();
}

ToggleGridShortcut::ToggleGridShortcut() : AShortcut{cen::scancodes::g, gPrimaryModifier}
{}

void ToggleGridShortcut::Activate(entt::dispatcher&)
{
  prefs::SetShowGrid(!prefs::GetShowGrid());
}

ToggleUiShortcut::ToggleUiShortcut() : AShortcut{cen::scancodes::tab} {}

void ToggleUiShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ToggleUiEvent>();
}

auto ToggleUiShortcut::IsEnabled(const Model& model, const WidgetManager& widgets) const
    -> bool
{
  return model.HasActiveDocument() && widgets.IsEditorFocused();
}

}  // namespace tactile