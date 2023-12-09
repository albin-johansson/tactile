// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tmj-format/common/json.hpp"

#include "tactile/foundation/io/stream.hpp"

namespace tactile::tmj {

auto parse_json(const FilePath& path) -> Result<JSON>
{
  if (auto parsed_json = stream_from_file<JSON>(path)) {
    return std::move(*parsed_json);
  }
  else {
    return error(SaveFormatError::kBadFile);
  }
}

}  // namespace tactile::tmj
