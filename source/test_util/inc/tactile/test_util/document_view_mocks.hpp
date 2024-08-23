// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>  // optional
#include <variant>   // variant

#include <gmock/gmock.h>

#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/document/document_visitor.hpp"
#include "tactile/base/document/layer_view.hpp"
#include "tactile/base/document/map_view.hpp"
#include "tactile/base/document/meta_view.hpp"
#include "tactile/base/document/object_view.hpp"
#include "tactile/base/document/tile_view.hpp"
#include "tactile/base/document/tileset_view.hpp"
#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::test {

class MetaViewMock : public IMetaView
{
 public:
  MetaViewMock() = default;

  explicit MetaViewMock(ir::Metadata meta);

  MOCK_METHOD(StringView, get_name, (), (const, override));

  MOCK_METHOD((std::pair<const String&, const Attribute&>),
              get_property,
              (usize),
              (const, override));

  MOCK_METHOD(usize, property_count, (), (const, override));

 private:
  ir::Metadata mMeta {};
};

class ObjectViewMock : public IObjectView
{
 public:
  explicit ObjectViewMock(ir::Object object,
                          std::variant<const ILayerView*, const ITileView*> parent = {});

  MOCK_METHOD((std::expected<void, std::error_code>),
              accept,
              (IDocumentVisitor&),
              (const, override));

  MOCK_METHOD(const ILayerView*, get_parent_layer, (), (const, override));

  MOCK_METHOD(const ITileView*, get_parent_tile, (), (const, override));

  MOCK_METHOD(ObjectType, get_type, (), (const, override));

  MOCK_METHOD(ObjectID, get_id, (), (const, override));

  MOCK_METHOD(Float2, get_position, (), (const, override));

  MOCK_METHOD(Float2, get_size, (), (const, override));

  MOCK_METHOD(StringView, get_tag, (), (const, override));

  MOCK_METHOD(bool, is_visible, (), (const, override));

  MOCK_METHOD(const IMetaView&, get_meta, (), (const, override));

  [[nodiscard]]
  auto get_meta_view_mock() -> MetaViewMock&
  {
    return mMeta;
  }

 private:
  ir::Object mObject {};
  std::variant<const ILayerView*, const ITileView*> mParent {};
  testing::NiceMock<MetaViewMock> mMeta {};
};

class TileViewMock : public ITileView
{
 public:
  TileViewMock(const ITilesetView* parent_tileset, ir::Tile tile);

  MOCK_METHOD((std::expected<void, std::error_code>),
              accept,
              (IDocumentVisitor&),
              (const, override));

  MOCK_METHOD(const ITilesetView&, get_parent_tileset, (), (const, override));

  MOCK_METHOD(TileIndex, get_index, (), (const, override));

  MOCK_METHOD(usize, object_count, (), (const, override));

  MOCK_METHOD(usize, animation_frame_count, (), (const, override));

  MOCK_METHOD((std::pair<TileIndex, Milliseconds>),
              get_animation_frame,
              (usize),
              (const, override));

  MOCK_METHOD(const IMetaView&, get_meta, (), (const, override));

  [[nodiscard]]
  auto get_meta_view_mock() -> MetaViewMock&
  {
    return mMeta;
  }

 private:
  const ITilesetView* mParentTileset {};
  ir::Tile mTile {};
  testing::NiceMock<MetaViewMock> mMeta {};
};

class TilesetViewMock : public ITilesetView
{
 public:
  TilesetViewMock()
  {
    ON_CALL(*this, get_meta).WillByDefault(testing::ReturnRef(mMeta));
  }

  explicit TilesetViewMock(ir::TilesetRef tileset_ref);

  MOCK_METHOD((std::expected<void, std::error_code>),
              accept,
              (IDocumentVisitor&),
              (const, override));

  MOCK_METHOD(TileID, get_first_tile_id, (), (const, override));

  MOCK_METHOD(usize, tile_count, (), (const, override));

  MOCK_METHOD(usize, tile_definition_count, (), (const, override));

  MOCK_METHOD(usize, column_count, (), (const, override));

