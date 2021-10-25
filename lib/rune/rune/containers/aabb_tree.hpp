#ifndef RUNE_AABB_TREE_HPP_
#define RUNE_AABB_TREE_HPP_

#include <algorithm>      // min, max
#include <array>          // array
#include <cassert>        // assert
#include <cmath>          // abs
#include <concepts>       // floating_point, invocable, same_as
#include <cstddef>        // byte
#include <deque>          // deque
#include <iterator>       // output_iterator
#include <limits>         // numeric_limits
#include <ostream>        // ostream
#include <stack>          // stack
#include <string>         // string
#include <type_traits>    // invoke_result_t
#include <unordered_map>  // unordered_map
#include <vector>         // vector

#include "../common/integers.hpp"
#include "../common/maybe.hpp"
#include "../math/vector2.hpp"
#include "aabb.hpp"
#include "aabb_node.hpp"
#include "stack_resource.hpp"

namespace rune {

/// \addtogroup containers
/// \{

/**
 * \class aabb_tree
 *
 * \brief An implementation of an AABB tree, intended to be used for efficient collision
 * detection.
 *
 * \details The usage of this class is relatively straight forward, register IDs with
 * AABBs and update their positions (or even size) throughout the duration of your game.
 * The following code illustrates the use of some of the core functions.
 * \code{cpp}
 * rune::aabb_tree<int> tree;  // AABB tree using integer keys
 *
 * // Add some AABB entries
 * tree.insert(42, {10, 10}, {20, 20});
 * tree.insert(123, {150, 125}, {192, 234});
 * tree.insert(27, {59, 95}, {73, 106});
 *
 * // Update the position of an existing AABB
 * tree.set_position(42, {27, 43});
 *
 * // Query the tree for collision candidates for a certain AABB
 * tree.query(42, [](int id) {
 *   // Invoked for each collision candidate (return true to stop query)
 * });
 *
 * // Remove an AABB
 * tree.erase(27);
 *
 * \endcode
 *
 * \tparam Key the type of the keys associated with tree entries.
 * \tparam Precision the floating-point type used, e.g. by stored vectors.
 *
 * \since 0.1.0
 */
template <typename Key, std::floating_point Precision = float>
class aabb_tree final
{
 public:
  using key_type = Key;
  using precision_type = Precision;
  using vector_type = basic_vector2<precision_type>;
  using aabb_type = basic_aabb<precision_type>;
  using node_type = aabb_node<key_type, precision_type>;
  using index_type = usize;
  using size_type = usize;

  // TODO reserve()?

  /// \name Construction
  /// \{

  explicit aabb_tree(const size_type capacity = 64u) : m_nodeCapacity{capacity}
  {
    assert(!m_root);
    assert(m_nodeCount == 0);
    assert(m_nodeCapacity == capacity);

    resize_to_match_node_capacity(0);
    assert(m_nextFreeIndex == 0);
  }

  /// \} End of construction

  /// \name Insertion/Deletion
  /// \{

  void insert(const key_type& key, const vector_type& lower, const vector_type& upper)
  {
    assert(!m_indices.contains(key));

    const auto index = allocate_node();

    {
      auto& node = m_nodes.at(index);
      node.id = key;
      node.box = {lower, upper};
      node.height = 0;

      if (m_thickness)
      {
        fatten(node.box, *m_thickness);
      }
    }

    insert_leaf(index);
    m_indices.emplace(key, index);

#ifdef RUNE_DEBUG_CONTAINERS
    validate();
#endif  // RUNE_DEBUG_CONTAINERS
  }

  void erase(const key_type& key)
  {
    if (const auto it = m_indices.find(key); it != m_indices.end())
    {
      const auto index = it->second;
      assert(index < m_nodeCapacity);
      assert(is_leaf(m_nodes.at(index)));

      m_indices.erase(it);

      remove_leaf(index);
      free_node(index);

#ifdef RUNE_DEBUG_CONTAINERS
      validate();
#endif  // RUNE_DEBUG_CONTAINERS
    }
  }

  void clear()
  {
    auto it = m_indices.begin();

    while (it != m_indices.end())
    {
      const auto nodeIndex = it->second;
      assert(nodeIndex < m_nodeCapacity);
      assert(is_leaf(m_nodes.at(nodeIndex)));

      remove_leaf(nodeIndex);
      free_node(nodeIndex);

      ++it;
    }

    m_indices.clear();

#ifdef RUNE_DEBUG_CONTAINERS
    validate();
#endif  // RUNE_DEBUG_CONTAINERS
  }

