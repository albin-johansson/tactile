// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/format/save_format_error.hpp"

namespace tactile {

auto SaveFormatErrorDomain::get_message(const uint32 error_id) const noexcept
    -> StringView
{
  switch (Error {error_id}) {
    case Error::kInvalidOperation: return "An invalid operation was attempted.";

    case Error::kUnknown: return "An unknown error occurred.";

    case Error::kBadFile: return "Could not read/save a file.";

    case Error::kUnsupportedFormat:
      return "Tried to load a file using an unsupported file format.";

    case Error::kMissingKey: return "An expected attribute was not found in a save file.";

    case Error::kUnsupportedOrientation:
      return "An unsupported map orientation was detected.";

    case Error::kUnsupportedLayerType: return "An unsupported layer type was detected";

    case Error::kUnsupportedPropertyType:
      return "An unsupported property type was detected";

    case Error::kUnsupportedTileEncoding:
      return "An unsupported tile encoding was detected";

    case Error::kBadColorProperty: return "An invalid color property was detected";

    case Error::kBadTileLayerData: return "Invalid tile layer data was detected";

    case Error::kBadCompressionMode: return "An invalid compression mode was detected";
  }

  return "";
}

}  // namespace tactile
