// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <gmock/gmock.h>

#include "tactile/base/document/document_visitor.hpp"
#include "tactile/base/document/layer_view.hpp"
#include "tactile/base/document/map_view.hpp"
#include "tactile/base/document/meta_view.hpp"
#include "tactile/base/document/object_view.hpp"
#include "tactile/base/document/tile_view.hpp"
#include "tactile/base/document/tileset_view.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::test {

class MetaViewMock : public IMetaView
{
 public:
  MOCK_METHOD(StringView, get_name, (), (const, override));

  MOCK_METHOD((Pair<const String&, const Attribute&>),
              get_property,
              (usize),
              (const, override));

  MOCK_METHOD(usize, property_count, (), (const, override));
};

class ObjectViewMock : public IObjectView
{
 public:
  ObjectViewMock()
  {
    ON_CALL(*this, accept).WillByDefault([this](IDocumentVisitor& visitor) {
      visitor.visit(*this);
    });

    ON_CALL(*this, get_meta).WillByDefault(testing::ReturnRef(mMeta));
  }

  MOCK_METHOD(void, accept, (IDocumentVisitor&), (const, override));

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

  MOCK_METHOD(void, accept, (IDocumentVisitor&), (const, override));

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

  MOCK_METHOD(void, accept, (IDocumentVisitor&), (const, override));

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

  MOCK_METHOD(void, accept, (IDocumentVisitor&), (const, override));

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

  MOCK_METHOD(void, accept, (IDocumentVisitor&), (const, override));

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
