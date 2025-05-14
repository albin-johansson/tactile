// Copyright (C) 2025 Albin Johansson

module;

#include <argparse/argparse.hpp>

module tactile.editor.cli;

import std;

namespace tactile::editor {
namespace {

constexpr const char* kVersionNumber = "0.5.0";

constexpr const char* kHelpMessage =
    R"(Usage: tactile [--help] [--version] [--log-level=<trc|dbg|inf|wrn|err>]

Options:
  -h, --help           Print this message
  -v, --version        Print the version number
  --log-level          The log verbosity (default: "inf"))";

void _init_parser(argparse::ArgumentParser& parser, CliOptions& options)
{
  parser.set_assign_chars("=");

  parser.add_argument("-h", "--help").nargs(0).action([&](const String&) {
    std::println("{}", kHelpMessage);
    options.should_launch = false;
  });

  parser.add_argument("-v", "--version").nargs(0).action([&](const String&) {
    std::println("{}", kVersionNumber);
    options.should_launch = false;
  });

  parser.add_argument("--log-level")
      .nargs(1)
      .choices("trc", "dbg", "inf", "wrn", "err")
      .default_value("inf")
      .action([&](const String& value) {
        if (value == "trc") {
          options.log_level = LogLevel::kTrace;
        }
        else if (value == "dbg") {
          options.log_level = LogLevel::kDebug;
        }
        else if (value == "inf") {
          options.log_level = LogLevel::kInfo;
        }
        else if (value == "wrn") {
          options.log_level = LogLevel::kWarn;
        }
        else if (value == "err") {
          options.log_level = LogLevel::kError;
        }
      });
}

}  // namespace

auto make_default_cli_options() -> CliOptions
{
  return {
    .log_level = LogLevel::kInfo,
    .should_launch = true,
  };
}

auto parse_cli_options(const int argc, char* argv[]) -> Result<CliOptions>
{
  argparse::ArgumentParser parser {"tactile",
                                   kVersionNumber,
                                   argparse::default_arguments::none};

  auto options = make_default_cli_options();
  _init_parser(parser, options);

  try {
    parser.parse_args(argc, argv);
  }
  catch (const std::exception& error) {
    std::println(std::cerr, "ERROR: {}", error.what());
    return err(Error::kInvalidArg);
  }
  catch (...) {
    std::println(std::cerr, "ERROR: Unknown");
    return err(Error::kUnknown);
  }

  return options;
}

}  // namespace tactile::editor
