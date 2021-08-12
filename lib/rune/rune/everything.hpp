/*
 * MIT License
 * Copyright (c) 2021 Albin Johansson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef RUNE_EVERYTHING_HPP
#define RUNE_EVERYTHING_HPP

#include "aliases/font_id.hpp"
#include "aliases/integers.hpp"
#include "aliases/json_type.hpp"
#include "aliases/str.hpp"
#include "aliases/texture_id.hpp"
#include "aliases/texture_index.hpp"
#include "containers/aabb.hpp"
#include "containers/aabb_node.hpp"
#include "containers/aabb_tree.hpp"
#include "containers/stack_resource.hpp"
#include "containers/static_vector.hpp"
#include "containers/vector_map.hpp"
#include "core/compiler.hpp"
#include "core/engine.hpp"
#include "core/from_string.hpp"
#include "core/game.hpp"
#include "core/iterable.hpp"
#include "core/rune_error.hpp"
#include "core/semi_fixed_game_loop.hpp"
#include "ecs/ecs_utils.hpp"
#include "ecs/entity_type.hpp"
#include "ecs/events/button_pressed_event.hpp"
#include "ecs/events/key_bind_triggered_event.hpp"
#include "ecs/null_entity.hpp"
#include "ecs/ui/button_system.hpp"
#include "ecs/ui/key_bind_system.hpp"
#include "ecs/ui/label_system.hpp"
#include "ecs/ui/lazy_texture_system.hpp"
#include "ecs/ui/line_system.hpp"
#include "ecs/ui/menu_system.hpp"
#include "ecs/ui/ui_background.hpp"
#include "ecs/ui/ui_button.hpp"
#include "ecs/ui/ui_checkbox.hpp"
#include "ecs/ui/ui_foreground.hpp"
#include "ecs/ui/ui_grid.hpp"
#include "ecs/ui/ui_key_bind.hpp"
#include "ecs/ui/ui_label.hpp"
#include "ecs/ui/ui_lazy_texture.hpp"
#include "ecs/ui/ui_line.hpp"
#include "ecs/ui/ui_menu.hpp"
#include "ecs/ui/ui_position.hpp"
#include "ecs/ui/ui_system.hpp"
#include "io/ini.hpp"
#include "io/ini_value.hpp"
#include "io/json_utils.hpp"
#include "io/tmx/parse_tileset.hpp"
#include "io/tmx/parse_tmx.hpp"
#include "io/tmx/tmx_animation.hpp"
#include "io/tmx/tmx_color.hpp"
#include "io/tmx/tmx_data.hpp"
#include "io/tmx/tmx_global_id.hpp"
#include "io/tmx/tmx_grid.hpp"
#include "io/tmx/tmx_halign.hpp"
#include "io/tmx/tmx_image_layer.hpp"
#include "io/tmx/tmx_layer.hpp"
#include "io/tmx/tmx_layer_type.hpp"
#include "io/tmx/tmx_local_id.hpp"
#include "io/tmx/tmx_map.hpp"
#include "io/tmx/tmx_map_orientation.hpp"
#include "io/tmx/tmx_map_render_order.hpp"
#include "io/tmx/tmx_object.hpp"
#include "io/tmx/tmx_object_layer.hpp"
#include "io/tmx/tmx_point.hpp"
#include "io/tmx/tmx_property.hpp"
#include "io/tmx/tmx_stagger_axis.hpp"
#include "io/tmx/tmx_stagger_index.hpp"
#include "io/tmx/tmx_terrain.hpp"
#include "io/tmx/tmx_text.hpp"
#include "io/tmx/tmx_tile.hpp"
#include "io/tmx/tmx_tile_layer.hpp"
#include "io/tmx/tmx_tile_layer_compression.hpp"
#include "io/tmx/tmx_tile_layer_encoding.hpp"
#include "io/tmx/tmx_tile_offset.hpp"
#include "io/tmx/tmx_tileset.hpp"
#include "io/tmx/tmx_valign.hpp"
#include "io/tmx/tmx_wang_color.hpp"
#include "io/tmx/tmx_wang_set.hpp"
#include "io/tmx/tmx_wang_tile.hpp"
#include "math/almost_equal.hpp"
#include "math/index_to_matrix.hpp"
#include "math/max.hpp"
#include "math/min.hpp"
#include "math/random_utils.hpp"
#include "math/vector2.hpp"

#endif  // RUNE_EVERYTHING_HPP
