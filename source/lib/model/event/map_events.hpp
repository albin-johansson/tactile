// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/tile_format.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

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
  Int2 tile_size {};
  uint64 row_count {};
  uint64 column_count {};
};

struct OpenMapEvent final
{
  Path path;
};

struct InspectMapEvent final
{};

struct OpenResizeMapDialogEvent final
{};

struct ResizeMapEvent final
{
  usize row_count {};
  usize col_count {};
};

struct FixTilesInMapEvent final
{};

/// Event emitted when the user wants to save a map as a Godot scene.
struct ExportAsGodotSceneEvent final
{
  Path root_dir;
  Path map_dir;
  Path image_dir;
  Path tileset_dir;
  usize polygon_points {};
};

struct SetTileFormatCompressionEvent final
{
  CompressionType compression {};
};

struct SetTileFormatEncodingEvent final
{
  TileEncoding encoding {};
};

struct SetZlibCompressionLevelEvent final
{
  int level {};
};

struct SetZstdCompressionLevelEvent final
{
  int level {};
};

}  // namespace tactile
