// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/event/map_event_handler.hpp"

#include <magic_enum.hpp>

#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/numeric/vec_format.hpp"
#include "tactile/base/runtime.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/event/dialog_events.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/map_events.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/platform/file_dialog.hpp"
#include "tactile/core/ui/widget_manager.hpp"

namespace tactile {

MapEventHandler::MapEventHandler(Model* model,
                                 ui::WidgetManager* widget_manager,
                                 IRuntime* runtime) :
  mModel {require_not_null(model, "null model")},
  mWidgetManager {require_not_null(widget_manager, "null widget manager")},
  mRuntime {require_not_null(runtime, "null runtime")}
{}

void MapEventHandler::install(EventDispatcher& dispatcher)
{
  TACTILE_LOG_DEBUG("Installing map event handler");

  using Self = MapEventHandler;
  dispatcher.bind<ShowNewMapDialogEvent, &Self::on_show_new_map_dialog>(this);
  dispatcher.bind<ShowOpenMapDialogEvent, &Self::on_show_open_map_dialog>(this);
  dispatcher.bind<CreateMapEvent, &Self::on_create_map>(this);
}

void MapEventHandler::on_show_new_map_dialog(const ShowNewMapDialogEvent&)
{
  TACTILE_LOG_TRACE("ShowNewMapDialogEvent");
  mWidgetManager->get_new_map_dialog().open();
}

void MapEventHandler::on_show_open_map_dialog(const ShowOpenMapDialogEvent&)
{
  TACTILE_LOG_TRACE("ShowOpenMapDialogEvent");

  const auto map_path = FileDialog::open_map();
  if (!map_path.has_value()) {
    return;
  }

  TACTILE_LOG_TRACE("Trying to load map {}", map_path->string());
  const auto format_id = _guess_save_format(*map_path);
  if (!format_id.has_value()) {
    TACTILE_LOG_ERROR("Unknown save format for extension '{}'",
                      map_path->extension().string());
    return;
  }

  const auto* save_format = mRuntime->get_save_format(*format_id);
  if (!save_format) {
    TACTILE_LOG_ERROR("Found no suitable installed save format");
    return;
  }

  // TODO
  const SaveFormatReadOptions read_options {
    .base_dir = map_path->parent_path(),
    .strict_mode = false,
  };

  const auto ir_map = save_format->load_map(*map_path, read_options);
  if (!ir_map.has_value()) {
    TACTILE_LOG_ERROR("Could not load map: {}", ir_map.error().message());
    return;
  }

  auto& document_manager = mModel->get_document_manager();

  const auto document_uuid =
      document_manager.create_and_open_map(*mRuntime->get_renderer(), *ir_map);
  if (!document_uuid.has_value()) {
    TACTILE_LOG_ERROR("Could not create map document: {}", document_uuid.error().message());
    return;
  }

  auto& document = document_manager.get_document(*document_uuid);
  document.set_format(*format_id);
}

void MapEventHandler::on_create_map(const CreateMapEvent& event)
{
  TACTILE_LOG_TRACE("CreateMapEvent(orientation: {}, size: {}, tile_size: {})",
                    magic_enum::enum_name(event.spec.orientation),
                    event.spec.extent,
                    event.spec.tile_size);

  auto& document_manager = mModel->get_document_manager();
  const auto document_uuid = document_manager.create_and_open_map(event.spec);
  if (document_uuid.has_value()) {
    TACTILE_LOG_DEBUG("Created map document (uuid: {})", *document_uuid);
  }
}

auto MapEventHandler::_guess_save_format(const std::filesystem::path& path)
    -> std::optional<SaveFormatId>
{
  const auto extension = path.extension();

  if (extension == ".yaml" || extension == ".yml") {
    return SaveFormatId::kTactileYaml;
  }

  if (extension == ".tmj" || extension == ".json") {
    return SaveFormatId::kTiledTmj;
  }

  if (extension == ".tmx" || extension == ".xml") {
    return SaveFormatId::kTiledTmx;
  }

  return std::nullopt;
}

}  // namespace tactile
