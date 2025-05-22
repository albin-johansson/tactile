// Copyright (C) 2025 Albin Johansson

module tactile.core.tile;

import tactile.core.log;

namespace tactile {

TileAnimation::TileAnimation(const TileAnimationFrame& first_frame)
{
  m_frames.push_back(first_frame);
}

void TileAnimation::update(const SteadyClock::time_point update_time)
{
  const auto& frame = current_frame();

  const auto elapsed_time = update_time - m_last_update;
  if (elapsed_time >= frame.duration) {
    m_current_frame = (m_current_frame + 1uz) % m_frames.size();
    m_last_update = update_time;
  }
}

auto TileAnimation::insert_frame(const isize index, const TileAnimationFrame& frame)
    -> Result<void>
{
  const auto frame_count = m_frames.size();

  if (index == 0 || std::cmp_greater(index, frame_count)) {
    get_logger().log(LogLevel::kError,
                     "Cannot insert tile animation frame at index {}",
                     index);
    return err(Error::kInvalidArg);
  }

  if (std::cmp_equal(index, frame_count)) {
    append_frame(frame);
  }
  else {
    m_frames.insert(m_frames.begin() + index, frame);
  }

  m_last_update = SteadyClock::now();
  m_current_frame = 0;

  return ok();
}

void TileAnimation::append_frame(const TileAnimationFrame& frame)
{
  m_frames.push_back(frame);
}

auto TileAnimation::erase_frame(const isize index) -> Result<TileAnimationFrame>
{
  if (index == 0 || index >= m_frames.size()) {
    return err(Error::kInvalidArg);
  }

  auto frame = m_frames.at(to_unsigned(index));
  m_frames.erase(m_frames.begin() + index);

  return frame;
}

auto TileAnimation::current_frame() const -> const TileAnimationFrame&
{
  return m_frames.at(m_current_frame);
}

auto TileAnimation::frame_count() const -> usize
{
  return m_frames.size();
}

}  // namespace tactile
