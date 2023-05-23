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

#include "model_system.hpp"

#include "components/document.hpp"
#include "components/texture.hpp"
#include "core/language.hpp"
#include "core/menu.hpp"
#include "model/settings.hpp"

namespace tactile::sys {

void init_model(Model& model)
{
  auto& document_context = model.add<DocumentContext>();
  document_context.active_document = kNullEntity;

  model.add<Settings>();
  model.add<Languages>();
  model.add<TextureCache>();
  model.add<Icons>();
  model.add<MenuItems>();
}

}  // namespace tactile::sys