  /// \} End of insertion/deletion

  /// \name Tree structure
  /// \{

  void rebuild()
  {
    if (empty())
    {
      return;
    }

    std::vector<index_type> indices(m_nodeCount);
    int count{0};

    for (size_type index = 0; index < m_nodeCapacity; ++index)
    {
      auto& node = m_nodes.at(index);

      if (node.height < 0)
      {
        continue;
      }

      if (is_leaf(node))
      {
        node.parent = nothing;
        indices.at(static_cast<size_type>(count)) = index;
        ++count;
      }
      else
      {
        free_node(index);
      }
    }

    while (count > 1)
    {
      auto minCost = std::numeric_limits<precision_type>::max();
      int iMin{-1};
      int jMin{-1};

      for (auto i = 0; i < count; ++i)
      {
        const auto fstAabb = m_nodes.at(indices.at(static_cast<size_type>(i))).box;

        for (auto j = (i + 1); j < count; ++j)
        {
          const auto sndAabb = m_nodes.at(indices.at(static_cast<size_type>(j))).box;
          const auto cost = merge(fstAabb, sndAabb).area;

          if (cost < minCost)
          {
            iMin = i;
            jMin = j;
            minCost = cost;
          }
        }
      }

      const auto index1 = indices.at(iMin);
      const auto index2 = indices.at(jMin);

      const auto parentIndex = allocate_node();

      {
        auto& parentNode = m_nodes.at(parentIndex);
        auto& index1Node = m_nodes.at(index1);
        auto& index2Node = m_nodes.at(index2);

        parentNode.left = index1;
        parentNode.right = index2;
        parentNode.height = 1 + std::max(index1Node.height, index2Node.height);
        parentNode.box = merge(index1Node.box, index2Node.box);
        parentNode.parent = nothing;

        index1Node.parent = parentIndex;
        index2Node.parent = parentIndex;
      }

      indices.at(jMin) = indices.at(count - 1);
      indices.at(iMin) = parentIndex;

      --count;
    }

    m_root = indices.at(0);

#ifdef RUNE_DEBUG_CONTAINERS
    validate();
#endif  // RUNE_DEBUG_CONTAINERS
  }

  void print(std::ostream& stream) const
  {
    stream << "aabb_tree\n";
    print(stream, "", m_root, false);
  }

  [[nodiscard]] auto maximum_balance() const -> size_type
  {
    size_type maxBalance{};

    for (size_type index = 0; index < m_nodeCapacity; ++index)
    {
      const auto& node = m_nodes.at(index);
      if (node.height > 2)
      {
        assert(!is_leaf(node));
        assert(node.left);
        assert(node.right);

        const auto leftHeight = m_nodes.at(*node.left).height;
        const auto rightHeight = m_nodes.at(*node.right).height;

        const auto balance = std::abs(leftHeight - rightHeight);
        maxBalance = std::max(maxBalance, static_cast<size_type>(balance));
      }
    }

    return maxBalance;
  }

  [[nodiscard]] auto surface_area_ratio() const -> precision_type
  {
    if (!m_root)
    {
      return 0;
    }

    const auto rootArea = compute_area(m_nodes.at(*m_root).box);

    precision_type totalArea{};
    for (size_type index = 0; index < m_nodeCapacity; ++index)
    {
      const auto& node = m_nodes.at(index);
      if (node.height >= 0)
      {
        totalArea += compute_area(node.box);
      }
    }

    return totalArea / rootArea;
  }

  [[nodiscard]] auto height() const -> int
  {
    return m_root ? m_nodes.at(*m_root).height : 0;
  }

  [[nodiscard]] auto node_count() const noexcept -> size_type
  {
    return m_nodeCount;
  }

  /// \} End of tree structure

  /// \name AABB mutators
  /// \{

