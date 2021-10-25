#ifndef RUNE_AABB_NODE_HPP_
#define RUNE_AABB_NODE_HPP_

#include <concepts>  // floating_point

#include "../common/integers.hpp"
#include "../common/maybe.hpp"
#include "aabb.hpp"

namespace rune {

/// \addtogroup containers
/// \{

/**
 * \class aabb_node
 *
 * \brief Represents a node in an AABB tree.
 *
 * \tparam Key the type of the associated key.
 *
 * \see `aabb_tree`
 * \see `is_leaf()`
 */
template <typename Key, std::floating_point Precision>
class aabb_node final
{
 public:
  using key_type = Key;                               ///< The type of the associated key.
  using precision_type = Precision;                   ///< The vector coordinate type.
  using vector_type = basic_vector2<precision_type>;  ///< The associated vector type.
  using aabb_type = basic_aabb<precision_type>;       ///< The associated AABB type.
  using index_type = usize;                           ///< The type used for node indices

  maybe<key_type> id;        ///< The user-provided ID associated with the AABB.
  aabb_type box;             ///< The associated AABB.
  maybe<index_type> parent;  ///< Index of parent node.
  maybe<index_type> left;    ///< Index of left child.
  maybe<index_type> right;   ///< Index of right child.
  maybe<index_type> next;    ///< Index of next adjacent node.
  int height{-1};            ///< Amount of levels below the node (0 for leaves).
};

/**
 * \brief Serializes an AABB node.
 *
 * \param archive the serialization archive that will be used.
 * \param node the AABB node that will be serialized.
 */
template <typename Key, std::floating_point Precision>
void serialize(auto& archive, aabb_node<Key, Precision>& node)
{
  archive(node.id, node.box, node.parent, node.left, node.right, node.next, node.height);
}

/**
 * \brief Indicates whether or not an AABB node is a leaf in an AABB tree.
 *
 * \param node the node that will be checked.
 *
 * \return `true` if the node is a leaf; `false` otherwise.
 */
template <typename Key, std::floating_point Precision>
[[nodiscard]] constexpr auto is_leaf(const aabb_node<Key, Precision>& node) noexcept
    -> bool
{
  return !node.left;
}

/// \} End of group containers

}  // namespace rune

#endif  // RUNE_AABB_NODE_HPP_
