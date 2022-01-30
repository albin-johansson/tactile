#include "parse_data.hpp"

namespace tactile::parsing {

void parse_data::set_path(const std::filesystem::path& path)
{
  mPath = std::filesystem::absolute(path);
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

auto parse_data::data() -> ir::map_data&
{
  return mData;
}

auto parse_data::data() const -> const ir::map_data&
{
  return mData;
}

}  // namespace tactile::parsing