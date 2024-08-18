// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class Model;
class EventDispatcher;
class IRuntime;

struct SaveEvent;
struct SaveAsEvent;
struct ReopenLastClosedFileEvent;
struct ClearFileHistoryEvent;
struct CloseEvent;
struct QuitEvent;

/**
 * Handles events related to "file" actions.
 */
class FileEventHandler final
{
 public:
  /**
   * Creates an file event handler.
   *
   * \param model   The associated model, cannot be null.
   * \param runtime The associated runtime, cannot be null.
   */
  FileEventHandler(Model* model, IRuntime* runtime);

  /**
   * Installs the event handler to a given event dispatcher.
   *
   * \param dispatcher The target event dispatcher.
   */
  void install(EventDispatcher& dispatcher);

  /**
   * Saves the state of the active document to disk.
   *
   * \param event The associated event.
   */
  void on_save(const SaveEvent& event);

  /**
   * Saves the state of the active document to disk using another name.
   *
   * \param event The associated event.
   */
  void on_save_as(const SaveAsEvent& event);

  /**
   * Reopens the most recently closed document.
   *
   * \param event The associated event.
   */
  void on_reopen_last_closed_file(const ReopenLastClosedFileEvent& event);

  /**
   * Clears the current history of opened documents.
   *
   * \param event The associated event.
   */
  void on_clear_file_history(const ClearFileHistoryEvent& event);

  /**
   * Closes the currently active document.
   *
   * \param event The associated event.
   */
  void on_close(const CloseEvent& event);

  /**
   * Stops the program.
   *
   * \param event The associated event.
   */
  void on_quit(const QuitEvent& event);

 private:
  Model* mModel;
  IRuntime* mRuntime;
};

}  // namespace tactile
