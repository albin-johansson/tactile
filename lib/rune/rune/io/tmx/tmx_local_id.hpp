#ifndef RUNE_TMX_LOCAL_ID_HPP
#define RUNE_TMX_LOCAL_ID_HPP

#include <nenya.hpp>  // strong_type

#include "../../aliases/integers.hpp"

namespace rune {

/// \cond FALSE
namespace tags {
struct tmx_local_id_tag;
}  // namespace tags
/// \endcond

/// \addtogroup tmx
/// \{

using tmx_local_id = nenya::strong_type<int, tags::tmx_local_id_tag>;

/// \} End of group tmx

namespace tmx_literals {

/// \addtogroup tmx
/// \{

/// \name Literal operators
/// \{

[[nodiscard]] constexpr auto operator""_local(const ulonglong value) noexcept
    -> tmx_local_id
{
  return tmx_local_id{static_cast<tmx_local_id::value_type>(value)};
}

/// \} End of literal operators

/// \} End of group tmx

}  // namespace tmx_literals
}  // namespace rune

#endif  // RUNE_TMX_LOCAL_ID_HPP
