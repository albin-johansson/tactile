#include "core/common/functional.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(InvokeN, ZeroInvocations)
{
  bool called = false;
  invoke_n(0, [&] { called = true; });
  ASSERT_FALSE(called);
}

TEST(InvokeN, OneInvocation)
{
  int calls = 0;
  invoke_n(1, [&] { ++calls; });
  ASSERT_EQ(1, calls);
}

TEST(InvokeN, SeveralInvocations)
{
  int calls = 0;
  invoke_n(42, [&] { ++calls; });
  ASSERT_EQ(42, calls);
}

}  // namespace tactile::test
