// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/filesystem.hpp"

#include <gtest/gtest.h>

#include "tactile/foundation/platform/environment.hpp"
#include "tactile/foundation/prelude.hpp"

using namespace tactile;
using tactile::fs_literals::operator""_path;

/// \tests tactile::fs_literals::operator""_path
TEST(Filesystem, PathLiteralOperator)
{
  EXPECT_EQ("x"_path, FilePath {"x"});
  EXPECT_EQ("x.y"_path, FilePath {"x.y"});
  EXPECT_EQ("x/y/z"_path, FilePath {"x/y/z"});
}

/// \tests tactile::get_persistent_storage_directory
TEST(Filesystem, GetPersistentStorageDirectory)
{
  EXPECT_NO_THROW((void) get_persistent_storage_directory());
}


/// \tests tactile::normalize_path
TEST(Filesystem, NormalizePath)
{
  EXPECT_EQ(normalize_path(R"(~/a/b)"), "~/a/b");
  EXPECT_EQ(normalize_path(R"(C:\a\b\c.txt)"), "C:/a/b/c.txt");
}

/// \tests tactile::shorten_home_directory_prefix
TEST(Filesystem, ShortenHomeDirectoryPrefix)
{
  const FilePath home = get_env(kIsWindows ? "USERPROFILE" : "HOME").value();

  EXPECT_EQ(shorten_home_directory_prefix(home), "~");
  EXPECT_EQ(shorten_home_directory_prefix(home.string() + "/foo/"), "~/foo/");
  EXPECT_EQ(shorten_home_directory_prefix(home.string() + "/foo/bar.txt"), "~/foo/bar.txt");
  EXPECT_EQ(shorten_home_directory_prefix(home.string() + '/'), "~/");

  EXPECT_FALSE(shorten_home_directory_prefix("/a/b").has_value());
  EXPECT_FALSE(shorten_home_directory_prefix("a/b/c").has_value());
  EXPECT_FALSE(shorten_home_directory_prefix("file.txt").has_value());
}

/// \tests tactile::has_home_directory_prefix
TEST(Filesystem, HasHomeDirectoryPrefix)
{
  const FilePath home = get_env(kIsWindows ? "USERPROFILE" : "HOME").value();

  EXPECT_FALSE(has_home_directory_prefix(""));
  EXPECT_FALSE(has_home_directory_prefix("abc.xyz"));
  EXPECT_FALSE(has_home_directory_prefix("a/b.c"));
  EXPECT_FALSE(has_home_directory_prefix("a/b/c"));
  EXPECT_FALSE(has_home_directory_prefix("/a/b"));

  EXPECT_TRUE(has_home_directory_prefix(home));
  EXPECT_TRUE(has_home_directory_prefix(home / "a"));
  EXPECT_TRUE(has_home_directory_prefix(home / "a.b"));
  EXPECT_TRUE(has_home_directory_prefix(home / "a" / "b.c"));
  EXPECT_TRUE(has_home_directory_prefix(home.string() + "/"));
}