  auto update(const key_type& key, aabb_type box, const bool forceReinsert = false)
      -> bool
  {
    if (const auto it = m_indices.find(key); it != m_indices.end())
    {
      const auto nodeIndex = it->second;
      assert(nodeIndex < m_nodeCapacity);
      assert(is_leaf(m_nodes.at(nodeIndex)));

      // No need to update if the particle is still within its fattened AABB.
      if (!forceReinsert && m_nodes.at(nodeIndex).box.contains(box))
      {
        return false;
      }

      remove_leaf(nodeIndex);

      if (m_thickness)
      {
        fatten(box, *m_thickness);
      }

      {
        auto& node = m_nodes.at(nodeIndex);
        node.box = box;
        node.box.area = compute_area(node.box);
      }

      insert_leaf(nodeIndex);

#ifdef RUNE_DEBUG_CONTAINERS
      validate();
#endif  // RUNE_DEBUG_CONTAINERS
      return true;
    }
    else
    {
      return false;
    }
  }

  auto update(const key_type& key,
              const vector_type& lower,
              const vector_type& upper,
              const bool forceReinsert = false) -> bool
  {
    return update(key, aabb_type{lower, upper}, forceReinsert);
  }

  auto set_position(const key_type& key,
                    const vector_type& position,
                    const bool forceReinsert = false) -> bool
  {
    if (const auto it = m_indices.find(key); it != m_indices.end())
    {
      const auto& box = m_nodes.at(it->second).box;
      return update(key, {position, position + box.size()}, forceReinsert);
    }
    else
    {
      return false;
    }
  }

  /// \} End of AABB mutators

  /// \name AABB queries
  /// \{

  [[nodiscard]] auto at(const key_type& key) const -> const aabb_type&
  {
    const auto index = m_indices.at(key);
    return m_nodes.at(index).box;
  }

  [[nodiscard]] auto find(const key_type& key) const -> const aabb_type*
  {
    if (const auto it = m_indices.find(key); it != m_indices.end())
    {
      const auto index = it->second;
      return &m_nodes.at(index).box;
    }
    else
    {
      return nullptr;
    }
  }

  [[nodiscard]] auto size() const noexcept -> size_type
  {
    return m_indices.size();
  }

  [[nodiscard]] auto empty() const noexcept -> bool
  {
    return m_indices.empty();
  }

  /// \} End of AABB queries

  /// \name Thickness factor
  /// \{

  void disable_thickness_factor()
  {
    m_thickness.reset();
  }

  void set_thickness_factor(const precision_type factor)
  {
    m_thickness = factor;
  }

  [[nodiscard]] auto thickness_factor() const noexcept -> maybe<precision_type>
  {
    return m_thickness;
  }

  /// \} End of thickness factor

  /// \name Collision queries
  /// \{

  template <size_type BufferSize = 256, std::invocable<key_type> T>
  void query(const key_type& key, T&& callable) const
  {
    if (const auto it = m_indices.find(key); it != m_indices.end())
    {
      const auto index = it->second;
      const auto& sourceNode = m_nodes.at(index);

      stack_resource<BufferSize * sizeof(maybe<index_type>)> resource;
      pmr_stack<maybe<index_type>> stack{resource.get()};

      stack.push(m_root);

      bool quit{};
      while (!stack.empty() && !quit)
      {
        const auto nodeIndex = stack.top();
        stack.pop();

        if (!nodeIndex)
        {
          continue;
        }

        const auto& node = m_nodes.at(*nodeIndex);

        // Test for overlap between the AABBs
        if (overlaps(sourceNode.box, node.box, m_touchIsOverlap))
        {
          if (is_leaf(node) && node.id && node.id != key)
          {
            // The boolean return type is optional
            if constexpr (std::same_as<bool, std::invoke_result_t<T, key_type>>)
            {
              quit = callable(*node.id);
            }
            else
            {
              callable(*node.id);
            }
          }
          else
          {
            stack.push(node.left);
            stack.push(node.right);
          }
        }
      }
    }
  }

  template <size_type BufferSize = 256, std::output_iterator<key_type> T>
  void query(const key_type& key, T iterator) const
  {
    query<BufferSize>(key, [&](const key_type& key) {
      *iterator = key;
      ++iterator;
    });
  }

  /// \} End of collision queries

  /// \name Serialization
  /// \{

  void serialize(auto& archive)
  {
    archive(m_nodes,
            m_indices,
            m_root,
            m_nextFreeIndex,
            m_nodeCount,
            m_nodeCapacity,
            m_thickness,
            m_touchIsOverlap);
  }

  /// \} End of serialization

 private:
  inline static constexpr auto def_thickness_factor = static_cast<precision_type>(0.05);

