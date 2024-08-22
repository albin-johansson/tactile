// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>  // optional

#include <gtest/gtest.h>

#include "tactile/base/prelude.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::test {

class ObjectCommandTest : public testing::Test
{
 public:
  void SetUp() override;

  [[nodiscard]]
  auto make_test_object() -> EntityID;

 protected:
  std::optional<MapDocument> m_document;
  EntityID m_map_id {kInvalidEntity};
  EntityID m_layer_id {kInvalidEntity};
};

}  // namespace tactile::test
