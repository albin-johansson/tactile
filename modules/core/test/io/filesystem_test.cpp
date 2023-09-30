// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/filesystem.hpp"

#include <gtest/gtest.h>

#include "tactile/core/platform/environment.hpp"
#include "tactile/core/prelude.hpp"

using namespace tactile;
using tactile::fs_literals::operator""_path;

/// \tests tactile::fs_literals::operator""_path
TEST(Filesystem, PathLiteralOperator)
{
  EXPECT_EQ("x"_path, fs::Path {"x"});
  EXPECT_EQ("x.y"_path, fs::Path {"x.y"});
  EXPECT_EQ("x/y/z"_path, fs::Path {"x/y/z"});
}

/// \tests tactile::fs::use_forward_slashes
TEST(Filesystem, UseForwardSlashes)
{
  EXPECT_EQ(fs::use_forward_slashes(R"(~/a/b)"), "~/a/b");
  EXPECT_EQ(fs::use_forward_slashes(R"(C:\a\b\c.txt)"), "C:/a/b/c.txt");
}

/// \tests tactile::fs::use_short_home_prefix
TEST(Filesystem, UseShortHomePrefix)
{
  const fs::Path home = get_env(kIsWindows ? "USERPROFILE" : "HOME").value();

  EXPECT_EQ(fs::use_short_home_prefix(home), "~");
  EXPECT_EQ(fs::use_short_home_prefix(home.string() + "/foo/"), "~/foo/");
  EXPECT_EQ(fs::use_short_home_prefix(home.string() + "/foo/bar.txt"), "~/foo/bar.txt");
  EXPECT_EQ(fs::use_short_home_prefix(home.string() + '/'), "~/");

  EXPECT_FALSE(fs::use_short_home_prefix("/a/b").has_value());
  EXPECT_FALSE(fs::use_short_home_prefix("a/b/c").has_value());
  EXPECT_FALSE(fs::use_short_home_prefix("file.txt").has_value());
}

/// \tests tactile::fs::has_home_prefix
TEST(Filesystem, HasHomePrefix)
{
  const fs::Path home = get_env(kIsWindows ? "USERPROFILE" : "HOME").value();

  EXPECT_FALSE(fs::has_home_prefix(""));
  EXPECT_FALSE(fs::has_home_prefix("abc.xyz"));
  EXPECT_FALSE(fs::has_home_prefix("a/b.c"));
  EXPECT_FALSE(fs::has_home_prefix("a/b/c"));
  EXPECT_FALSE(fs::has_home_prefix("/a/b"));

  EXPECT_TRUE(fs::has_home_prefix(home));
  EXPECT_TRUE(fs::has_home_prefix(home / "a"));
  EXPECT_TRUE(fs::has_home_prefix(home / "a.b"));
  EXPECT_TRUE(fs::has_home_prefix(home / "a" / "b.c"));
  EXPECT_TRUE(fs::has_home_prefix(home.string() + "/"));
}
