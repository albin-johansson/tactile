// Copyright (C) 2025 Albin Johansson

module;

#include <cassert>

module tactile.core.layer;

import tactile.core.ext.std;
import tactile.core.error;
import tactile.core.common;
import tactile.core.util;

namespace tactile {

GroupLayer::GroupLayer(const LayerID id)
  : m_info {id}
{}

void GroupLayer::visit(IConstLayerVisitor& visitor) const
{
  visitor.on_group_layer(*this);

  for (const auto& layer : m_layers) {
    layer->visit(visitor);
  }
}

void GroupLayer::visit(ILayerVisitor& visitor)
{
  visitor.on_group_layer(*this);

  for (const auto& layer : m_layers) {
    layer->visit(visitor);
  }
}

auto GroupLayer::info() -> LayerInfo&
{
  return m_info;
}

auto GroupLayer::info() const -> const LayerInfo&
{
  return m_info;
}

void GroupLayer::append_layer(Unique<ILayer> layer)
{
  if (layer == nullptr) {
    throw std::invalid_argument {"tried to append null layer to group"};
  }

  if (find_layer(layer->info().id()) != nullptr) {
    throw std::invalid_argument {
      "tried to append layer to group more than once"};
  }

  m_layers.push_back(std::move(layer));
}

void GroupLayer::append_layer_to(const LayerID parent_id, Unique<ILayer> layer)
{
  if (layer == nullptr) {
    throw std::invalid_argument {"tried to append null layer to group"};
  }

  if (find_layer(layer->info().id()) != nullptr) {
    throw std::invalid_argument {
      "tried to append layer to group more than once"};
  }

  auto* parent_layer = dynamic_cast<GroupLayer*>(find_layer(parent_id));
  if (parent_layer == nullptr) {
    throw std::invalid_argument {"tried to append layer to invalid target"};
  }

  parent_layer->append_layer(std::move(layer));
}

auto GroupLayer::remove_layer(const LayerID id) -> Unique<ILayer>
{
  const auto result = _find_layer(id);
  if (!result.found) {
    return nullptr;
  }

  assert(result.parent_layer != nullptr);
  const auto iter = result.parent_layer->m_layers.begin() + result.rel_index;

  auto removed_layer = std::move(*iter);
  result.parent_layer->m_layers.erase(iter);

  return removed_layer;
}

auto GroupLayer::raise_layer(const LayerID id) -> Result<void>
{
  const auto result = _find_layer(id);
  if (!result.found) {
    return err(Error::kInvalidArg);
  }

  assert(result.parent_layer != nullptr);

  const auto parent_begin = result.parent_layer->m_layers.begin();
  const auto old_pos = parent_begin + result.rel_index;

  if (old_pos == parent_begin) {
    return err(Error::kInvalidOp);
  }

  const auto new_pos = old_pos - 1z;
  std::iter_swap(old_pos, new_pos);

  return ok();
}

auto GroupLayer::lower_layer(const LayerID id) -> Result<void>
{
  const auto result = _find_layer(id);
  if (!result.found) {
    return err(Error::kInvalidArg);
  }

  assert(result.parent_layer != nullptr);

  const auto parent_begin = result.parent_layer->m_layers.begin();
  const auto parent_end = result.parent_layer->m_layers.end();

  const auto old_pos = parent_begin + result.rel_index;
  if (old_pos == parent_end - 1z) {
    return err(Error::kInvalidOp);
  }

  const auto new_pos = old_pos + 1z;
  std::iter_swap(old_pos, new_pos);

  return ok();
}

auto GroupLayer::layer_index_rel(const LayerID id) const -> isize
{
  const auto result = _find_layer(id);
  if (!result.found) {
    throw std::invalid_argument {"no such layer"};
  }

  assert(result.parent_layer != nullptr);

  return result.rel_index;
}

auto GroupLayer::layer_index_abs(const LayerID id) const -> isize
{
  const auto result = _find_layer(id);
  if (!result.found) {
    throw std::invalid_argument {"no such layer"};
  }

  assert(result.parent_layer != nullptr);

  return result.abs_index;
}

auto GroupLayer::find_layer(const LayerID id) -> ILayer*
{
  const auto result = _find_layer(id);
  return result.found ? result.parent_layer->m_layers
                            .at(checked_cast<usize>(result.rel_index))
                            .get()
                      : nullptr;
}

auto GroupLayer::find_layer(const LayerID id) const -> const ILayer*
{
  const auto result = _find_layer(id);
  return result.found ? result.parent_layer->m_layers
                            .at(checked_cast<usize>(result.rel_index))
                            .get()
                      : nullptr;
}

auto GroupLayer::find_parent_layer(const LayerID id) -> GroupLayer*
{
  const auto result = _find_layer(id);
  return result.parent_layer;
}

auto GroupLayer::find_parent_layer(const LayerID id) const -> const GroupLayer*
{
  const auto result = _find_layer(id);
  return result.parent_layer;
}

auto GroupLayer::layer_count() const -> isize
{
  isize count {0};

  for (const auto& layer : m_layers) {
    ++count;

    if (const auto* group_layer =
            dynamic_cast<const GroupLayer*>(layer.get())) {
      count += group_layer->layer_count();
    }
  }

  return count;
}

auto GroupLayer::_find_layer(const LayerID id, isize abs_index)
    -> FindLayerResult
{
  isize rel_index {0};

  for (const auto& layer : m_layers) {
    if (layer->info().id() == id) {
      return FindLayerResult {.parent_layer = this,
                              .rel_index = rel_index,
                              .abs_index = abs_index,
                              .found = true};
    }

    ++rel_index;
    ++abs_index;

    if (auto* group_layer = dynamic_cast<GroupLayer*>(layer.get())) {
      const auto result = group_layer->_find_layer(id, abs_index);
      abs_index = result.abs_index;

      if (result.found) {
        return result;
      }
    }
  }

  return FindLayerResult {.parent_layer = nullptr,
                          .rel_index = 0,
                          .abs_index = abs_index,
                          .found = false};
}

auto GroupLayer::_find_layer(const LayerID id, isize abs_index) const
    -> FindConstLayerResult
{
  isize rel_index {0};

  for (const auto& layer : m_layers) {
    if (layer->info().id() == id) {
      return FindConstLayerResult {.parent_layer = this,
                                   .rel_index = rel_index,
                                   .abs_index = abs_index,
                                   .found = true};
    }

    ++rel_index;
    ++abs_index;

    if (const auto* group_layer =
            dynamic_cast<const GroupLayer*>(layer.get())) {
      const auto result = group_layer->_find_layer(id, abs_index);
      abs_index = result.abs_index;

      if (result.found) {
        return result;
      }
    }
  }

  return FindConstLayerResult {.parent_layer = nullptr,
                               .rel_index = 0,
                               .abs_index = abs_index,
                               .found = false};
}

}  // namespace tactile
