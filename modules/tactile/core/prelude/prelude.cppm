// Copyright (C) 2025 Albin Johansson

/// Provides common vocabulary types and functions.
export module tactile.core.prelude;

import tactile.core.ext.std;

export namespace tactile {

using usize = std::size_t;
using isize = std::ptrdiff_t;

using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;
using ulonglong = unsigned long long;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

template <typename T, typename Deleter = std::default_delete<T>>
using Unique = std::unique_ptr<T, Deleter>;

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T>
using Weak = std::weak_ptr<T>;

template <typename T, usize Extent = std::dynamic_extent>
using Span = std::span<T, Extent>;

template <typename T, usize N>
using Array = std::array<T, N>;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Deque = std::deque<T>;

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

template <typename K, typename V>
using TreeMap = std::map<K, V, std::less<>>;

template <typename T>
using Option = std::optional<T>;

template <typename T, typename E>
using Expected = std::expected<T, E>;

template <typename T>
using Unexpected = std::unexpected<T>;

using String = std::string;

using StringView = std::string_view;

using Path = std::filesystem::path;

using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;
using SteadyClock = std::chrono::steady_clock;
using SystemClock = std::chrono::system_clock;

using std::make_shared;
using std::make_unique;
using std::chrono::duration_cast;

inline constexpr std::nullopt_t kNone {std::nullopt};

enum class Platform : u8
{
  kLinux,
  kMacOS,
  kWindows,
};

#ifdef NDEBUG
inline constexpr bool kIsDebugBuild {false};
#else
inline constexpr bool kIsDebugBuild {true};
#endif

#if defined(_WIN32)
inline constexpr auto kCurrentPlatform = Platform::kWindows;
#elif defined(__APPLE__) && defined(__MACH__)
inline constexpr auto kCurrentPlatform = Platform::kMacOS;
#elif defined(__linux__)
inline constexpr auto kCurrentPlatform = Platform::kLinux;
#else
  #error "Unsupported operating system"
#endif

inline constexpr auto kMinI8 = std::numeric_limits<i8>::min();
inline constexpr auto kMaxI8 = std::numeric_limits<i8>::max();

inline constexpr auto kMinI16 = std::numeric_limits<i16>::min();
inline constexpr auto kMaxI16 = std::numeric_limits<i16>::max();

inline constexpr auto kMinI32 = std::numeric_limits<i32>::min();
inline constexpr auto kMaxI32 = std::numeric_limits<i32>::max();

inline constexpr auto kMinI64 = std::numeric_limits<i64>::min();
inline constexpr auto kMaxI64 = std::numeric_limits<i64>::max();

inline constexpr auto kMinU8 = std::numeric_limits<u8>::min();
inline constexpr auto kMaxU8 = std::numeric_limits<u8>::max();

inline constexpr auto kMinU16 = std::numeric_limits<u16>::min();
inline constexpr auto kMaxU16 = std::numeric_limits<u16>::max();

inline constexpr auto kMinU32 = std::numeric_limits<u32>::min();
inline constexpr auto kMaxU32 = std::numeric_limits<u32>::max();

inline constexpr auto kMinU64 = std::numeric_limits<u64>::min();
inline constexpr auto kMaxU64 = std::numeric_limits<u64>::max();

}  // namespace tactile
