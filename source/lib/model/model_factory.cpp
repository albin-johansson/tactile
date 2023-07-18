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

#include "model_factory.hpp"

#include "model/documents/document_components.hpp"
#include "model/i18n/language_components.hpp"
#include "model/persistence/file_history_components.hpp"
#include "model/persistence/settings.hpp"
#include "model/textures/texture_components.hpp"
#include "model/view/menu_components.hpp"
#include "ui/widget_state.hpp"

namespace tactile::sys {

void init_model(Registry& registry, const BackendAPI api)
{
  auto& document_context = registry.add<DocumentContext>();
  document_context.active_document = kNullEntity;

  registry.add<Settings>();
  registry.add<Languages>();
  registry.add<FileHistory>();
  registry.add<Icons>();
  registry.add<MenuItems>();

  registry.add<ui::WidgetState>();
}

auto create_model(const BackendAPI api) -> Registry
{
  Registry model;
  init_model(model, api);
  return model;
}

}  // namespace tactile::sys
