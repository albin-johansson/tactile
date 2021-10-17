#pragma once

#include <filesystem>  // path

#include "common/map_id.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct AddRowEvent final
{};

struct AddColumnEvent final
{};

struct RemoveRowEvent final
{};

struct RemoveColumnEvent final
{};

struct AddMapEvent final
{
  int tile_width{};
  int tile_height{};
};

struct CloseMapEvent final
{
  MapID id;
};

struct OpenMapEvent final
{
  std::filesystem::path path;
};

struct SelectMapEvent final
{
  MapID id;
};

struct ShowMapPropertiesEvent final
{};

struct OpenResizeMapDialogEvent final
{};

struct ResizeMapEvent final
{
  int row_count;
  int col_count;
};

/// \} End of group events

}  // namespace Tactile
