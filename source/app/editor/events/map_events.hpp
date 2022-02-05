#pragma once

#include <filesystem>  // path

#include "tactile.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct show_new_map_dialog_event final
{};

struct show_open_map_dialog_event final
{};

struct add_row_event final
{};

struct add_column_event final
{};

struct remove_row_event final
{};

struct remove_column_event final
{};

struct create_map_event final
{
  int32 tile_width{};
  int32 tile_height{};
  usize row_count{};
  usize column_count{};
};

struct close_map_event final
{
  map_id id{};
};

struct open_map_event final
{
  std::filesystem::path path;
};

struct select_map_event final
{
  map_id id;
};

struct inspect_map_event final
{};

struct open_resize_map_dialog_event final
{};

struct resize_map_event final
{
  usize row_count{};
  usize col_count{};
};

/// \} End of group events

}  // namespace tactile