  std::vector<node_type> m_nodes;                      ///< The collection of nodes
  std::unordered_map<key_type, index_type> m_indices;  ///< User IDs -> Node indices
  maybe<index_type> m_root;                            ///< Root node index
  maybe<index_type> m_nextFreeIndex{0};                ///< Index of next free node
  size_type m_nodeCount{0};                            ///< Number of nodes in the tree
  size_type m_nodeCapacity{};                          ///< Current node capacity
  maybe<precision_type> m_thickness{def_thickness_factor};  ///< Thickness factor
  bool m_touchIsOverlap{true};  ///< Overlap detection strategy

  /// \name Allocation
  /// \{

  /**
   * \brief Resizes the node vector.
   *
   * \param beginInitIndex the index at which the function will start to initialize the
   * `next` and `height` members of the new nodes.
   */
  void resize_to_match_node_capacity(const index_type beginInitIndex)
  {
    m_nodes.resize(m_nodeCapacity);

    const auto end = m_nodeCapacity - 1;
    for (auto index = beginInitIndex; index < end; ++index)
    {
      auto& node = m_nodes.at(index);
      node.next = index + 1;
      node.height = -1;
    }

    auto& node = m_nodes.at(end);
    node.next = nothing;
    node.height = -1;
  }

  /// \brief Doubles the size of the node pool.
  void grow_pool()
  {
    assert(m_nodeCount == m_nodeCapacity);  // Don't grow the pool unnecessarily

    // Expand the pool of nodes, only initializing new nodes.
    m_nodeCapacity *= 2;
    resize_to_match_node_capacity(m_nodeCount);

    // Update the index of the next free node.
    m_nextFreeIndex = static_cast<index_type>(m_nodeCount);
  }

  /**
   * \brief Returns the index to a new node.
   *
   * \details This function will grow the node pool if there are no available nodes.
   * Otherwise, this function will just increment the next free node index and return the
   * index of the previous next free node.
   *
   * \return the index of the allocated node.
   */
  [[nodiscard]] auto allocate_node() -> index_type
  {
    if (!m_nextFreeIndex)
    {
      grow_pool();
    }

    const auto index = m_nextFreeIndex.value();
    auto& node = m_nodes.at(index);

    m_nextFreeIndex = node.next;

    node.parent = nothing;
    node.left = nothing;
    node.right = nothing;
    node.height = 0;

    ++m_nodeCount;

    return index;
  }

  void free_node(const index_type index)
  {
    assert(index < m_nodeCapacity);
    assert(0 < m_nodeCount);

    {
      auto& node = m_nodes.at(index);
      node.next = m_nextFreeIndex;
      node.height = -1;
    }

    m_nextFreeIndex = index;

    --m_nodeCount;
  }

  /// \} End of allocation

  /// \name Tree structure
  /// \{

  void print(std::ostream& stream,
             const std::string& prefix,
             const maybe<index_type> index,
             const bool isLeft) const
  {
    if (index)
    {
      stream << prefix << (isLeft ? "├── " : "└── ");

      const auto& node = m_nodes.at(*index);
      if (is_leaf(node))
      {
        stream << node.id.value() << '\n';
      }
      else
      {
        stream << "X\n";
      }

      print(stream, prefix + (isLeft ? "│   " : "    "), node.left, true);
      print(stream, prefix + (isLeft ? "│   " : "    "), node.right, false);
    }
  }

  [[nodiscard]] static auto left_cost(const aabb_type& leafAabb,
                                      const node_type& leftNode,
                                      const precision_type minimumCost) -> precision_type
  {
    if (is_leaf(leftNode))
    {
      return merge(leafAabb, leftNode.box).area + minimumCost;
    }
    else
    {
      const auto oldArea = leftNode.box.area;
      const auto newArea = merge(leafAabb, leftNode.box).area;
      return (newArea - oldArea) + minimumCost;
    }
  }

  [[nodiscard]] static auto right_cost(const aabb_type& leafAabb,
                                       const node_type& rightNode,
                                       const precision_type minimumCost) -> precision_type
  {
    if (is_leaf(rightNode))
    {
      return merge(leafAabb, rightNode.box).area + minimumCost;
    }
    else
    {
      const auto oldArea = rightNode.box.area;
      const auto newArea = merge(leafAabb, rightNode.box).area;
      return (newArea - oldArea) + minimumCost;
    }
  }

