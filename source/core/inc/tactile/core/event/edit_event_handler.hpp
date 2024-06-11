// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class Model;
class EventDispatcher;

struct UndoEvent;
struct RedoEvent;

/**
 * Handles events related to "edit" actions.
 */
class EditEventHandler
{
 public:
  /**
   * Creates an edit event handler.
   *
   * \param model The associated model, cannot be null.
   */
  explicit EditEventHandler(Model* model);

  /**
   * Installs the event handler to a given event dispatcher.
   *
   * \param dispatcher The target event dispatcher.
   */
  void install(EventDispatcher& dispatcher);

  /**
   * Undoes the most recent command in the active document.
   *
   * \param event The associated event.
   */
  void on_undo(const UndoEvent& event);

  /**
   * Reapplies the most recently reverted command in the active document.
   *
   * \param event The associated event.
   */
  void on_redo(const RedoEvent& event);

 private:
  Model* mModel;
};

}  // namespace tactile
