// Copyright (C) 2025 Albin Johansson

module;

#include <algorithm>
#include <array>
#include <charconv>
#include <chrono>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <exception>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <print>
#include <random>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

export module tactile.core.ext.std;

export namespace std {

// From <algorithm>
using std::clamp;
using std::iter_swap;
namespace ranges {
using std::ranges::find_if;
}  // namespace ranges

// From <array>
using std::array;

// From <charconv>
using std::from_chars;

// From <chrono>
namespace chrono {
using std::chrono::duration;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;
using std::chrono::steady_clock;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::operator+;
using std::chrono::operator-;
using std::chrono::operator*;
using std::chrono::operator/;
using std::chrono::operator==;
using std::chrono::operator<=;
using std::chrono::operator>=;
using std::chrono::operator<;
using std::chrono::operator>;
}  // namespace chrono

// From <compare>
using std::partial_ordering;

// From <concepts>
using std::integral;
using std::signed_integral;
using std::unsigned_integral;
using std::floating_point;
using std::same_as;
using std::invocable;
using std::derived_from;

// From <cstddef>
using std::size_t;
using std::ptrdiff_t;

// From <cstdint>
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

// From <cstdio>
using std::printf;

// From <deque>
using std::deque;

// From <exception>
using std::exception;

// From <expected>
using std::expected;
using std::unexpected;
using std::unexpect;

// From <filesystem>
namespace filesystem {
using std::filesystem::path;
using std::filesystem::operator==;
using std::filesystem::operator!=;
}  // namespace filesystem

// From <format>
using std::format_args;
using std::format_string;
using std::format;
using std::format_to;
using std::vformat;
using std::vformat_to;
using std::make_format_args;

// From <fstream>
using std::basic_ifstream;
using std::basic_ofstream;
using std::ifstream;
using std::ofstream;

// From <functional>
using std::hash;
using std::less;

// From <ios>
using std::ios;

// From <iostream>
using std::clog;
using std::cout;
using std::cerr;

// From <iterator>
using std::back_inserter;

// From <limits>
using std::numeric_limits;

// From <map>
using std::map;

// From <memory>
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::default_delete;
using std::make_unique;
using std::make_shared;

// From <numeric>
using std::iota;

// From <optional>
using std::optional;
using std::nullopt_t;
using std::nullopt;
using std::make_optional;

// From <print>
using std::print;
using std::println;

// From <random>
using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

// From <span>
using std::span;
using std::dynamic_extent;

// From <stdexcept>
using std::runtime_error;
using std::logic_error;
using std::range_error;
using std::invalid_argument;
using std::underflow_error;
using std::overflow_error;
using std::out_of_range;

// From <string>
using std::string;
using std::to_string;

// From <string_view>
using std::string_view;

// From <system_error>
using std::errc;

// From <type_traits>
using std::make_signed_t;
using std::make_unsigned_t;
using std::invoke_result_t;

// From <unordered_map>
using std::unordered_map;

// From <utility>
using std::move;
using std::forward;
using std::exchange;
using std::swap;
using std::in_range;
using std::cmp_equal;
using std::cmp_not_equal;
using std::cmp_less;
using std::cmp_less_equal;
using std::cmp_greater;
using std::cmp_greater_equal;
using std::to_underlying;

// From <variant>
using std::variant;
using std::monostate;
using std::get;
using std::get_if;
using std::holds_alternative;
using std::visit;

// From <vector>
using std::vector;

// Miscellaneous
using std::operator==;
using std::operator!=;
using std::operator<=;
using std::operator>=;
using std::operator<;
using std::operator>;
using std::operator<<;
using std::operator>>;

}  // namespace std
