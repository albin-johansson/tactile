// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile::core {

class MapDocument;
class EventDispatcher;

namespace ui {

class Language;

/**
 * Represents the dock widget that displays the tilesets attached to a map.
 */
class TilesetDock final
{
 public:
  /**
   * Pushes the tileset dock to the widget stack.
   *
   * \param language   The current language.
   * \param document   The currently active map document.
   * \param dispatcher The event dispatcher to use.
   */
  void push(const Language& language,
            const MapDocument& document,
            EventDispatcher& dispatcher);
};

}  // namespace ui
}  // namespace tactile::core
