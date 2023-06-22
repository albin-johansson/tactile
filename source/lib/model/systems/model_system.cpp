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

#include "model/components/document.hpp"
#include "model/components/file_history.hpp"
#include "model/components/language.hpp"
#include "model/components/menu.hpp"
#include "model/components/tool.hpp"
#include "model/settings.hpp"
#include "model/textures/gl_texture_system.hpp"
#include "model/textures/null_texture_system.hpp"
#include "model/textures/texture_components.hpp"
#include "model/tools/bucket_tool.hpp"
#include "model/tools/stamp_tool.hpp"
#include "ui/widget_state.hpp"

namespace tactile::sys {

void init_model(Model& model, const BackendAPI api)
{
  auto& document_context = model.add<DocumentContext>();
  document_context.active_document = kNullEntity;

  model.add<Settings>();
  model.add<Languages>();
  model.add<FileHistory>();
  model.add<TextureCache>();
  model.add<Icons>();
  model.add<MenuItems>();

  model.add<ui::WidgetState>();

  auto& texture_callbacks = model.add<TextureCallbacks>();
  if (api == BackendAPI::Null) {
    texture_callbacks.init = &on_init_null_texture;
    texture_callbacks.destroy = &on_destroy_null_texture;
  }
  else if (api == BackendAPI::OpenGL) {
    texture_callbacks.init = &on_init_gl_texture;
    texture_callbacks.destroy = &on_destroy_gl_texture;
  }

  auto& tool_context = model.add<ToolContext>();
  tool_context.tools[ToolType::Stamp] = create_stamp_tool(model);
  tool_context.tools[ToolType::Bucket] = create_bucket_tool(model);
}

}  // namespace tactile::sys
