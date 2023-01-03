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

#include "common/predef.hpp"
#include "common/util/env.hpp"

namespace tactile::test {

TEST_SUITE("Filesystem")
{
  TEST_CASE("convert_to_forward_slashes")
  {
    const fs::path source = R"(C:\foo\bar\abc.yaml)";
    REQUIRE("C:/foo/bar/abc.yaml" == convert_to_forward_slashes(source));
  }

  TEST_CASE("has_home_prefix")
  {
    const auto home = env_var(on_windows ? "USERPROFILE" : "HOME").value();

    REQUIRE(!has_home_prefix(""));
    REQUIRE(!has_home_prefix("foo.cpp"));
    REQUIRE(!has_home_prefix("foo/bar.yaml"));
    REQUIRE(!has_home_prefix("some/random/path"));

    REQUIRE(has_home_prefix(to_path(home)));
    REQUIRE(has_home_prefix(to_path(home + "/")));
    REQUIRE(has_home_prefix(to_path(home + "/foo")));
    REQUIRE(has_home_prefix(to_path(home + "/foo.txt")));
    REQUIRE(has_home_prefix(to_path(home + "/foo/bar.txt")));
  }

  TEST_CASE("to_canonical")
  {
    const auto home = env_var(on_windows ? "USERPROFILE" : "HOME").value();

    REQUIRE("~" == to_canonical(to_path(home)));
    REQUIRE("~/" == to_canonical(to_path(home + '/')));
    REQUIRE("~/foo/" == to_canonical(to_path(home + "/foo/")));
    REQUIRE("~/foo/bar.txt" == to_canonical(to_path(home + "/foo/bar.txt")));

    REQUIRE(!to_canonical("some/random/path").has_value());
    REQUIRE(!to_canonical("file.txt").has_value());
  }

  TEST_CASE("to_os_string")
  {
    REQUIRE(!to_os_string(nullptr).has_value());

#if TACTILE_OS_WINDOWS
    REQUIRE(L"" == to_os_string(""));
    REQUIRE(L"1" == to_os_string("1"));
    REQUIRE(L"foo" == to_os_string("foo"));
    REQUIRE(L"bar.txt" == to_os_string("bar.txt"));
    REQUIRE(L"foo/bar" == to_os_string("foo/bar"));
    REQUIRE(L"foo/bar.txt" == to_os_string("foo/bar.txt"));
    REQUIRE(L"\0" == to_os_string("\0"));
#else
    REQUIRE("" == to_os_string(""));
    REQUIRE("1" == to_os_string("1"));
    REQUIRE("foo" == to_os_string("foo"));
    REQUIRE("bar.txt" == to_os_string("bar.txt"));
    REQUIRE("foo/bar" == to_os_string("foo/bar"));
    REQUIRE("foo/bar.txt" == to_os_string("foo/bar.txt"));
    REQUIRE("\0" == to_os_string("\0"));
#endif  // TACTILE_OS_WINDOWS
  }
}

}  // namespace tactile::test
