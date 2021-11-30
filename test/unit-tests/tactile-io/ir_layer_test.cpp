#include <tactile_io.hpp>

#include <gtest/gtest.h>

using namespace Tactile;

TEST(IrLayer, Defaults)
{
  auto map = IO::CreateMap();
  auto& layer = IO::AddLayer(*map);

  ASSERT_STREQ("", IO::GetName(layer));

  ASSERT_EQ(0, IO::GetId(layer));
  ASSERT_EQ(0u, IO::GetIndex(layer));
  ASSERT_EQ(0u, IO::GetPropertyCount(layer));
  ASSERT_FLOAT_EQ(1.0f, IO::GetOpacity(layer));
  ASSERT_TRUE(IO::IsVisible(layer));

  ASSERT_EQ(LayerType::TileLayer, IO::GetType(layer));
  ASSERT_TRUE(IO::IsTileLayer(layer));
  ASSERT_FALSE(IO::IsObjectLayer(layer));
  ASSERT_FALSE(IO::IsGroupLayer(layer));
}

TEST(IrLayer, Usage)
{
  auto map = IO::CreateMap();
  auto& layer = IO::AddLayer(*map);

  IO::SetId(layer, 84);
  ASSERT_EQ(84, IO::GetId(layer));

  IO::SetIndex(layer, 7u);
  ASSERT_EQ(7u, IO::GetIndex(layer));

  IO::SetName(layer, "this-is-a-layer");
  ASSERT_STREQ("this-is-a-layer", IO::GetName(layer));

  IO::SetOpacity(layer, 0.8f);
  ASSERT_FLOAT_EQ(0.8f, IO::GetOpacity(layer));

  IO::SetVisible(layer, false);
  ASSERT_FALSE(IO::IsVisible(layer));
}

TEST(IrLayer, TileLayer)
{
  auto map = IO::CreateMap();
  auto& layer = IO::AddLayer(*map);
  auto& tileLayer = IO::GetTileLayer(layer);

  IO::ReserveTiles(tileLayer, 5u, 7u);
  ASSERT_EQ(5u, IO::GetRowCount(tileLayer));
  ASSERT_EQ(7u, IO::GetColumnCount(tileLayer));

  IO::SetTile(tileLayer, 3u, 2u, 56);
  ASSERT_EQ(56, IO::GetTile(tileLayer, 3u, 2u));
}

TEST(IrLayer, ObjectLayer)
{
  auto map = IO::CreateMap();
  auto& layer = IO::AddLayer(*map);
  auto& objectLayer = IO::MarkAsObjectLayer(layer);

  IO::ReserveObjects(objectLayer, 3u);
  ASSERT_EQ(0u, IO::GetObjectCount(objectLayer));

  auto& object [[maybe_unused]] = IO::AddObject(objectLayer);
  ASSERT_EQ(1u, IO::GetObjectCount(objectLayer));
}

TEST(IrLayer, GroupLayer)
{
  auto map = IO::CreateMap();
  auto& layer = IO::AddLayer(*map);
  auto& groupLayer = IO::MarkAsGroupLayer(layer);

  IO::ReserveLayers(groupLayer, 6u);
  ASSERT_EQ(0u, IO::GetLayerCount(groupLayer));

  auto& childLayer [[maybe_unused]] = IO::AddLayer(groupLayer);
  ASSERT_EQ(1u, IO::GetLayerCount(groupLayer));
  ASSERT_NO_THROW(IO::GetLayer(groupLayer, 0u));
}