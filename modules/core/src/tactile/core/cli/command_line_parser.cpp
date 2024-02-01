// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cli/command_line_parser.hpp"

#include <exception>  // exception
#include <iostream>   // cout, cerr

#include <argparse/argparse.hpp>

#include "tactile/foundation/container/hash_map.hpp"
#include "tactile/foundation/container/lookup.hpp"
#include "tactile/foundation/io/filesystem.hpp"

namespace tactile::core {
namespace {

inline constexpr int kDefaultVerbosityLevel = 4;

inline const HashMap<int, LogLevel> kVerbosityToLogLevel = {
  {1, LogLevel::kFatal},
  {2, LogLevel::kError},
  {3, LogLevel::kWarn},
  {4, LogLevel::kInfo},
  {5, LogLevel::kDebug},
  {6, LogLevel::kTrace},
};

void _init_parser(argparse::ArgumentParser& parser)
{
  parser.add_argument("-h", "--help")
      .help("Prints this help message and exits.")
      .nargs(0);

  parser.add_argument("-v", "--version")
      .help("Prints the current version number and exits.")
      .nargs(0);

  parser.add_argument("--persistent-dir")
      .help("Prints the file path to the persistent file directory and exits.")
      .nargs(0);

  parser.add_argument("--reset").help("Resets all persistent settings.").nargs(0);

  parser.add_argument("--verbosity")
      .help("Sets the verbosity of log output.")
      .metavar("<level>")
      .nargs(1)
      .scan<'i', int>()
      .default_value(kDefaultVerbosityLevel);

  parser.add_argument("--renderer")
      .help("Sets the preferred renderer backend.")
      .metavar("<api>")
      .nargs(1)
      .default_value("OpenGL");

  parser.add_epilog(
      "Valid verbosity levels are [0, 6], use 0 to disable logging.\n"
      "Possible renderer backends are: \"OpenGL\", and \"Vulkan\".");
}

}  // namespace

auto CommandLineParser::parse(const int argc, char** argv) -> CommandLineOptions
{
  CommandLineOptions options {};
  options.log_level = LogLevel::kInfo;

  try {
    argparse::ArgumentParser parser {"tactile",
                                     TACTILE_VERSION_STRING,
                                     argparse::default_arguments::none};
    _init_parser(parser);

    parser.parse_args(argc, argv);

    if (parser.is_used("--help")) {
      std::cout << parser << '\n';
      options.should_exit = true;
    }
    else if (parser.is_used("--version")) {
      std::cout << "Tactile " TACTILE_VERSION_STRING "\n";
      options.should_exit = true;
    }
    else if (parser.is_used("--persistent-dir")) {
      std::cout << get_persistent_storage_directory().string() << '\n';
      options.should_exit = true;
    }

    const auto verbosity = parser.get<int>("verbosity");
    if (const auto log_level_iter = kVerbosityToLogLevel.find(verbosity);
        log_level_iter != kVerbosityToLogLevel.end()) {
      options.log_level = log_level_iter->second;
    }

    if (parser.is_used("--renderer")) {
      const auto& renderer = parser.get<String>("--renderer");
      if (renderer == "OpenGL") {
        options.renderer_id = RendererID::kOpenGL;
      }
      else if (renderer == "Vulkan") {
        options.renderer_id = RendererID::kVulkan;
      }
      else {
        std::cerr << "Invalid renderer '" << renderer << "'\n";
        options.should_exit = true;
      }
    }

    options.reset_settings = parser.is_used("--reset");
  }
  catch (const std::exception& ex) {
    std::cerr << "Could not parse command line arguments: " << ex.what() << '\n';
  }

  return options;
}

}  // namespace tactile::core
