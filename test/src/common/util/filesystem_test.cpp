/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "common/util/filesystem.hpp"

#include <doctest/doctest.h>

#include "core/platform/environment.hpp"
#include "core/prelude.hpp"

using namespace tactile;

TEST_SUITE("FilesystemUtils")
{
  TEST_CASE("to_forward_slashes_path")
  {
    const fs::path source = R"(C:\foo\bar\abc.yaml)";
    CHECK(to_forward_slashes_path(source) == "C:/foo/bar/abc.yaml");
  }

  TEST_CASE("has_home_prefix")
  {
    const auto home = read_env_var(kOnWindows ? "USERPROFILE" : "HOME").value();

    CHECK(!has_home_prefix(""));
    CHECK(!has_home_prefix("foo.cpp"));
    CHECK(!has_home_prefix("foo/bar.yaml"));
    CHECK(!has_home_prefix("some/random/path"));

    CHECK(has_home_prefix(Path {home}));
    CHECK(has_home_prefix(Path {home + "/"}));
    CHECK(has_home_prefix(Path {home + "/foo"}));
    CHECK(has_home_prefix(Path {home + "/foo.txt"}));
    CHECK(has_home_prefix(Path {home + "/foo/bar.txt"}));
  }

  TEST_CASE("use_short_home_prefix")
  {
    const auto home = read_env_var(kOnWindows ? "USERPROFILE" : "HOME").value();

    CHECK(use_short_home_prefix(Path {home}) == "~");
    CHECK(use_short_home_prefix(Path {home + '/'}) == "~/");
    CHECK(use_short_home_prefix(Path {home + "/foo/"}) == "~/foo/");
    CHECK(use_short_home_prefix(Path {home + "/foo/bar.txt"}) == "~/foo/bar.txt");

    CHECK(!use_short_home_prefix("some/random/path").has_value());
    CHECK(!use_short_home_prefix("file.txt").has_value());
  }

  TEST_CASE("make_native_string")
  {
    CHECK(!make_native_string(nullptr).has_value());

#if TACTILE_OS_WINDOWS
    CHECK(make_native_string("") == L"");
    CHECK(make_native_string("1") == L"1");
    CHECK(make_native_string("foo") == L"foo");
    CHECK(make_native_string("bar.txt") == L"bar.txt");
    CHECK(make_native_string("foo/bar") == L"foo/bar");
    CHECK(make_native_string("foo/bar.txt") == L"foo/bar.txt");
    CHECK(make_native_string("\0") == L"\0");
#else
    CHECK(make_native_string("") == "");
    CHECK(make_native_string("1") == "1");
    CHECK(make_native_string("foo") == "foo");
    CHECK(make_native_string("bar.txt") == "bar.txt");
    CHECK(make_native_string("foo/bar") == "foo/bar");
    CHECK(make_native_string("foo/bar.txt") == "foo/bar.txt");
    CHECK(make_native_string("\0") == "\0");
#endif  // TACTILE_OS_WINDOWS
  }
}
