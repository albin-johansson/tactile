// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path

namespace tactile::core {
class Model;
class EventDispatcher;
}  // namespace tactile::core

namespace tactile::core::ui {

/**
 * A dialog for exporting maps as Godot engine scenes.
 */
class GodotExportDialog final
{
 public:
  /**
   * Pushes the dialog to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher that will be used.
   */
  void push(const Model& model, EventDispatcher& dispatcher);

  /**
   * Schedules the dialog to be opened.
   */
  void open();

 private:
  std::filesystem::path m_project_dir {};
  int m_version {};
  bool m_should_open {false};
};

}  // namespace tactile::core::ui
