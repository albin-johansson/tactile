/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <utility>  // forward

#include "common/enum/menu_action.hpp"
#include "common/enum/tool_type.hpp"
#include "common/macros.hpp"
#include "common/type/deque.hpp"
#include "common/type/dispatcher.hpp"
#include "common/type/ecs.hpp"
#include "common/type/ref.hpp"
#include "common/type/string.hpp"
#include "common/type/vector.hpp"
#include "model/registry.hpp"
#include "model/services/i18n/strings.hpp"
#include "model/services/settings/settings.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(SystemManager)

class ModelView final {
 public:
  ModelView(const Registry& registry,
            const SystemManager& systems,
            Dispatcher& dispatcher);

  template <typename Event, typename... Args>
  void enqueue(Args&&... args)
  {
    get_dispatcher().enqueue<Event>(std::forward<Args>(args)...);
  }

  [[nodiscard]] auto get_settings() const -> const Settings&;

  [[nodiscard]] auto get_language_strings() const -> const Strings&;

  [[nodiscard]] auto get_file_history() const -> const Deque<String>&;

  [[nodiscard]] auto get_open_documents() const -> const Vector<Entity>&;

  [[nodiscard]] auto get_tactile_icon_texture() const -> void*;

  [[nodiscard]] auto has_active_document() const -> bool;

  [[nodiscard]] auto has_active_map_document() const -> bool;

  [[nodiscard]] auto has_active_tileset_document() const -> bool;

  [[nodiscard]] auto is_tile_layer_active() const -> bool;

  [[nodiscard]] auto is_document_active(Entity document_entity) const -> bool;

  [[nodiscard]] auto is_document_open(Entity document_entity) const -> bool;

  [[nodiscard]] auto is_map_document(Entity document_entity) const -> bool;

  [[nodiscard]] auto is_tileset_document(Entity document_entity) const -> bool;

  [[nodiscard]] auto get_document_name(Entity document_entity) const -> String;

  [[nodiscard]] auto is_available(MenuAction action) const -> bool;

  [[nodiscard]] auto get_registry() const -> const Registry& { return mRegistry.get(); }

  [[nodiscard]] auto get_dispatcher() -> Dispatcher& { return mDispatcher.get(); }

 private:
  Ref<const Registry> mRegistry;
  Ref<const SystemManager> mSystems;
  Ref<Dispatcher> mDispatcher;

  [[nodiscard]] auto _can_clear_file_history() const -> bool;
  [[nodiscard]] auto _can_reopen_last_closed_file() const -> bool;
  [[nodiscard]] auto _is_tool_available(ToolType type) const -> bool;
};

}  // namespace tactile
