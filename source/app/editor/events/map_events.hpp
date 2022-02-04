#pragma once

#include <filesystem>  // path

#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct ShowNewMapDialogEvent final
{};

struct ShowOpenMapDialogEvent final
{};

struct AddRowEvent final
{};

struct AddColumnEvent final
{};

struct RemoveRowEvent final
{};

struct RemoveColumnEvent final
{};

struct CreateMapEvent final
{
  int32 tile_width{};
  int32 tile_height{};
  usize row_count{};
  usize column_count{};
};

struct CloseMapEvent final
{
  map_id id;
};

struct OpenMapEvent final
{
  std::filesystem::path path;
};

struct SelectMapEvent final
{
  map_id id;
};

struct ShowMapPropertiesEvent final
{};

struct OpenResizeMapDialogEvent final
{};

struct ResizeMapEvent final
{
  usize row_count{};
  usize col_count{};
};

/// \} End of group events

}  // namespace tactile
