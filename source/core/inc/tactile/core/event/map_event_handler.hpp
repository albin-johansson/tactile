// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path
#include <optional>    // optional

#include "tactile/base/io/save/save_format_id.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class IRuntime;
class Model;
class EventDispatcher;

namespace ui {
class WidgetManager;
}  // namespace ui

struct ShowNewMapDialogEvent;
struct ShowOpenMapDialogEvent;
struct ShowGodotExportDialogEvent;
struct CreateMapEvent;
struct ExportAsGodotSceneEvent;

/**
 * Handles events related to maps.
 */
class MapEventHandler final
{
 public:
  /**
   * Creates a map event handler.
   *
   * \param model          The associated model, cannot be null.
   * \param widget_manager The associated widget manager, cannot be null.
   * \param runtime        The associated runtim, cannot be null.
   */
  MapEventHandler(Model* model, ui::WidgetManager* widget_manager, IRuntime* runtime);

  /**
   * Installs the event handler to a given event dispatcher.
   *
   * \param dispatcher The target event dispatcher.
   */
  void install(EventDispatcher& dispatcher);

  /**
   * Opens the map creation dialog.
   *
   * \param event The associated event.
   */
  void on_show_new_map_dialog(const ShowNewMapDialogEvent& event);

  /**
   * Opens the map selector dialog.
   *
   * \param event The associated event.
   */
  void on_show_open_map_dialog(const ShowOpenMapDialogEvent& event);

  void on_show_godot_export_dialog(const ShowGodotExportDialogEvent& event);

  /**
   * Creates a new map.
   *
   * \param event The associated event.
   */
  void on_create_map(const CreateMapEvent& event);

  void on_export_as_godot_scene(const ExportAsGodotSceneEvent& event) const;

 private:
  Model* mModel;
  ui::WidgetManager* mWidgetManager;
  IRuntime* mRuntime;

  [[nodiscard]]
  static auto _guess_save_format(const std::filesystem::path& path)
      -> std::optional<SaveFormatId>;
};

}  // namespace tactile
