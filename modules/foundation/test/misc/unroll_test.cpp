// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/unroll.hpp"

#include <ostream>  // ostream

#include <gtest/gtest.h>

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"

using namespace tactile;

struct UnrollTestCfg final {
  int index;
  usize count;
};

inline auto operator<<(std::ostream& stream, const UnrollTestCfg& test_cfg)
    -> std::ostream&
{
  stream << "(index: " << test_cfg.index << ", count: " << test_cfg.count << ")";
  return stream;
}

class UnrollTest : public testing::TestWithParam<UnrollTestCfg> {};

INSTANTIATE_TEST_SUITE_P(Unroll,
                         UnrollTest,
                         testing::Values(UnrollTestCfg {.index = 0, .count = 0},
                                         UnrollTestCfg {.index = 0, .count = 1},
                                         UnrollTestCfg {.index = 0, .count = 2},
                                         UnrollTestCfg {.index = 0, .count = 3},
                                         UnrollTestCfg {.index = 0, .count = 4},
                                         UnrollTestCfg {.index = 0, .count = 5},
                                         UnrollTestCfg {.index = 0, .count = 6},
                                         UnrollTestCfg {.index = 0, .count = 7},
                                         UnrollTestCfg {.index = 0, .count = 8},
                                         UnrollTestCfg {.index = 0, .count = 9},
                                         UnrollTestCfg {.index = 0, .count = 89},
                                         UnrollTestCfg {.index = 0, .count = 128},
                                         UnrollTestCfg {.index = 1, .count = 61},
                                         UnrollTestCfg {.index = 28, .count = 99},
                                         UnrollTestCfg {.index = 1, .count = 1},
                                         UnrollTestCfg {.index = 2, .count = 5},
                                         UnrollTestCfg {.index = 3, .count = 7},
                                         UnrollTestCfg {.index = -10, .count = 10},
                                         UnrollTestCfg {.index = -100, .count = 42}));

/// \trace tactile::unroll4
TEST_P(UnrollTest, Unroll4)
{
  const auto& test_cfg = GetParam();

  Vector<int> indices {};
  indices.reserve(test_cfg.count);

  unroll4(test_cfg.index, as_signed(test_cfg.count), [&](const int index) {
    indices.push_back(index);
  });

  ASSERT_EQ(indices.size(), test_cfg.count);

  for (usize index = 0; index < test_cfg.count; ++index) {
    const auto expected_index = test_cfg.index + static_cast<int>(index);
    EXPECT_EQ(indices.at(index), expected_index);
  }
}

/// \trace tactile::unroll8
TEST_P(UnrollTest, Unroll8)
{
  const auto& test_cfg = GetParam();

  Vector<int> indices {};
  indices.reserve(test_cfg.count);

  unroll8(test_cfg.index, as_signed(test_cfg.count), [&](const int index) {
    indices.push_back(index);
  });

  ASSERT_EQ(indices.size(), test_cfg.count);

  for (usize index = 0; index < test_cfg.count; ++index) {
    const auto expected_index = test_cfg.index + static_cast<int>(index);
    EXPECT_EQ(indices.at(index), expected_index);
  }
}
