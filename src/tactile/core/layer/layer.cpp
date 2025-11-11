// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/layer/layer.hpp"

#include <algorithm>
#include <stdexcept>

namespace tactile {

Layer::Layer(const LayerId id):
    m_id {id}
{}

void Layer::set_opacity(const float32 opacity)
{
  m_opacity = std::clamp(opacity, 0.0f, 1.0f);
}

void Layer::set_visible(const bool visible)
{
  m_visible = visible;
}

}  // namespace tactile
