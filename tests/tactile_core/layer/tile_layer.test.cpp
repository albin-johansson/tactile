// Copyright (C) 2025 Albin Johansson

#include <gmock/gmock.h>
#include <gtest/gtest.h>

import tactile.layer;

namespace tactile::tests {
namespace {

class TileLayerTest : public testing::Test
{
 protected:
  TileLayer m_layer {LayerID {1}};
};

}  // namespace
}  // namespace tactile::tests
