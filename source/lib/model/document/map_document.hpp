// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <boost/uuid/uuid_hash.hpp>

#include "common/type/tile_cache.hpp"
#include "core/context/context_manager.hpp"
#include "core/layer/layer_type.hpp"
#include "core/layer/tile_format.hpp"
#include "core/map.hpp"
#include "core/tile/tile_extent.hpp"
#include "core/viewport.hpp"
#include "model/document/document.hpp"
#include "model/document/document_delegate.hpp"
#include "model/tool/tool_manager.hpp"
#include "model/tool/tool_type.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Represents a loaded map.
class MapDocument final : public Document
{
 public:
  MapDocument(const Int2& tile_size, TileExtent extent);

  void update() override;

  /// Executes a command that adds a row to the associated map.
  void add_row();

  /// Executes a command that adds a column to the associated map.
  void add_column();

  /// Executes a command that removes a row from the associated map.
  void remove_row();

  /// Executes a command that removes a column from the associated map.
  void remove_column();

  /// Executes a command that resizes the associated map.
  void resize(TileExtent extent);

  /// Executes a command that fixes all invalid tiles in the associated map.
  void fix_tiles();

  /// Executes a command that adds an empty layer to the associated map.
  void add_layer(LayerType type);

  /// Executes a command that removes a layer from the associated map.
  void remove_layer(const UUID& layer_id);

  /// Executes a command that duplicates a layer in the associated map.
  void duplicate_layer(const UUID& layer_id);

  /// Executes a command that renames a layer in the associated map.
  void rename_layer(const UUID& layer_id, String name);

  /// Executes a command that moves a layer up in the associated map.
  void move_layer_up(const UUID& layer_id);

  /// Executes a command that moves a layer down in the associated map.
  void move_layer_down(const UUID& layer_id);

  /// Executes a command that sets the opacity of a layer in the associated map.
  void set_layer_opacity(const UUID& layer_id, float opacity);

  /// Executes a command that sets the visibility of a layer in the associated map.
  void set_layer_visible(const UUID& layer_id, bool visible);

  /// Registers a command for a stamp sequence.
  void register_stamp_sequence(const UUID& layer_id,
                               TileCache previous,
                               TileCache sequence);

  /// Registers a command for an eraser sequence.
  void register_eraser_sequence(const UUID& layer_id, TileCache previous);

  /// Executes a command that performs a flood fill in a layer in the associated map.
  void flood(const UUID& layer_id, const TilePos& origin, TileID replacement);

  /// Executes a command that adds a rectangle to a layer in the associated map.
  void add_rectangle(const UUID& layer_id, const Float2& pos, const Float2& size);

  /// Executes a command that adds an ellipse to a layer in the associated map.
  void add_ellipse(const UUID& layer_id, const Float2& pos, const Float2& size);

  /// Executes a command that adds a point to a layer in the associated map.
  void add_point(const UUID& layer_id, const Float2& pos);

  /// Executes a command that removes an object from a layer in the associated map.
  void remove_object(const UUID& object_id);

  /// Executes a command that moves an object in the associated map.
  void move_object(const UUID& object_id, const Float2& previous, const Float2& updated);

  /// Executes a command that sets the visibility of an object in the associated map.
  void set_object_visible(const UUID& object_id, bool visible);

  /// Executes a command that sets the name of an object in the associated map.
  void set_object_name(const UUID& object_id, String name);

  /// Executes a command that sets the tag of an object in the associated map.
  void set_object_tag(const UUID& object_id, String tag);

  /// Attempts to lookup an object in any of the object layers.
  [[nodiscard]] auto get_object(const UUID& object_id) -> Shared<Object>;

  /// Executes a command that sets the tile format encoding.
  void set_tile_format_encoding(TileEncoding encoding);

  /// Executes a command that sets the tile format compression strategy.
  void set_tile_format_compression(CompressionType compression);

  /// Executes a command that sets the Zlib compression level.
  void set_zlib_compression_level(int level);

  /// Executes a command that sets the ZStd compression level.
  void set_zstd_compression_level(int level);

  void set_component_index(Shared<ComponentIndex> index) override;

  void set_name(String name) override;

  void set_path(Path path) override;

  [[nodiscard]] auto has_path() const -> bool override;

  [[nodiscard]] auto get_component_index_ptr() -> Shared<ComponentIndex> override;
  [[nodiscard]] auto find_component_index() const -> const ComponentIndex* override;

  [[nodiscard]] auto get_tools() -> ToolManager&;
  [[nodiscard]] auto get_tools() const -> const ToolManager&;

  [[nodiscard]] auto get_contexts() -> ContextManager& override;
  [[nodiscard]] auto get_contexts() const -> const ContextManager& override;

  [[nodiscard]] auto get_history() -> CommandStack& override;
  [[nodiscard]] auto get_history() const -> const CommandStack& override;

  [[nodiscard]] auto get_viewport() -> Viewport& override;
  [[nodiscard]] auto get_viewport() const -> const Viewport& override;

  [[nodiscard]] auto get_name() const -> const String& override;

  [[nodiscard]] auto get_path() const -> const Path& override;

  [[nodiscard]] auto get_type() const -> DocumentType override;

  [[nodiscard]] auto get_map() -> Map& { return *mMap; }
  [[nodiscard]] auto get_map() const -> const Map& { return *mMap; }

  [[nodiscard]] auto get_map_ptr() -> const Shared<Map>& { return mMap; }

 private:
  Shared<Map> mMap;
  ToolManager mTools;
  DocumentDelegate mDelegate;
};

}  // namespace tactile
