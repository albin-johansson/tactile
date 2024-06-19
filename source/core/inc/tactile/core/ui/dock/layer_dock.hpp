// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class MapDocument;
class EventDispatcher;

namespace ui {

class Language;

/**
 * A dock widget that displays the layers present in a map document.
 */
class LayerDock final
{
 public:
  /**
   * Pushes the layer dock to the widget stack.
   *
   * \param language   The current language.
   * \param document   The currently active map document.
   * \param dispatcher The event dispatcher to use.
   */
  void update(const Language& language,
              const MapDocument& document,
              EventDispatcher& dispatcher);
};

}  // namespace ui
}  // namespace tactile
