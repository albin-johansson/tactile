// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/ui/fonts.hpp"
#include "tactile/core/ui/theme.hpp"

namespace tactile::core {

/**
 * Event for resetting the dock widget layout.
 */
struct ResetLayoutEvent final
{};

/**
 * Event for toggling the visibility of dock widgets.
 */
struct ToggleUiEvent final
{};

/**
 * Event for toggling the visibility of the property dock widget.
 */
struct TogglePropertyDockEvent final
{};

/**
 * Event for toggling the visibility of the component dock widget.
 */
struct ToggleComponentDockEvent final
{};

/**
 * Event for toggling the visibility of the layer dock widget.
 */
struct ToggleLayerDockEvent final
{};

/**
 * Event for toggling the visibility of the tileset dock widget.
 */
struct ToggleTilesetDockEvent final
{};

/**
 * Event for toggling the visibility of the animation dock widget.
 */
struct ToggleAnimationDockEvent final
{};

/**
 * Event for toggling the visibility of the log dock widget.
 */
struct ToggleLogDockEvent final
{};

/**
 * Event for changing the active editor theme.
 */
struct SetThemeEvent final
{
  ui::Theme theme;
};

/**
 * Event for gradually increasing the editor font size.
 */
struct IncreaseFontSizeEvent final
{};

/**
 * Event for gradually decreasing the editor font size.
 */
struct DecreaseFontSizeEvent final
{};

/**
 * Event for resetting the editor font size.
 */
struct ResetFontSizeEvent final
{};

/**
 * Event for setting the editor font.
 */
struct SetFontEvent final
{
  /** The selected font. */
  ui::FontID font;
};

/**
 * Event for reloading the current fonts.
 */
struct ReloadFontsEvent final
{
  /** The current framebuffer scale (usually either 1 or 2). */
  float framebuffer_scale;
};

/**
 * Event for toggling the visibility of the viewport grid.
 */
struct ToggleGridEvent final
{};

/**
 * Event for toggling the highlighting of the active layer.
 */
struct ToggleLayerHighlightEvent final
{};

}  // namespace tactile::core
