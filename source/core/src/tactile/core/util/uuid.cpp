// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/uuid.hpp"

#include <algorithm>  // copy, all_of
#include <format>     // format

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_hash.hpp>

#include "tactile/base/util/hash.hpp"

namespace tactile {

auto UUID::generate() -> UUID
{
  static boost::uuids::random_generator uuid_generator {};
  const auto value = uuid_generator();

  UUID uuid {};
  std::ranges::copy(value, uuid.mData.begin());

  return uuid;
}

auto UUID::hash_code() const noexcept -> std::size_t
{
  std::size_t hash {};

  for (const std::uint8_t byte : mData) {
    hash_combine(hash, byte);
  }

  return hash;
}

auto UUID::is_null() const -> bool
{
  return std::ranges::all_of(mData, [](const std::uint8_t byte) { return byte == 0; });
}

auto to_string(const UUID& uuid) -> std::string
{
  return std::format(
      "{:02X}{:02X}{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
      uuid.mData[0],
      uuid.mData[1],
      uuid.mData[2],
      uuid.mData[3],
      uuid.mData[4],
      uuid.mData[5],
      uuid.mData[6],
      uuid.mData[7],
      uuid.mData[8],
      uuid.mData[9],
      uuid.mData[10],
      uuid.mData[11],
      uuid.mData[12],
      uuid.mData[13],
      uuid.mData[14],
      uuid.mData[15]);
}

auto operator<<(std::ostream& stream, const UUID& uuid) -> std::ostream&
{
  return stream << to_string(uuid);
}

}  // namespace tactile
