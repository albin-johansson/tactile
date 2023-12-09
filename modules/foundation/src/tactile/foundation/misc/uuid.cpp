// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/misc/uuid.hpp"

#include <algorithm>  // copy, all_of

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_hash.hpp>
#include <fmt/format.h>

#include "tactile/foundation/functional/hash.hpp"

namespace tactile {

auto UUID::generate() -> UUID
{
  static boost::uuids::random_generator uuid_generator;
  const auto value = uuid_generator();

  UUID uuid;
  std::copy(value.begin(), value.end(), uuid.mData.begin());

  return uuid;
}

auto UUID::hash_code() const noexcept -> usize
{
  usize hash {};

  for (const uint8 byte : mData) {
    hash_combine(hash, byte);
  }

  return hash;
}

auto UUID::is_null() const -> bool
{
  auto is_zero = [](const uint8 byte) { return byte == 0; };
  return std::all_of(mData.begin(), mData.end(), is_zero);
}

auto to_string(const UUID& uuid) -> String
{
  return fmt::format(
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

}  // namespace tactile