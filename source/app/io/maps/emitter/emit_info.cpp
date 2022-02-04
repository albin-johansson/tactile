#include "emit_info.hpp"

#include <utility>  // move

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

}  // namespace tactile::emitter
