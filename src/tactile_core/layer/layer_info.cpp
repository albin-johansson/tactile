// Copyright (C) 2025 Albin Johansson

module tactile.layer;

import std;

namespace tactile {

LayerInfo::LayerInfo(const LayerID id)
  : m_id {id}
{}

void LayerInfo::set_opacity(const f32 opacity)
{
  m_opacity = std::clamp(opacity, 0.0f, 1.0f);
}

void LayerInfo::set_visible(const bool visible)
{
  m_visible = visible;
}

auto LayerInfo::id() const noexcept -> LayerID
{
  return m_id;
}

auto LayerInfo::opacity() const noexcept -> f32
{
  return m_opacity;
}

auto LayerInfo::visible() const noexcept -> bool
{
  return m_visible;
}

}  // namespace tactile