  [[nodiscard]] auto find_best_sibling(const aabb_type& leafAabb) const -> index_type
  {
    auto index = m_root.value();

    while (!is_leaf(m_nodes.at(index)))
    {
      const auto& node = m_nodes.at(index);
      const auto left = node.left.value();
      const auto right = node.right.value();

      const auto surfaceArea = node.box.area;
      const auto combinedSurfaceArea = merge(node.box, leafAabb).area;

      // Cost of creating a new parent for this node and the new leaf.
      const auto cost = precision_type{2.0} * combinedSurfaceArea;

      // Minimum cost of pushing the leaf further down the tree.
      const auto minimumCost = precision_type{2.0} * (combinedSurfaceArea - surfaceArea);

      const auto costLeft = left_cost(leafAabb, m_nodes.at(left), minimumCost);
      const auto costRight = right_cost(leafAabb, m_nodes.at(right), minimumCost);

      // Descend according to the minimum cost.
      if ((cost < costLeft) && (cost < costRight))
      {
        break;
      }

      if (costLeft < costRight)
      {
        index = left;
      }
      else
      {
        index = right;
      }
    }

    return index;
  }

  void rotate_right(const index_type nodeIndex,
                    const index_type leftIndex,
                    const index_type rightIndex)
  {
    auto& node = m_nodes.at(nodeIndex);
    auto& rightNode = m_nodes.at(rightIndex);

    const auto rightLeft = rightNode.left.value();
    assert(rightLeft < m_nodeCapacity);

    const auto rightRight = rightNode.right.value();
    assert(rightRight < m_nodeCapacity);

    // Swap node and its right-hand child.
    rightNode.left = nodeIndex;
    rightNode.parent = node.parent;
    node.parent = rightIndex;

    // The node's old parent should now point to its right-hand child.
    if (rightNode.parent)
    {
      auto& rightParent = m_nodes.at(*rightNode.parent);
      if (rightParent.left == nodeIndex)
      {
        rightParent.left = rightIndex;
      }
      else
      {
        assert(rightParent.right == nodeIndex);
        rightParent.right = rightIndex;
      }
    }
    else
    {
      m_root = rightIndex;
    }

    auto& leftNode = m_nodes.at(leftIndex);
    auto& rightRightNode = m_nodes.at(rightRight);
    auto& rightLeftNode = m_nodes.at(rightLeft);

    // Rotate.
    if (rightLeftNode.height > rightRightNode.height)
    {
      rightNode.right = rightLeft;
      node.right = rightRight;

      rightRightNode.parent = nodeIndex;

      node.box = merge(leftNode.box, rightRightNode.box);
      rightNode.box = merge(node.box, rightLeftNode.box);

      node.height = 1 + std::max(leftNode.height, rightRightNode.height);
      rightNode.height = 1 + std::max(node.height, rightLeftNode.height);
    }
    else
    {
      rightNode.right = rightRight;
      node.right = rightLeft;

      rightLeftNode.parent = nodeIndex;

      node.box = merge(leftNode.box, rightLeftNode.box);
      rightNode.box = merge(node.box, rightRightNode.box);

      node.height = 1 + std::max(leftNode.height, rightLeftNode.height);
      rightNode.height = 1 + std::max(node.height, rightRightNode.height);
    }
  }

  void rotate_left(const index_type nodeIndex,
                   const index_type leftIndex,
                   const index_type rightIndex)
  {
    auto& node = m_nodes.at(nodeIndex);
    auto& leftNode = m_nodes.at(leftIndex);

    const auto leftLeft = leftNode.left.value();
    assert(leftLeft < m_nodeCapacity);

    const auto leftRight = leftNode.right.value();
    assert(leftRight < m_nodeCapacity);

    // Swap node and its left-hand child.
    leftNode.left = nodeIndex;
    leftNode.parent = node.parent;
    node.parent = leftIndex;

    // The node's old parent should now point to its left-hand child.
    if (leftNode.parent)
    {
      auto& leftParent = m_nodes.at(*leftNode.parent);
      if (leftParent.left == nodeIndex)
      {
        leftParent.left = leftIndex;
      }
      else
      {
        assert(leftParent.right == nodeIndex);
        leftParent.right = leftIndex;
      }
    }
    else
    {
      m_root = leftIndex;
    }

    auto& rightNode = m_nodes.at(rightIndex);
    auto& leftLeftNode = m_nodes.at(leftLeft);
    auto& leftRightNode = m_nodes.at(leftRight);

    // Rotate.
    if (leftLeftNode.height > leftRightNode.height)
    {
      leftNode.right = leftLeft;
      node.left = leftRight;

      leftRightNode.parent = nodeIndex;

      node.box = merge(rightNode.box, leftRightNode.box);
      leftNode.box = merge(node.box, leftLeftNode.box);

      node.height = 1 + std::max(rightNode.height, leftRightNode.height);
      leftNode.height = 1 + std::max(node.height, leftLeftNode.height);
    }
    else
    {
      leftNode.right = leftRight;
      node.left = leftLeft;

      leftLeftNode.parent = nodeIndex;

      node.box = merge(rightNode.box, leftLeftNode.box);
      leftNode.box = merge(node.box, leftRightNode.box);

      node.height = 1 + std::max(rightNode.height, leftLeftNode.height);
      leftNode.height = 1 + std::max(node.height, leftRightNode.height);
    }
  }

