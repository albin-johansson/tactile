// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/save/save_format_error.hpp"

namespace tactile {
namespace {

class SaveFormatErrorCategory final : public std::error_category {
 public:
  [[nodiscard]]
  auto name() const noexcept -> const char* override
  {
    return "save format";
  }

  [[nodiscard]]
  auto message(const int value) const -> std::string override
  {
    switch (SaveFormatError {value}) {
      using enum SaveFormatError;
      case kInvalidOperation: return "an invalid operation was attempted";
      case kUnknown: return "an unknown error occurred";
      case kBadFile: return "could not read or save a file";
      case kUnsupportedFormat: return "a file uses an unsupported file format";
      case kMissingKey: return "an expected attribute was not found in a save file";
      case kUnsupportedOrientation: return "detected an unsupported map orientation";
      case kUnsupportedLayerType: return "detected an unsupported layer type";
      case kUnsupportedPropertyType: return "detected an unsupported property type";
      case kUnsupportedCompressionMode: return "detected an unsupported compression mode";
      case kUnsupportedTileEncoding: return "detected an unsupported tile encoding";
      case kBadColorProperty: return "detected an invalid color property";
      case kBadTileLayerData: return "detected an invalid tile layer data";
      case kBadCompressionMode: return "detected an invalid compression mode";
    }

    return "unknown";
  }
};

}  // namespace

auto get_save_format_error_category() noexcept -> const std::error_category&
{
  static SaveFormatErrorCategory save_format_error_category {};
  return save_format_error_category;
}

}  // namespace tactile
