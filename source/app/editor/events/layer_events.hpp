#pragma once

#include <string>  // string

#include "core/layer_type.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct add_layer_event final
{
  layer_type type{};
};

struct remove_layer_event final
{
  layer_id id{};
};

struct duplicate_layer_event final
{
  layer_id id{};
};

struct select_layer_event final
{
  layer_id id{};
};

struct move_layer_up_event final
{
  layer_id id{};
};

struct move_layer_down_event final
{
  layer_id id{};
};

struct set_layer_opacity_event final
{
  layer_id id{};
  float opacity{};
};

struct set_layer_visible_event final
{
  layer_id id{};
  bool visible{};
};

struct open_rename_layer_dialog_event final
{
  layer_id id{};
};

struct rename_layer_event final
{
  layer_id id{};
  std::string name;
};

/// \} End of group events

}  // namespace tactile
