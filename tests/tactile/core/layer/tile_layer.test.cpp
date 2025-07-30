// Copyright (C) 2025 Albin Johansson

#include <gmock/gmock.h>
#include <gtest/gtest.h>

import tactile.core.layer;

namespace tactile::tests {
namespace {

class TileLayerTest : public testing::Test
{
 protected:
  TileLayer m_layer {LayerID {1}};
};

// TODO

}  // namespace
}  // namespace tactile::tests
