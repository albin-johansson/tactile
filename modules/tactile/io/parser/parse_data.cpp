#include "parse_data.hpp"

#include <utility>  // move

namespace tactile::parsing {

void parse_data::set_path(std::filesystem::path path)
{
  mPath = std::move(path);
}

void parse_data::set_error(const parse_error error)
{
  mError = error;
}

auto parse_data::path() const -> const std::filesystem::path&
{
  return mPath;
}

auto parse_data::error() const -> parse_error
{
  return mError;
}

auto parse_data::data() -> map_data&
{
  return mData;
}

auto parse_data::data() const -> const map_data&
{
  return mData;
}

}  // namespace tactile::parsing