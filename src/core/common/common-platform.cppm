// Copyright (C) 2025 Albin Johansson

export module tactile.core.common:platform;

export import :primitives;

export namespace tactile {

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

}  // namespace tactile