  [[nodiscard]] auto balance(const index_type index) -> index_type
  {
    if (is_leaf(m_nodes.at(index)) || (m_nodes.at(index).height < 2))
    {
      return index;
    }

    const auto leftIndex = m_nodes.at(index).left.value();
    assert(leftIndex < m_nodeCapacity);

    const auto rightIndex = m_nodes.at(index).right.value();
    assert(rightIndex < m_nodeCapacity);

    const auto currentBalance =
        m_nodes.at(rightIndex).height - m_nodes.at(leftIndex).height;

    // Rotate right branch up.
    if (currentBalance > 1)
    {
      rotate_right(index, leftIndex, rightIndex);
      return rightIndex;
    }

    // Rotate left branch up.
    if (currentBalance < -1)
    {
      rotate_left(index, leftIndex, rightIndex);
      return leftIndex;
    }

    return index;
  }

  void fix_tree_upwards(maybe<index_type> index)
  {
    while (index)
    {
      index = balance(*index);

      auto& node = m_nodes.at(*index);

      const auto& leftNode = m_nodes.at(node.left.value());
      const auto& rightNode = m_nodes.at(node.right.value());

      node.box = merge(leftNode.box, rightNode.box);
      node.height = 1 + std::max(leftNode.height, rightNode.height);

      index = node.parent;
    }
  }

  /// \} End of tree structure

  /// \name Leaves
  /// \{

  void insert_leaf(const index_type leafIndex)
  {
    if (!m_root)
    {
      m_root = leafIndex;
      m_nodes.at(leafIndex).parent = nothing;
      return;
    }

    // Find the best sibling for the node.
    const auto leafAabb = m_nodes.at(leafIndex).box;  // copy current AABB
    const auto siblingIndex = find_best_sibling(leafAabb);

    // Create a new parent.
    const auto oldParentIndex = m_nodes.at(siblingIndex).parent;
    const auto newParentIndex = allocate_node();

    {
      auto& newParent = m_nodes.at(newParentIndex);
      newParent.parent = oldParentIndex;
      newParent.box = merge(leafAabb, m_nodes.at(siblingIndex).box);
      newParent.height = m_nodes.at(siblingIndex).height + 1;
      newParent.left = siblingIndex;
      newParent.right = leafIndex;
    }

    if (oldParentIndex)
    {
      // The sibling was not the root.
      auto& oldParent = m_nodes.at(*oldParentIndex);
      if (oldParent.left == siblingIndex)
      {
        oldParent.left = newParentIndex;
      }
      else
      {
        oldParent.right = newParentIndex;
      }
    }
    else
    {
      // The sibling was the root.
      m_root = newParentIndex;
    }

    m_nodes.at(siblingIndex).parent = newParentIndex;
    m_nodes.at(leafIndex).parent = newParentIndex;

    // Walk back up the tree fixing heights and AABBs.
    fix_tree_upwards(m_nodes.at(leafIndex).parent);
  }

