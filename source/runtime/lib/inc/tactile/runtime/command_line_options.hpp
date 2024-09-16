// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>   // uint8_t
#include <optional>  // optional

#include "tactile/base/prelude.hpp"
#include "tactile/base/render/renderer_options.hpp"
#include "tactile/runtime/api.hpp"

namespace tactile {

enum class RendererBackendId : std::uint8_t
{
  kOpenGL,
  kVulkan,
};

struct CommandLineOptions final
{
  RendererBackendId renderer_backend;
  RendererOptions renderer_options;
  bool load_zlib;
  bool load_zstd;
  bool load_yaml_format;
  bool load_tiled_tmj_format;
  bool load_tiled_tmx_format;
  bool load_godot_tscn_format;
};

[[nodiscard]]
TACTILE_RUNTIME_API auto parse_command_line_options(int argc, char* argv[])
    -> std::optional<CommandLineOptions>;

}  // namespace tactile
