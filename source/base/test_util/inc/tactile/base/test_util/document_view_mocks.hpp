// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // move

#include <gmock/gmock.h>

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

  explicit MetaViewMock(ir::Metadata meta)
    : mMeta {std::move(meta)}
  {
    ON_CALL(*this, get_name).WillByDefault([this] { return mMeta.name; });

    ON_CALL(*this, get_property)
        .WillByDefault([this](const usize index) -> Pair<const String&, const Attribute&> {
          const auto& property = mMeta.properties.at(index);
          return {property.name, property.value};
        });

    ON_CALL(*this, property_count).WillByDefault(testing::Return(mMeta.properties.size()));
  }

  MOCK_METHOD(StringView, get_name, (), (const, override));

  MOCK_METHOD((Pair<const String&, const Attribute&>),
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
  ObjectViewMock()
  {
    ON_CALL(*this, accept).WillByDefault([this](IDocumentVisitor& visitor) {
      visitor.visit(*this).value();
      return kOK;
    });

    ON_CALL(*this, get_meta).WillByDefault(testing::ReturnRef(mMeta));
  }

  MOCK_METHOD(Result<void>, accept, (IDocumentVisitor&), (const, override));

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
  testing::NiceMock<MetaViewMock> mMeta {};
};

class TileViewMock : public ITileView
{
 public:
  TileViewMock()
  {
    ON_CALL(*this, get_meta).WillByDefault(testing::ReturnRef(mMeta));
  }

  MOCK_METHOD(Result<void>, accept, (IDocumentVisitor&), (const, override));

  MOCK_METHOD(const ITilesetView&, get_parent_tileset, (), (const, override));

  MOCK_METHOD(TileIndex, get_index, (), (const, override));

  MOCK_METHOD(usize, object_count, (), (const, override));

  MOCK_METHOD(usize, animation_frame_count, (), (const, override));

  MOCK_METHOD((Pair<TileIndex, Milliseconds>),
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
  testing::NiceMock<MetaViewMock> mMeta {};
};

class TilesetViewMock : public ITilesetView
{
 public:
  TilesetViewMock()
  {
    ON_CALL(*this, get_meta).WillByDefault(testing::ReturnRef(mMeta));
  }

  MOCK_METHOD(Result<void>, accept, (IDocumentVisitor&), (const, override));

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
  testing::NiceMock<MetaViewMock> mMeta {};
};

class LayerViewMock : public ILayerView
{
 public:
  LayerViewMock()
  {
    ON_CALL(*this, get_meta).WillByDefault(testing::ReturnRef(mMeta));
  }

  MOCK_METHOD(Result<void>, accept, (IDocumentVisitor&), (const, override));

  MOCK_METHOD(void, write_tile_bytes, (ByteStream&), (const, override));

  MOCK_METHOD(const ILayerView*, get_parent_layer, (), (const, override));

  MOCK_METHOD(LayerID, get_id, (), (const, override));

  MOCK_METHOD(LayerType, get_type, (), (const, override));

  MOCK_METHOD(float, get_opacity, (), (const, override));

  MOCK_METHOD(bool, is_visible, (), (const, override));

  MOCK_METHOD(usize, get_global_index, (), (const, override));

  MOCK_METHOD(usize, layer_count, (), (const, override));

  MOCK_METHOD(usize, object_count, (), (const, override));

  MOCK_METHOD(Optional<TileID>, get_tile, (const MatrixIndex&), (const, override));

  MOCK_METHOD(TileEncoding, get_tile_encoding, (), (const, override));

  MOCK_METHOD(Optional<CompressionFormat>, get_tile_compression, (), (const, override));

  MOCK_METHOD(Optional<int>, get_compression_level, (), (const, override));

  MOCK_METHOD(Optional<MatrixExtent>, get_extent, (), (const, override));

  MOCK_METHOD(const IMetaView&, get_meta, (), (const, override));

  [[nodiscard]]
  auto get_meta_view_mock() -> MetaViewMock&
  {
    return mMeta;
  }

 private:
  testing::NiceMock<MetaViewMock> mMeta {};
};

class MapViewMock : public IMapView
{
 public:
  MapViewMock()
  {
    ON_CALL(*this, get_meta).WillByDefault(testing::ReturnRef(mMeta));
  }

  explicit MapViewMock(const ir::Map& ir_map)
    : mMeta {ir_map.meta}
  {
    using testing::Return;

    ON_CALL(*this, get_tile_size).WillByDefault(Return(ir_map.tile_size));
    ON_CALL(*this, get_extent).WillByDefault(Return(ir_map.extent));
    ON_CALL(*this, get_next_layer_id).WillByDefault(Return(ir_map.next_layer_id));
    ON_CALL(*this, get_next_object_id).WillByDefault(Return(ir_map.next_object_id));
    ON_CALL(*this, get_tile_encoding).WillByDefault(Return(ir_map.tile_format.encoding));
    ON_CALL(*this, get_tile_compression).WillByDefault(Return(ir_map.tile_format.compression));
    ON_CALL(*this, get_compression_level)
        .WillByDefault(Return(ir_map.tile_format.compression_level));
    ON_CALL(*this, layer_count).WillByDefault(Return(ir_map.layers.size()));
    ON_CALL(*this, tileset_count).WillByDefault(Return(ir_map.tilesets.size()));
    ON_CALL(*this, component_count).WillByDefault(Return(ir_map.components.size()));
    ON_CALL(*this, get_meta).WillByDefault(testing::ReturnRef(mMeta));
  }

  MOCK_METHOD(Result<void>, accept, (IDocumentVisitor&), (const, override));

  MOCK_METHOD(const Path*, get_path, (), (const, override));

  MOCK_METHOD(Int2, get_tile_size, (), (const, override));

  MOCK_METHOD(MatrixExtent, get_extent, (), (const, override));

  MOCK_METHOD(LayerID, get_next_layer_id, (), (const, override));

  MOCK_METHOD(ObjectID, get_next_object_id, (), (const, override));

  MOCK_METHOD(TileEncoding, get_tile_encoding, (), (const, override));

  MOCK_METHOD(Optional<CompressionFormat>, get_tile_compression, (), (const, override));

  MOCK_METHOD(Optional<int>, get_compression_level, (), (const, override));

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
};

}  // namespace tactile::test
