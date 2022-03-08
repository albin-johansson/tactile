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

#include "components/add_component_cmd.hpp"
#include "components/create_component_attr_cmd.hpp"
#include "components/create_component_def_cmd.hpp"
#include "components/duplicate_component_attr_cmd.hpp"
#include "components/remove_component_attr_cmd.hpp"
#include "components/remove_component_cmd.hpp"
#include "components/remove_component_def_cmd.hpp"
#include "components/rename_component_attr_cmd.hpp"
#include "components/rename_component_cmd.hpp"
#include "components/reset_component_cmd.hpp"
#include "components/set_component_attr_type_cmd.hpp"
#include "components/update_component_attr_cmd.hpp"
#include "components/update_component_cmd.hpp"
#include "layers/add_layer_cmd.hpp"
#include "layers/duplicate_layer_cmd.hpp"
#include "layers/move_layer_down_cmd.hpp"
#include "layers/move_layer_up_cmd.hpp"
#include "layers/remove_layer_cmd.hpp"
#include "layers/rename_layer_cmd.hpp"
#include "layers/set_layer_opacity_cmd.hpp"
#include "layers/set_layer_visibility_cmd.hpp"
#include "maps/add_column_cmd.hpp"
#include "maps/add_row_cmd.hpp"
#include "maps/remove_column_cmd.hpp"
#include "maps/remove_row_cmd.hpp"
#include "maps/resize_map_cmd.hpp"
#include "objects/move_object_cmd.hpp"
#include "objects/set_object_name_cmd.hpp"
#include "objects/set_object_tag_cmd.hpp"
#include "objects/set_object_visibility_cmd.hpp"
#include "properties/add_property_cmd.hpp"
#include "properties/change_property_type_cmd.hpp"
#include "properties/remove_property_cmd.hpp"
#include "properties/rename_property_cmd.hpp"
#include "properties/update_property_cmd.hpp"
#include "tilesets/add_tileset_cmd.hpp"
#include "tilesets/remove_tileset_cmd.hpp"
#include "tilesets/set_tileset_name_cmd.hpp"
#include "tools/add_rectangle_cmd.hpp"
#include "tools/bucket_cmd.hpp"
#include "tools/eraser_sequence_cmd.hpp"
#include "tools/point_tool_cmd.hpp"
#include "tools/stamp_sequence_cmd.hpp"