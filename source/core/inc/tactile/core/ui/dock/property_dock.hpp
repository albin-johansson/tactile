// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class IDocument;
class EventDispatcher;

namespace ui {

class Language;

/**
 * A dock widget that provides a view into the properties of the active context.
 */
class PropertyDock final
{
 public:
  /**
   * Pushes the property dock to the widget stack.
   *
   * \param language   The current language.
   * \param document   The currently active document.
   * \param dispatcher The event dispatcher to use.
   */
  void push(const Language& language, const IDocument& document, EventDispatcher& dispatcher);
};

}  // namespace ui
}  // namespace tactile
