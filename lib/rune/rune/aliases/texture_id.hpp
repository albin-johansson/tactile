#ifndef RUNE_ALIASES_TEXTURE_ID_HPP
#define RUNE_ALIASES_TEXTURE_ID_HPP

#include <nenya.hpp>  // strong_type

#include "integers.hpp"

namespace rune {

/// \cond FALSE
namespace tags {
struct texture_id_tag;
}  // namespace tags
/// \endcond

/// \addtogroup core
/// \{

/**
 * \def RUNE_TEXTURE_ID_UNDERLYING_TYPE
 *
 * \brief The underlying type of the `texture_id` strong type.
 *
 * \details By default, the underlying type is `usize`.
 *
 * \note The value of this macro must be of a hashable type.
 */
#ifndef RUNE_TEXTURE_ID_UNDERLYING_TYPE
#define RUNE_TEXTURE_ID_UNDERLYING_TYPE usize
#endif  // RUNE_TEXTURE_ID_UNDERLYING_TYPE

/**
 * \typedef texture_id
 *
 * \brief Used as unique identifiers for textures.
 *
 * \details This is used in order to avoid loading the same texture more than once.
 *
 * \see `RUNE_TEXTURE_ID_UNDERLYING_TYPE`
 */
using texture_id =
    nenya::strong_type<RUNE_TEXTURE_ID_UNDERLYING_TYPE, tags::texture_id_tag>;

/// \} End of group core

}  // namespace rune

#endif  // RUNE_ALIASES_TEXTURE_ID_HPP
