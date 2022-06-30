/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

/* Convenience header that includes all other command headers */

#include "core/commands/components/attach_component_cmd.hpp"
#include "core/commands/components/create_component_attr_cmd.hpp"
#include "core/commands/components/create_component_def_cmd.hpp"
#include "core/commands/components/duplicate_component_attr_cmd.hpp"
#include "core/commands/components/remove_component_attr_cmd.hpp"
#include "core/commands/components/remove_component_cmd.hpp"
#include "core/commands/components/remove_component_def_cmd.hpp"
#include "core/commands/components/rename_component_attr_cmd.hpp"
#include "core/commands/components/rename_component_cmd.hpp"
#include "core/commands/components/reset_component_cmd.hpp"
#include "core/commands/components/set_component_attr_type_cmd.hpp"
#include "core/commands/components/update_component_attr_cmd.hpp"
#include "core/commands/components/update_component_cmd.hpp"
#include "core/commands/layers/add_layer_cmd.hpp"
#include "core/commands/layers/duplicate_layer_cmd.hpp"
#include "core/commands/layers/move_layer_down_cmd.hpp"
#include "core/commands/layers/move_layer_up_cmd.hpp"
#include "core/commands/layers/remove_layer_cmd.hpp"
#include "core/commands/layers/rename_layer_cmd.hpp"
#include "core/commands/layers/set_layer_opacity_cmd.hpp"
#include "core/commands/layers/set_layer_visibility_cmd.hpp"
#include "core/commands/maps/add_column_cmd.hpp"
#include "core/commands/maps/add_row_cmd.hpp"
#include "core/commands/maps/fix_tiles_in_map_cmd.hpp"
#include "core/commands/maps/remove_column_cmd.hpp"
#include "core/commands/maps/remove_row_cmd.hpp"
#include "core/commands/maps/resize_map_cmd.hpp"
#include "core/commands/objects/add_object_cmd.hpp"
#include "core/commands/objects/move_object_cmd.hpp"
#include "core/commands/objects/set_object_name_cmd.hpp"
#include "core/commands/objects/set_object_tag_cmd.hpp"
#include "core/commands/objects/set_object_visible_cmd.hpp"
#include "core/commands/properties/add_property_cmd.hpp"
#include "core/commands/properties/change_property_type_cmd.hpp"
#include "core/commands/properties/remove_property_cmd.hpp"
#include "core/commands/properties/rename_property_cmd.hpp"
#include "core/commands/properties/update_property_cmd.hpp"
#include "core/commands/tilesets/add_tileset_cmd.hpp"
#include "core/commands/tilesets/remove_tileset_cmd.hpp"
#include "core/commands/tilesets/rename_tileset_cmd.hpp"
#include "core/commands/tools/bucket_tool_cmd.hpp"
#include "core/commands/tools/eraser_tool_cmd.hpp"
#include "core/commands/tools/stamp_tool_cmd.hpp"