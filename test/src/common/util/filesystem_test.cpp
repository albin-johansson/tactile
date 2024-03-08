/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "common/util/env.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::test {

TEST_SUITE("Filesystem")
{
  TEST_CASE("use_forward_slashes")
  {
    const fs::path source = R"(C:\foo\bar\abc.yaml)";
    REQUIRE("C:/foo/bar/abc.yaml" == use_forward_slashes(source));
  }

  TEST_CASE("has_home_prefix")
  {
    const auto home = env_var(kOnWindows ? "USERPROFILE" : "HOME").value();

    REQUIRE(!has_home_prefix(""));
    REQUIRE(!has_home_prefix("foo.cpp"));
    REQUIRE(!has_home_prefix("foo/bar.yaml"));
    REQUIRE(!has_home_prefix("some/random/path"));

    REQUIRE(has_home_prefix(Path {home}));
    REQUIRE(has_home_prefix(Path {home + "/"}));
    REQUIRE(has_home_prefix(Path {home + "/foo"}));
    REQUIRE(has_home_prefix(Path {home + "/foo.txt"}));
    REQUIRE(has_home_prefix(Path {home + "/foo/bar.txt"}));
  }

  TEST_CASE("use_short_home_prefix")
  {
    const auto home = env_var(kOnWindows ? "USERPROFILE" : "HOME").value();

    REQUIRE("~" == use_short_home_prefix(Path {home}));
    REQUIRE("~/" == use_short_home_prefix(Path {home + '/'}));
    REQUIRE("~/foo/" == use_short_home_prefix(Path {home + "/foo/"}));
    REQUIRE("~/foo/bar.txt" == use_short_home_prefix(Path {home + "/foo/bar.txt"}));

    REQUIRE(!use_short_home_prefix("some/random/path").has_value());
    REQUIRE(!use_short_home_prefix("file.txt").has_value());
  }

  TEST_CASE("make_native_string")
  {
    REQUIRE(!make_native_string(nullptr).has_value());

#if TACTILE_OS_WINDOWS
    REQUIRE(L"" == make_native_string(""));
    REQUIRE(L"1" == make_native_string("1"));
    REQUIRE(L"foo" == make_native_string("foo"));
    REQUIRE(L"bar.txt" == make_native_string("bar.txt"));
    REQUIRE(L"foo/bar" == make_native_string("foo/bar"));
    REQUIRE(L"foo/bar.txt" == make_native_string("foo/bar.txt"));
    REQUIRE(L"\0" == make_native_string("\0"));
#else
    REQUIRE("" == make_native_string(""));
    REQUIRE("1" == make_native_string("1"));
    REQUIRE("foo" == make_native_string("foo"));
    REQUIRE("bar.txt" == make_native_string("bar.txt"));
    REQUIRE("foo/bar" == make_native_string("foo/bar"));
    REQUIRE("foo/bar.txt" == make_native_string("foo/bar.txt"));
    REQUIRE("\0" == make_native_string("\0"));
#endif  // TACTILE_OS_WINDOWS
  }
}

}  // namespace tactile::test
