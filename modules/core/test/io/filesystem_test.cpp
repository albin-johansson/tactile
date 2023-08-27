// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/filesystem.hpp"

#include <gtest/gtest.h>

#include "tactile/core/platform/environment.hpp"
#include "tactile/core/prelude.hpp"

using namespace tactile;
using tactile::fs_literals::operator""_path;

TEST(Filesystem, PathLiteralOperator)
{
  EXPECT_EQ("x"_path, Path {"x"});
  EXPECT_EQ("x.y"_path, Path {"x.y"});
  EXPECT_EQ("x/y/z"_path, Path {"x/y/z"});
}

TEST(Filesystem, UseForwardSlashes)
{
  EXPECT_EQ(use_forward_slashes(R"(~/a/b)"), "~/a/b");
  EXPECT_EQ(use_forward_slashes(R"(C:\a\b\c.txt)"), "C:/a/b/c.txt");
}

TEST(Filesystem, UseShortHomePrefix)
{
  const Path home = get_env(kIsWindows ? "USERPROFILE" : "HOME").value();

  EXPECT_EQ(use_short_home_prefix(home), "~");
  EXPECT_EQ(use_short_home_prefix(home / "foo/"), "~/foo/");
  EXPECT_EQ(use_short_home_prefix(home / "foo" / "bar.txt"), "~/foo/bar.txt");
  EXPECT_EQ(use_short_home_prefix(home.string() + '/'), "~/");

  EXPECT_FALSE(use_short_home_prefix("/a/b").has_value());
  EXPECT_FALSE(use_short_home_prefix("a/b/c").has_value());
  EXPECT_FALSE(use_short_home_prefix("file.txt").has_value());
}

TEST(Filesystem, HasHomePrefix)
{
  const Path home = get_env(kIsWindows ? "USERPROFILE" : "HOME").value();

  EXPECT_FALSE(has_home_prefix(""));
  EXPECT_FALSE(has_home_prefix("abc.xyz"));
  EXPECT_FALSE(has_home_prefix("a/b.c"));
  EXPECT_FALSE(has_home_prefix("a/b/c"));
  EXPECT_FALSE(has_home_prefix("/a/b"));

  EXPECT_TRUE(has_home_prefix(home));
  EXPECT_TRUE(has_home_prefix(home / "a"));
  EXPECT_TRUE(has_home_prefix(home / "a.b"));
  EXPECT_TRUE(has_home_prefix(home / "a" / "b.c"));
  EXPECT_TRUE(has_home_prefix(home.string() + "/"));
}
