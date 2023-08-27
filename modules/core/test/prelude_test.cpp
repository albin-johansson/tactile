// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/prelude.hpp"

#include <limits>       // numeric_limits
#include <type_traits>  // is_floating_point_v, is_signed_v, is_unsigned_v

#include <gtest/gtest.h>

using namespace tactile;

TEST(Prelude, VersionString)
{
  EXPECT_STREQ(TACTILE_VERSION_STRING, "0.5.0 [WIP]");
}

TEST(Prelude, FixedSizePrimitives)
{
  static_assert(sizeof(float32) == 4);
  static_assert(sizeof(float64) == 8);

  static_assert(std::is_floating_point_v<float32>);
  static_assert(std::is_floating_point_v<float64>);

  static_assert(sizeof(int8) == 1);
  static_assert(sizeof(int16) == 2);
  static_assert(sizeof(int32) == 4);
  static_assert(sizeof(int64) == 8);

  static_assert(sizeof(uint8) == 1);
  static_assert(sizeof(uint16) == 2);
  static_assert(sizeof(uint32) == 4);
  static_assert(sizeof(uint64) == 8);

  static_assert(std::is_signed_v<int8>);
  static_assert(std::is_signed_v<int16>);
  static_assert(std::is_signed_v<int32>);
  static_assert(std::is_signed_v<int64>);

  static_assert(std::is_unsigned_v<uint8>);
  static_assert(std::is_unsigned_v<uint16>);
  static_assert(std::is_unsigned_v<uint32>);
  static_assert(std::is_unsigned_v<uint64>);
}

TEST(Prelude, LiteralOperatorUZ)
{
  using tactile::int_literals::operator""_uz;

  static_assert(std::is_same_v<decltype(0_uz), usize>);
  static_assert(std::numeric_limits<usize>::max() == -1_uz);

  EXPECT_EQ(usize {723'843}, 723'843_uz);
}

TEST(Prelude, BuildModeDetection)
{
#ifdef NDEBUG
  EXPECT_EQ(kIsDebugBuild, false);
#else
  EXPECT_EQ(kIsDebugBuild, true);
#endif
}

TEST(Prelude, PlatformDetection)
{
  // Ensure that the detected platforms are mutually exclusive.
  const auto platform_sum = static_cast<int>(kIsWindows) +  //
                            static_cast<int>(kIsApple) +    //
                            static_cast<int>(kIsLinux);
  EXPECT_EQ(platform_sum, 1);
}

TEST(Prelude, CompilerDetection)
{
  // Ensure that the detected compilers are mutually exclusive.
  const auto compiler_sum = static_cast<int>(kIsMSVC) +   //
                            static_cast<int>(kIsClang) +  //
                            static_cast<int>(kIsGCC);
  EXPECT_EQ(compiler_sum, 1);
}

TEST(Prelude, EmptyTileConstant)
{
  EXPECT_EQ(kEmptyTile, 0);
}

TEST(Prelude, InterfaceClassMacro)
{
  class IDummy final {
   public:
    TACTILE_INTERFACE_CLASS(IDummy);
  };

  static_assert(std::is_default_constructible_v<IDummy>);
  static_assert(std::is_copy_constructible_v<IDummy>);
  static_assert(std::is_copy_assignable_v<IDummy>);
  static_assert(std::is_nothrow_move_constructible_v<IDummy>);
  static_assert(std::is_nothrow_move_assignable_v<IDummy>);
  static_assert(std::has_virtual_destructor_v<IDummy>);
}