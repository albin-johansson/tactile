// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/prelude.hpp"

#include <limits>       // numeric_limits
#include <type_traits>  // is_floating_point_v, is_signed_v, is_unsigned_v

#include <gtest/gtest.h>

using namespace tactile;

TEST(Prelude, VersionString)
{
  EXPECT_STREQ(TACTILE_VERSION_STRING, "0.5.0 [WIP]");
}

/// \tests tactile::float32
/// \tests tactile::float64
/// \tests tactile::int8
/// \tests tactile::int16
/// \tests tactile::int32
/// \tests tactile::int64
/// \tests tactile::uint8
/// \tests tactile::uint16
/// \tests tactile::uint32
/// \tests tactile::uint64
TEST(Prelude, FixedSizePrimitives)
{
  EXPECT_EQ(sizeof(float32), 4);
  EXPECT_EQ(sizeof(float64), 8);

  EXPECT_TRUE(std::is_floating_point_v<float32>);
  EXPECT_TRUE(std::is_floating_point_v<float64>);

  EXPECT_EQ(sizeof(int8), 1);
  EXPECT_EQ(sizeof(int16), 2);
  EXPECT_EQ(sizeof(int32), 4);
  EXPECT_EQ(sizeof(int64), 8);

  EXPECT_EQ(sizeof(uint8), 1);
  EXPECT_EQ(sizeof(uint16), 2);
  EXPECT_EQ(sizeof(uint32), 4);
  EXPECT_EQ(sizeof(uint64), 8);

  EXPECT_TRUE(std::is_signed_v<int8>);
  EXPECT_TRUE(std::is_signed_v<int16>);
  EXPECT_TRUE(std::is_signed_v<int32>);
  EXPECT_TRUE(std::is_signed_v<int64>);

  EXPECT_TRUE(std::is_unsigned_v<uint8>);
  EXPECT_TRUE(std::is_unsigned_v<uint16>);
  EXPECT_TRUE(std::is_unsigned_v<uint32>);
  EXPECT_TRUE(std::is_unsigned_v<uint64>);
}

/// \tests tactile::int_literals::operator""_uz
TEST(Prelude, LiteralOperatorUZ)
{
  using tactile::int_literals::operator""_uz;

  EXPECT_TRUE((std::is_same_v<decltype(0_uz), usize>) );
  EXPECT_EQ(std::numeric_limits<usize>::max(), ~0_uz);

  EXPECT_EQ(usize {723'843}, 723'843_uz);
}

/// \tests tactile::kIsDebugBuild
TEST(Prelude, BuildModeDetection)
{
#ifdef NDEBUG
  EXPECT_EQ(kIsDebugBuild, false);
#else
  EXPECT_EQ(kIsDebugBuild, true);
#endif
}

/// \tests tactile::kIsWindows
/// \tests tactile::kIsApple
/// \tests tactile::kIsLinux
TEST(Prelude, PlatformDetection)
{
  // Ensure that the detected platforms are mutually exclusive.
  const auto platform_sum = static_cast<int>(kIsWindows) +  //
                            static_cast<int>(kIsApple) +    //
                            static_cast<int>(kIsLinux);
  EXPECT_EQ(platform_sum, 1);
}

/// \tests tactile::kIsMSVC
/// \tests tactile::kIsClang
/// \tests tactile::kIsGCC
TEST(Prelude, CompilerDetection)
{
  // Ensure that the detected compilers are mutually exclusive.
  const auto compiler_sum = static_cast<int>(kIsMSVC) +   //
                            static_cast<int>(kIsClang) +  //
                            static_cast<int>(kIsGCC);
  EXPECT_EQ(compiler_sum, 1);
}

TEST(Prelude, InterfaceClassMacro)
{
  class Interface {
   public:
    TACTILE_INTERFACE_CLASS(Interface);
  };

  EXPECT_TRUE(std::has_virtual_destructor_v<Interface>);
  EXPECT_FALSE(std::is_default_constructible_v<Interface>);
  EXPECT_FALSE(std::is_copy_constructible_v<Interface>);
  EXPECT_FALSE(std::is_copy_assignable_v<Interface>);
  EXPECT_FALSE(std::is_nothrow_move_constructible_v<Interface>);
  EXPECT_FALSE(std::is_nothrow_move_assignable_v<Interface>);

  class Implementation final : public Interface {};

  EXPECT_TRUE(std::has_virtual_destructor_v<Implementation>);
  EXPECT_TRUE(std::is_default_constructible_v<Implementation>);
  EXPECT_TRUE(std::is_copy_constructible_v<Implementation>);
  EXPECT_TRUE(std::is_copy_assignable_v<Implementation>);
  EXPECT_TRUE(std::is_nothrow_move_constructible_v<Implementation>);
  EXPECT_TRUE(std::is_nothrow_move_assignable_v<Implementation>);
}
