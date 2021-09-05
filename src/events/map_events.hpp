#pragma once

#include <filesystem>  // path

#include "aliases/map_id.hpp"

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

/// \} End of group events

}  // namespace Tactile
