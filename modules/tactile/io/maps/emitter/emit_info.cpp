#include "emit_info.hpp"

#include <utility>  // move

#include "tactile_stdlib.hpp"
#include "throw.hpp"

namespace tactile::emitter {

emit_info::emit_info(std::filesystem::path destination, ir::map_data data)
    : mDestinationFile{std::move(destination)}
    , mDestinationDir{mDestinationFile.parent_path()}
    , mData{std::move(data)}
{}

auto emit_info::destination_file() const -> const std::filesystem::path&
{
  return mDestinationFile;
}

auto emit_info::destination_dir() const -> const std::filesystem::path&
{
  return mDestinationDir;
}

auto emit_info::data() const -> const ir::map_data&
{
  return mData;
}

auto stringify(attribute_type type) -> std::string
{
  switch (type) {
    case attribute_type::string:
      return "string";

    case attribute_type::integer:
      return "int";

    case attribute_type::floating:
      return "float";

    case attribute_type::boolean:
      return "bool";

    case attribute_type::file:
      return "file";

    case attribute_type::color:
      return "color";

    case attribute_type::object:
      return "object";

    default:
      ThrowTraced(TactileError{"Could not recognize property type!"});
  }
}

}  // namespace tactile::emitter