  void remove_leaf(const index_type leafIndex)
  {
    if (leafIndex == m_root)
    {
      m_root = nothing;
      return;
    }

    const auto parentIndex = m_nodes.at(leafIndex).parent;
    const auto grandParentIndex = m_nodes.at(parentIndex.value()).parent;

    const auto siblingIndex = (m_nodes.at(parentIndex.value()).left == leafIndex)
                                  ? m_nodes.at(parentIndex.value()).right
                                  : m_nodes.at(parentIndex.value()).left;

    // Destroy the parent and connect the sibling to the grandparent.
    if (grandParentIndex)
    {
      if (m_nodes.at(*grandParentIndex).left == parentIndex)
      {
        m_nodes.at(*grandParentIndex).left = siblingIndex;
      }
      else
      {
        m_nodes.at(*grandParentIndex).right = siblingIndex;
      }

      m_nodes.at(siblingIndex.value()).parent = grandParentIndex;
      free_node(parentIndex.value());

      // Adjust ancestor bounds.
      fix_tree_upwards(grandParentIndex);
    }
    else
    {
      m_root = siblingIndex;
      m_nodes.at(siblingIndex.value()).parent = nothing;
      free_node(parentIndex.value());
    }
  }

  /// \} End of leaves

  /// \name Collision queries
  /// \{

  template <typename T>
  using pmr_stack = std::stack<T, std::pmr::deque<T>>;

  /// \} End of collision queries

  /// \name Validation
  /// \{

  [[nodiscard]] auto compute_height(const maybe<index_type> nodeIndex) const -> size_type
  {
    if (!nodeIndex)
    {
      return 0;
    }

    assert(nodeIndex < m_nodeCapacity);

    const auto& node = m_nodes.at(*nodeIndex);
    if (is_leaf(node))
    {
      return 0;
    }
    else
    {
      const auto left = compute_height(node.left);
      const auto right = compute_height(node.right);
      return 1 + std::max(left, right);
    }
  }

  [[nodiscard]] auto compute_height() const -> size_type
  {
    return compute_height(m_root);
  }

  void validate_structure(const maybe<index_type> nodeIndex) const
  {
    if (nodeIndex == nothing)
    {
      return;
    }

    const auto& node = m_nodes.at(*nodeIndex);

    if (nodeIndex == m_root)
    {
      assert(!node.parent);
    }

    const auto left = node.left;
    const auto right = node.right;

    if (is_leaf(node))
    {
      assert(!left);
      assert(!right);
      assert(node.height == 0);
    }
    else
    {
      assert(left < m_nodeCapacity);
      assert(right < m_nodeCapacity);
      assert(left);
      assert(right);

      assert(m_nodes.at(*left).parent == nodeIndex);
      assert(m_nodes.at(*right).parent == nodeIndex);

      validate_structure(left);
      validate_structure(right);
    }
  }

  void validate_metrics(const maybe<index_type> nodeIndex) const
  {
    if (!nodeIndex)
    {
      return;
    }

    const auto& node = m_nodes.at(*nodeIndex);
    const auto left = node.left;
    const auto right = node.right;

    if (is_leaf(node))
    {
      assert(!left);
      assert(!right);
      assert(node.height == 0);
      return;
    }
    else
    {
      assert(left < m_nodeCapacity);
      assert(right < m_nodeCapacity);
      assert(left);
      assert(right);

      const auto leftHeight = m_nodes.at(*left).height;
      const auto rightHeight = m_nodes.at(*right).height;
      const auto height = 1 + std::max(leftHeight, rightHeight);
      assert(node.height == height);

      const auto box = merge(m_nodes.at(*left).box, m_nodes.at(*right).box);

      assert(box.min.x == node.box.min.x);
      assert(box.min.y == node.box.min.y);
      assert(box.max.x == node.box.max.x);
      assert(box.max.y == node.box.max.y);

      validate_metrics(left);
      validate_metrics(right);
    }
  }

  void validate()
  {
    validate_structure(m_root);
    validate_metrics(m_root);

    auto freeCount = 0;
    auto freeIndex = m_nextFreeIndex;

    while (freeIndex)
    {
      assert(freeIndex < m_nodeCapacity);
      freeIndex = m_nodes.at(*freeIndex).next;
      ++freeCount;
    }

    assert(height() == compute_height());
    assert((m_nodeCount + freeCount) == m_nodeCapacity);
  }

  /// \} End of validation
};

template <typename Key, std::floating_point Precision>
auto operator<<(std::ostream& stream, const aabb_tree<Key, Precision>& tree)
    -> std::ostream&
{
  tree.print(stream);
  return stream;
}

/// \} End of group containers

}  // namespace rune

#endif  // RUNE_AABB_TREE_HPP_