  MOCK_METHOD(Int2, get_tile_size, (), (const, override));

  MOCK_METHOD(Int2, get_image_size, (), (const, override));

  MOCK_METHOD(const Path&, get_image_path, (), (const, override));

  MOCK_METHOD(const IMetaView&, get_meta, (), (const, override));

  MOCK_METHOD(String, get_filename, (), (const, override));

  [[nodiscard]]
  auto get_meta_view_mock() -> MetaViewMock&
  {
    return mMeta;
  }

 private:
  ir::TilesetRef mTilesetRef {};
  testing::NiceMock<MetaViewMock> mMeta {};
};

class LayerViewMock : public ILayerView
{
 public:
  LayerViewMock(ir::Layer layer,
                const ir::TileFormat& tile_format,
                const LayerViewMock* parent_layer = nullptr);

  MOCK_METHOD((std::expected<void, std::error_code>),
              accept,
              (IDocumentVisitor&),
              (const, override));

  MOCK_METHOD(void, write_tile_bytes, (ByteStream&), (const, override));

  MOCK_METHOD(const ILayerView*, get_parent_layer, (), (const, override));

  MOCK_METHOD(LayerID, get_id, (), (const, override));

  MOCK_METHOD(LayerType, get_type, (), (const, override));

  MOCK_METHOD(float, get_opacity, (), (const, override));

  MOCK_METHOD(bool, is_visible, (), (const, override));

  MOCK_METHOD(usize, get_global_index, (), (const, override));

  MOCK_METHOD(usize, layer_count, (), (const, override));

  MOCK_METHOD(usize, object_count, (), (const, override));

  MOCK_METHOD(std::optional<TileID>, get_tile, (const MatrixIndex&), (const, override));

  MOCK_METHOD(TileEncoding, get_tile_encoding, (), (const, override));

  MOCK_METHOD(std::optional<CompressionFormat>, get_tile_compression, (), (const, override));

  MOCK_METHOD(std::optional<int>, get_compression_level, (), (const, override));

  MOCK_METHOD(std::optional<MatrixExtent>, get_extent, (), (const, override));

  MOCK_METHOD(const IMetaView&, get_meta, (), (const, override));

  [[nodiscard]]
  auto get_meta_view_mock() -> MetaViewMock&
  {
    return mMeta;
  }

 private:
  ir::Layer mLayer {};
  ir::TileFormat mTileFormat {};
  const LayerViewMock* mParentLayer {};
  testing::NiceMock<MetaViewMock> mMeta {};
};

class MapViewMock : public IMapView
{
 public:
  explicit MapViewMock(const ir::Map& map);

  MOCK_METHOD((std::expected<void, std::error_code>),
              accept,
              (IDocumentVisitor&),
              (const, override));

  MOCK_METHOD(const Path*, get_path, (), (const, override));

  MOCK_METHOD(Int2, get_tile_size, (), (const, override));

  MOCK_METHOD(MatrixExtent, get_extent, (), (const, override));

  MOCK_METHOD(LayerID, get_next_layer_id, (), (const, override));

  MOCK_METHOD(ObjectID, get_next_object_id, (), (const, override));

  MOCK_METHOD(TileEncoding, get_tile_encoding, (), (const, override));

  MOCK_METHOD(std::optional<CompressionFormat>, get_tile_compression, (), (const, override));

  MOCK_METHOD(std::optional<int>, get_compression_level, (), (const, override));

  MOCK_METHOD(usize, layer_count, (), (const, override));

  MOCK_METHOD(usize, tileset_count, (), (const, override));

  MOCK_METHOD(usize, component_count, (), (const, override));

  MOCK_METHOD(const IMetaView&, get_meta, (), (const, override));

  [[nodiscard]]
  auto get_meta_view_mock() -> MetaViewMock&
  {
    return mMeta;
  }

 private:
  testing::NiceMock<MetaViewMock> mMeta {};
  Vector<Unique<TilesetViewMock>> mTilesets {};
  Vector<Unique<LayerViewMock>> mLayers {};
};

}  // namespace tactile::test
